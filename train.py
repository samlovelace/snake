import subprocess
import time
import threading
import signal
import sys

import zmq
import json
import random

import torch
import torch.nn as nn
import torch.nn.functional as F

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
policy = QNetwork(obs_dim=12)

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

def save_weights(path="model_weights.pt"):
    torch.save(policy.state_dict(), path)
    print(f"Saved model weights to {path}")

context = zmq.Context()
socket = context.socket(zmq.REP)
socket.bind("tcp://localhost:5555")

episode_num = 0
episode_reward = 0.0
episode_steps = 0
episode_loss_sum = 0.0

def loop():
    global episode_num, episode_reward, episode_steps, episode_loss_sum

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
    save_weights()
    if process.poll() is None:
        process.terminate()
    sys.exit(0)

signal.signal(signal.SIGINT, handle_shutdown)
signal.signal(signal.SIGTERM, handle_shutdown)

thread.start()
thread.join()