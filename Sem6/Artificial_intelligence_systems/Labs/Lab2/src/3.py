import torch
import torch.nn as nn
import torch.optim as optim
import torchvision
import torchvision.transforms as transforms
import matplotlib.pyplot as plt
import numpy as np
import os
from sklearn.metrics import confusion_matrix, ConfusionMatrixDisplay

os.makedirs('../images', exist_ok=True)

device = torch.device('cuda' if torch.cuda.is_available() else 'cpu')
print(f'Using device: {device}')

transform = transforms.Compose([
    transforms.ToTensor(),
    transforms.Normalize((0.1307,), (0.3081,))
])

train_dataset = torchvision.datasets.MNIST(root='../data', train=True, download=True, transform=transform)
test_dataset = torchvision.datasets.MNIST(root='../data', train=False, download=True, transform=transform)

train_loader = torch.utils.data.DataLoader(train_dataset, batch_size=64, shuffle=True)
test_loader = torch.utils.data.DataLoader(test_dataset, batch_size=1000, shuffle=False)

class CNN(nn.Module):
    def __init__(self):
        super().__init__()
        self.conv1 = nn.Conv2d(1, 32, kernel_size=3, padding=1)
        self.conv2 = nn.Conv2d(32, 64, kernel_size=3, padding=1)
        self.pool = nn.MaxPool2d(2, 2)
        self.fc1 = nn.Linear(64 * 7 * 7, 128)
        self.fc2 = nn.Linear(128, 10)
        self.dropout = nn.Dropout(0.25)
        self.relu = nn.ReLU()

    def forward(self, x):
        x = self.pool(self.relu(self.conv1(x)))
        x = self.pool(self.relu(self.conv2(x)))
        x = x.view(-1, 64 * 7 * 7)
        x = self.dropout(self.relu(self.fc1(x)))
        x = self.fc2(x)
        return x

model = CNN().to(device)
criterion = nn.CrossEntropyLoss()
optimizer = optim.Adam(model.parameters(), lr=0.001)

print("Архитектура сети:")
print(model)

total_params = sum(p.numel() for p in model.parameters())
trainable_params = sum(p.numel() for p in model.parameters() if p.requires_grad)
print(f"Всего параметров: {total_params}")
print(f"Обучаемых параметров: {trainable_params}")

def train(epoch):
    model.train()
    running_loss = 0.0
    correct = 0
    total = 0
    for data, target in train_loader:
        data, target = data.to(device), target.to(device)
        optimizer.zero_grad()
        output = model(data)
        loss = criterion(output, target)
        loss.backward()
        optimizer.step()
        running_loss += loss.item()
        _, predicted = output.max(1)
        total += target.size(0)
        correct += predicted.eq(target).sum().item()
    train_loss = running_loss / len(train_loader)
    train_acc = 100. * correct / total
    return train_loss, train_acc

def test():
    model.eval()
    test_loss = 0.0
    correct = 0
    total = 0
    all_preds = []
    all_targets = []
    with torch.no_grad():
        for data, target in test_loader:
            data, target = data.to(device), target.to(device)
            output = model(data)
            test_loss += criterion(output, target).item()
            _, predicted = output.max(1)
            total += target.size(0)
            correct += predicted.eq(target).sum().item()
            all_preds.extend(predicted.cpu().numpy())
            all_targets.extend(target.cpu().numpy())
    test_loss /= len(test_loader)
    test_acc = 100. * correct / total
    return test_loss, test_acc, all_preds, all_targets

train_losses, test_losses = [], []
train_accs, test_accs = [], []

print("Начало обучения...")
for epoch in range(5):
    train_loss, train_acc = train(epoch)
    test_loss, test_acc, _, _ = test()
    train_losses.append(train_loss)
    test_losses.append(test_loss)
    train_accs.append(train_acc)
    test_accs.append(test_acc)
    print(f'Epoch {epoch+1}: Train Loss: {train_loss:.4f}, Train Acc: {train_acc:.2f}%, Test Acc: {test_acc:.2f}%')

plt.figure(figsize=(12,4))
plt.subplot(1,2,1)
plt.plot(range(1,6), train_losses, label='Train Loss')
plt.plot(range(1,6), test_losses, label='Test Loss')
plt.xlabel('Epoch')
plt.ylabel('Loss')
plt.legend()
plt.grid(True)

plt.subplot(1,2,2)
plt.plot(range(1,6), train_accs, label='Train Acc')
plt.plot(range(1,6), test_accs, label='Test Acc')
plt.xlabel('Epoch')
plt.ylabel('Accuracy (%)')
plt.legend()
plt.grid(True)
plt.savefig('../images/mnist_metrics.png')
plt.close()

_, test_acc, all_preds, all_targets = test()
print(f"Итоговая точность на тесте: {test_acc:.2f}%")

cm = confusion_matrix(all_targets, all_preds)
disp = ConfusionMatrixDisplay(confusion_matrix=cm)
disp.plot(cmap='Blues')
plt.title('Confusion Matrix')
plt.savefig('../images/mnist_confusion_matrix.png')
plt.close()

def visualize_filters(layer, title, filename):
    filters = layer.weight.data.cpu().numpy()
    fig, axes = plt.subplots(4, 8, figsize=(12, 6))
    fig.suptitle(title)
    for i, ax in enumerate(axes.flat):
        if i < filters.shape[0]:
            ax.imshow(filters[i, 0], cmap='viridis')
            ax.axis('off')
    plt.tight_layout()
    plt.savefig(f'../images/{filename}')
    plt.close()

visualize_filters(model.conv1, 'Фильтры первого свёрточного слоя (32 фильтра 3x3)', 'mnist_filters_conv1.png')

def visualize_feature_maps(image_tensor, model, layer, title, filename):
    activation = {}
    def hook_fn(m, i, o):
        activation['value'] = o.detach()
    handle = layer.register_forward_hook(hook_fn)
    model.eval()
    with torch.no_grad():
        _ = model(image_tensor.unsqueeze(0).to(device))
    handle.remove()
    feature_maps = activation['value'].squeeze(0).cpu().numpy()
    fig, axes = plt.subplots(4, 8, figsize=(12, 6))
    fig.suptitle(title)
    for i, ax in enumerate(axes.flat):
        if i < feature_maps.shape[0]:
            ax.imshow(feature_maps[i], cmap='viridis')
            ax.axis('off')
    plt.tight_layout()
    plt.savefig(f'../images/{filename}')
    plt.close()

sample_image, sample_label = test_dataset[0]
sample_image_orig = sample_image.squeeze().numpy()
plt.imshow(sample_image_orig, cmap='gray')
plt.title(f'Тестовое изображение (цифра {sample_label})')
plt.savefig('../images/mnist_sample.png')
plt.close()

visualize_feature_maps(sample_image, model, model.conv1, 'Feature maps после первого свёрточного слоя', 'mnist_feature_maps_conv1.png')
visualize_feature_maps(sample_image, model, model.conv2, 'Feature maps после второго свёрточного слоя', 'mnist_feature_maps_conv2.png')

print("Все изображения сохранены в папку ../images/")
