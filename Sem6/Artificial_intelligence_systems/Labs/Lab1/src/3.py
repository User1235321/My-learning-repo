import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import os
from sklearn.model_selection import cross_val_score, StratifiedKFold
from sklearn.neighbors import KNeighborsClassifier
from sklearn.preprocessing import StandardScaler

os.makedirs('../images', exist_ok=True)

data = pd.read_csv('../data/glass.csv')
data = data.drop('Id', axis=1)
X = data.drop('Type', axis=1).values
y = data['Type'].values.astype(int)

scaler = StandardScaler()
X_scaled = scaler.fit_transform(X)

cv = StratifiedKFold(n_splits=5, shuffle=True, random_state=42)

k_range = range(1, 31)
mean_scores = []
std_scores = []

for k in k_range:
    knn = KNeighborsClassifier(n_neighbors=k, metric='euclidean')
    scores = cross_val_score(knn, X_scaled, y, cv=cv, scoring='accuracy')
    mean_scores.append(np.mean(scores))
    std_scores.append(np.std(scores))

plt.figure(figsize=(10,6))
plt.plot(k_range, mean_scores, 'o-', label='Средняя точность')
plt.fill_between(k_range, np.array(mean_scores)-np.array(std_scores),
                 np.array(mean_scores)+np.array(std_scores), alpha=0.2)
plt.xlabel('Количество соседей k')
plt.ylabel('Точность')
plt.title('Зависимость точности от k (евклидова метрика)')
plt.grid(True)
plt.legend()
plt.savefig('../images/knn_accuracy_vs_k.png', dpi=150)
plt.show()

metrics = ['euclidean', 'manhattan', 'chebyshev', 'minkowski']
results = {}
best_k_for_metric = {}
for metric in metrics:
    scores_metric = []
    for k in k_range:
        knn = KNeighborsClassifier(n_neighbors=k, metric=metric, p=3 if metric=='minkowski' else 2)
        scores = cross_val_score(knn, X_scaled, y, cv=cv, scoring='accuracy')
        scores_metric.append(np.mean(scores))
    best_k = k_range[np.argmax(scores_metric)]
    best_score = np.max(scores_metric)
    results[metric] = (best_k, best_score)
    best_k_for_metric[metric] = scores_metric

plt.figure(figsize=(10,6))
for metric in metrics:
    plt.plot(k_range, best_k_for_metric[metric], label=f'{metric} (best k={results[metric][0]})')
plt.xlabel('Количество соседей k')
plt.ylabel('Точность')
plt.title('Сравнение метрик расстояния')
plt.grid(True)
plt.legend()
plt.savefig('../images/knn_metrics_comparison.png', dpi=150)
plt.show()

best_metric = max(results, key=lambda m: results[m][1])
best_k = results[best_metric][0]
print(f"Лучшая метрика: {best_metric}, оптимальное k = {best_k}, точность = {results[best_metric][1]:.3f}")

knn_final = KNeighborsClassifier(n_neighbors=best_k, metric=best_metric, p=3 if best_metric=='minkowski' else 2)
knn_final.fit(X_scaled, y)

new_sample = np.array([[1.516, 11.7, 1.01, 1.19, 72.59, 0.43, 11.44, 0.02, 0.1]])
new_sample_scaled = scaler.transform(new_sample)
predicted_class = knn_final.predict(new_sample_scaled)[0]
print(f"Новый образец относится к типу стекла: {predicted_class}")
