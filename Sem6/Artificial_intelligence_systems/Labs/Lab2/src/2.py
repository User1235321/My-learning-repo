import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import torch
import torch.nn as nn
import torch.optim as optim
from sklearn.preprocessing import StandardScaler
from sklearn.model_selection import train_test_split
import os

os.makedirs('../images', exist_ok=True)

def load_data(filename):
    df = pd.read_csv(filename)
    X = df[['X1', 'X2']].values.astype(np.float32)
    y = df['class'].values.astype(np.float32).reshape(-1, 1)
    return X, y

X, y = load_data('../data/nn_1.csv')

X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)

scaler = StandardScaler()
X_train_scaled = scaler.fit_transform(X_train)
X_test_scaled = scaler.transform(X_test)

X_train_t = torch.tensor(X_train_scaled, dtype=torch.float32)
y_train_t = torch.tensor(y_train, dtype=torch.float32)
X_test_t = torch.tensor(X_test_scaled, dtype=torch.float32)
y_test_t = torch.tensor(y_test, dtype=torch.float32)

class NeuralNetwork(nn.Module):
    def __init__(self):
        super().__init__()
        self.fc1 = nn.Linear(2, 20)
        self.fc2 = nn.Linear(20, 10)
        self.fc3 = nn.Linear(10, 1)
        self.relu = nn.ReLU()
        self.tanh = nn.Tanh()
    
    def forward(self, x):
        x = self.relu(self.fc1(x))
        x = self.relu(self.fc2(x))
        x = self.tanh(self.fc3(x))
        return x

model = NeuralNetwork()
criterion = nn.MSELoss()
optimizer = optim.Adam(model.parameters(), lr=0.01)

epochs = 500
train_losses = []
test_losses = []

for epoch in range(epochs):
    model.train()
    optimizer.zero_grad()
    outputs = model(X_train_t)
    loss = criterion(outputs, y_train_t)
    loss.backward()
    optimizer.step()
    train_losses.append(loss.item())
    
    model.eval()
    with torch.no_grad():
        test_outputs = model(X_test_t)
        test_loss = criterion(test_outputs, y_test_t)
        test_losses.append(test_loss.item())

def accuracy(model, X, y):
    model.eval()
    with torch.no_grad():
        preds = torch.sign(model(X))
        correct = (preds == y).float().sum()
        return correct / y.numel()

train_acc = accuracy(model, X_train_t, y_train_t)
test_acc = accuracy(model, X_test_t, y_test_t)

print(f'Точность на обучении: {train_acc.item()*100:.2f}%')
print(f'Точность на тесте: {test_acc.item()*100:.2f}%')

plt.figure(figsize=(10,4))
plt.subplot(1,2,1)
plt.plot(train_losses, label='Train loss')
plt.plot(test_losses, label='Test loss')
plt.xlabel('Epoch')
plt.ylabel('MSE Loss')
plt.legend()
plt.grid(True)

plt.subplot(1,2,2)
x_min, x_max = X_test_scaled[:,0].min() - 0.5, X_test_scaled[:,0].max() + 0.5
y_min, y_max = X_test_scaled[:,1].min() - 0.5, X_test_scaled[:,1].max() + 0.5
xx, yy = np.meshgrid(np.linspace(x_min, x_max, 200), np.linspace(y_min, y_max, 200))
grid = torch.tensor(np.c_[xx.ravel(), yy.ravel()], dtype=torch.float32)
with torch.no_grad():
    Z = model(grid)
    Z = torch.sign(Z).numpy().reshape(xx.shape)

plt.contourf(xx, yy, Z, alpha=0.3, cmap='coolwarm')
plt.scatter(X_test_scaled[:,0], X_test_scaled[:,1], c=y_test.flatten(), cmap='coolwarm', edgecolors='k')
plt.xlabel('X1 (норм.)')
plt.ylabel('X2 (норм.)')
plt.title('Разделяющая поверхность (тест)')
plt.grid(True)

plt.tight_layout()
plt.savefig('../images/nn1_decision_boundary.png')
plt.close()

print("График сохранён в ../images/nn1_decision_boundary.png")
