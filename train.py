import subprocess
import time
import threading
import signal
import sys
import os
import shutil
import argparse
from datetime import datetime
from collections import deque

import zmq
import json
import random

import torch
import torch.nn as nn
import torch.nn.functional as F

CHECKPOINT_ROOT = "checkpoints"
BEST_DIR = os.path.join(CHECKPOINT_ROOT, "best")
BEST_MODEL_PATH = os.path.join(BEST_DIR, "best.pt")
BEST_ONNX_PATH = os.path.join(BEST_DIR, "best.onnx")
RECENT_WINDOW = 20
OBS_DIM = 12

parser = argparse.ArgumentParser(description="Train the snake DQN agent.")
parser.add_argument(
    "--resume",
    action="store_true",
    help=f"Continue training from {BEST_MODEL_PATH} instead of starting from scratch.",
)
args = parser.parse_args()

class QNetwork(nn.Module):
    def __init__(self, obs_dim, n_actions=3, hidden_dim=64):
        super().__init__()
        self.net = nn.Sequential(
            nn.Linear(obs_dim, hidden_dim),
            nn.ReLU(),
            nn.Linear(hidden_dim, hidden_dim),
            nn.ReLU(),
            nn.Linear(hidden_dim, n_actions)  # Q-values for each action
        )

    def forward(self, x):
        return self.net(x)
    
    
# network setup
policy = QNetwork(obs_dim=OBS_DIM)

if args.resume:
    if os.path.exists(BEST_MODEL_PATH):
        policy.load_state_dict(torch.load(BEST_MODEL_PATH))
        print(f"Resumed training from {BEST_MODEL_PATH}")
    else:
        print(f"--resume requested but no best model found at {BEST_MODEL_PATH}; starting from scratch.")
else:
    print("Starting training from scratch.")

optimizer = torch.optim.Adam(
    policy.parameters(),
    lr=1e-3
)

loss_fn = nn.SmoothL1Loss()
discount = 0.99

def select_action(state, policy_net, epsilon, n_actions=3):
    if random.random() < epsilon:
        return random.randint(0, n_actions - 1)
    with torch.no_grad():
        q_values = policy_net(state)
        return q_values.argmax().item()
    

def parse_observation_tensor(json_str):
    data = json.loads(json_str)

    obs = (
        data["danger"]
        + data["direction"]
        + data["foodDir"]
        + [data["distance"]]
    )

    return torch.tensor(obs, dtype=torch.float32)

def train_step(state, action, reward, done, next_state, policy):

    if done:
        target = torch.tensor(reward)
    else:
        with torch.no_grad():
            target = reward + discount * policy(next_state).max()

    prediction = policy(state)[action]

    loss = loss_fn(prediction, target)

    optimizer.zero_grad()
    loss.backward()
    optimizer.step()

    return loss.item()

def build_metadata():
    avg_recent = sum(recent_rewards) / len(recent_rewards) if recent_rewards else 0.0
    avg_overall = total_reward_sum / episode_num if episode_num else 0.0

    return {
        "timestamp": datetime.now().isoformat(timespec="seconds"),
        "episodes": episode_num,
        "avg_reward_recent": avg_recent,
        "recent_window": len(recent_rewards),
        "avg_reward_overall": avg_overall,
        "best_episode_reward": best_episode_reward,
        "hyperparameters": {
            "lr": 1e-3,
            "discount": discount,
            "hidden_dim": 64,
            "obs_dim": OBS_DIM,
            "n_actions": 3,
            "epsilon": 0.1,
        },
    }

def export_onnx(model, path):
    was_training = model.training
    model.eval()
    dummy_input = torch.zeros(1, OBS_DIM)
    torch.onnx.export(model, dummy_input, path, input_names=["state"], output_names=["q_values"], external_data=False)
    model.train(was_training)
    print(f"Exported ONNX model to {path}")

def save_checkpoint():
    timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
    run_dir = os.path.join(CHECKPOINT_ROOT, timestamp)
    os.makedirs(run_dir, exist_ok=True)

    model_path = os.path.join(run_dir, "model.pt")
    metadata_path = os.path.join(run_dir, "metadata.json")

    torch.save(policy.state_dict(), model_path)
    metadata = build_metadata()
    with open(metadata_path, "w") as f:
        json.dump(metadata, f, indent=2)

    print(f"Saved checkpoint to {run_dir}")

    if episode_num == 0:
        print("No completed episodes; skipping best-model comparison.")
        return

    best_metadata_path = os.path.join(BEST_DIR, "metadata.json")
    is_best = True
    if os.path.exists(best_metadata_path):
        with open(best_metadata_path) as f:
            best_metadata = json.load(f)
        is_best = metadata["avg_reward_recent"] > best_metadata.get("avg_reward_recent", float("-inf"))

    if is_best:
        os.makedirs(BEST_DIR, exist_ok=True)
        shutil.copyfile(model_path, BEST_MODEL_PATH)
        shutil.copyfile(metadata_path, best_metadata_path)
        print(f"New best model (avg_reward_recent={metadata['avg_reward_recent']:.1f}) -> {BEST_MODEL_PATH}")
        export_onnx(policy, BEST_ONNX_PATH)

context = zmq.Context()
socket = context.socket(zmq.REP)
socket.bind("tcp://localhost:5555")

episode_num = 0
episode_reward = 0.0
episode_steps = 0
episode_loss_sum = 0.0

total_reward_sum = 0.0
best_episode_reward = float("-inf")
recent_rewards = deque(maxlen=RECENT_WINDOW)

def loop():
    global episode_num, episode_reward, episode_steps, episode_loss_sum
    global total_reward_sum, best_episode_reward

    while True:

        # receive observation from game
        initial = socket.recv_string()

        obs = parse_observation_tensor(initial)
        action = select_action(obs, policy, epsilon=0.1)

        # send action to game
        reply = {"direction": action}
        socket.send_string(json.dumps(reply))

        # get reward
        reward = socket.recv_string()
        reply = {"ack": 1}
        socket.send_string(json.dumps(reply))

        data = json.loads(reward)

        r = data["reward"]
        done = data["done"]
        next_state = parse_observation_tensor(json.dumps(data["state"]))

        # train step
        loss = train_step(obs, action, r, done, next_state, policy)

        episode_reward += r
        episode_steps += 1
        episode_loss_sum += loss

        if done:
            episode_num += 1
            total_reward_sum += episode_reward
            best_episode_reward = max(best_episode_reward, episode_reward)
            recent_rewards.append(episode_reward)

            print(f"Episode {episode_num} | steps={episode_steps} | "
                  f"reward={episode_reward:.1f} | avg_loss={episode_loss_sum / episode_steps:.4f}")
            episode_reward = 0.0
            episode_steps = 0
            episode_loss_sum = 0.0

thread = threading.Thread(target=loop, daemon=True)

# Launch the executable and immediately move to the next line
process = subprocess.Popen(["./build/snake", "--train"])

def handle_shutdown(signum, frame):
    print(f"\nReceived signal {signum}, saving weights and shutting down...")
    print(f"Stopped after {episode_num} episodes.")
    save_checkpoint()
    if process.poll() is None:
        process.terminate()
    sys.exit(0)

signal.signal(signal.SIGINT, handle_shutdown)
signal.signal(signal.SIGTERM, handle_shutdown)

thread.start()
thread.join()