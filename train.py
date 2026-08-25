import subprocess
import time
import threading 

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
policy = QNetwork(obs_dim=5)

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
    obs = [
        data["head"]["x"],
        data["head"]["y"],
        data["direction"],
        data["apple"]["x"],
        data["apple"]["y"],
    ]
    return torch.tensor(obs, dtype=torch.float32)

def train_step(state, action, reward, done, next_state, policy):

    if done:
        target = torch.tensor(reward)
    else:
        with torch.no_grad():
            target = reward + discount * policy(next_state).max()

    prediction = policy(state)[action]

    loss = loss_fn(prediction, target)
    print(f"Loss: {loss}")

    optimizer.zero_grad()
    loss.backward()
    optimizer.step()

context = zmq.Context()
socket = context.socket(zmq.REP)
socket.bind("tcp://localhost:5555")

def loop():
    while True:
        
        # receive observation from game 
        initial = socket.recv_string()
        #print(f"Received reply: {initial}")
        
        obs = parse_observation_tensor(initial)
        action = select_action(obs, policy, epsilon=0.1)
        #print(f"Selected action:  {action}")
        
        # send action to game 
        reply = {"direction": action}
        socket.send_string(json.dumps(reply))

        # get reward
        reward = socket.recv_string()
        #print(f"Received reply: {reward}")
        reply = {"ack": 1}
        socket.send_string(json.dumps(reply))

        data = json.loads(reward)

        r = data["reward"]
        done = data["done"]
        next_state = parse_observation_tensor(json.dumps(data["state"]))

        # train step
        train_step(obs, action, r, done, next_state, policy)

thread = threading.Thread(target=loop)
thread.start()

# Launch the executable and immediately move to the next line
process = subprocess.Popen(["./build/snake", "--train"])
thread.join()