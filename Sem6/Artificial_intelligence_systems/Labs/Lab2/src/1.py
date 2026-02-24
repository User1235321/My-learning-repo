import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import torch
import torch.nn as nn
import torch.optim as optim
import os
from sklearn.preprocessing import StandardScaler

os.makedirs('../images', exist_ok=True)

def load_data(filename):
    df = pd.read_csv(filename)
    X = df[['X1', 'X2']].values.astype(np.float32)
    y = df['class'].values.astype(np.float32).reshape(-1, 1)
    return X, y

X0, y0 = load_data('../data/nn_0.csv')
X1, y1 = load_data('../data/nn_1.csv')

def plot_data(X, y, title, filename):
    plt.figure(figsize=(5,4))
    plt.scatter(X[y.flatten()==1,0], X[y.flatten()==1,1], c='blue', label='class 1')
    plt.scatter(X[y.flatten()==-1,0], X[y.flatten()==-1,1], c='red', label='class -1')
    plt.xlabel('X1')
    plt.ylabel('X2')
    plt.title(title)
    plt.legend()
    plt.grid(True)
    plt.savefig(f'../images/{filename}')
    plt.close()

plot_data(X0, y0, 'Dataset nn_0.csv', 'plot_nn_0.png')
plot_data(X1, y1, 'Dataset nn_1.csv', 'plot_nn_1.png')

scaler = StandardScaler()
X0_scaled = scaler.fit_transform(X0)
X1_scaled = scaler.transform(X1)

X0_t = torch.tensor(X0_scaled, dtype=torch.float32)
y0_t = torch.tensor(y0, dtype=torch.float32)
X1_t = torch.tensor(X1_scaled, dtype=torch.float32)
y1_t = torch.tensor(y1, dtype=torch.float32)

class OneNeuron(nn.Module):
    def __init__(self, activation='tanh'):
        super().__init__()
        self.linear = nn.Linear(2, 1)
        if activation == 'tanh':
            self.act = nn.Tanh()
        elif activation == 'sigmoid':
            self.act = nn.Sigmoid()
        elif activation == 'linear':
            self.act = nn.Identity()
        else:
            raise ValueError("Unsupported activation")
    
    def forward(self, x):
        return self.act(self.linear(x))

def train_model(model, X, y, optimizer, criterion, epochs=200):
    losses = []
    for epoch in range(epochs):
        optimizer.zero_grad()
        outputs = model(X)
        loss = criterion(outputs, y)
        loss.backward()
        optimizer.step()
        losses.append(loss.item())
    return losses

def accuracy(model, X, y):
    with torch.no_grad():
        outputs = model(X)
        preds = torch.sign(outputs)
        if isinstance(model.act, nn.Sigmoid):
            preds = (outputs > 0.5).float() * 2 - 1
        correct = (preds == y).float().sum()
        return correct / y.numel()

datasets = {'nn_0': (X0_t, y0_t), 'nn_1': (X1_t, y1_t)}
activations = ['tanh', 'sigmoid', 'linear']
optimizers_config = {
    'SGD': lambda params: optim.SGD(params, lr=0.1),
    'Adam': lambda params: optim.Adam(params, lr=0.01)
}
criterion = nn.MSELoss()

results = []
for data_name, (X, y) in datasets.items():
    for act in activations:
        for opt_name, opt_builder in optimizers_config.items():
            print(f"\n--- Dataset: {data_name}, Activation: {act}, Optimizer: {opt_name} ---")
            model = OneNeuron(activation=act)
            optimizer = opt_builder(model.parameters())
            losses = train_model(model, X, y, optimizer, criterion, epochs=200)
            acc = accuracy(model, X, y)
            print(f'Final accuracy: {acc.item()*100:.2f}%')
            results.append({
                'dataset': data_name,
                'activation': act,
                'optimizer': opt_name,
                'final_loss': losses[-1],
                'accuracy': acc.item()
            })

df_results = pd.DataFrame(results)
print("\n=== Сводные результаты ===")
print(df_results.to_string(index=False))
