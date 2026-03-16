import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from sklearn.model_selection import cross_val_score
from sklearn.ensemble import BaggingClassifier
from sklearn.tree import DecisionTreeClassifier
from sklearn.neighbors import KNeighborsClassifier
from sklearn.svm import SVC
from sklearn.naive_bayes import GaussianNB
from sklearn.preprocessing import StandardScaler
import os
import time

os.makedirs('../images', exist_ok=True)

print("Загрузка данных...")
df = pd.read_csv('../data/glass.csv')
X = df.iloc[:, 1:-1].values
y = df.iloc[:, -1].values
print(f"Размер данных: {X.shape}")
print(f"Количество классов: {len(np.unique(y))}")
print()

print("Нормализация данных...")
scaler = StandardScaler()
X = scaler.fit_transform(X)
print("Нормализация завершена")
print()

base_classifiers = {
    'Decision Tree': DecisionTreeClassifier(random_state=42),
    'k-NN': KNeighborsClassifier(),
    'SVM': SVC(random_state=42),
    'Naive Bayes': GaussianNB()
}

n_estimators_range = [1, 2, 3, 5, 7, 10, 15, 20, 25, 30, 35, 40, 45, 50]
results = {name: [] for name in base_classifiers.keys()}
training_times = {name: [] for name in base_classifiers.keys()}

print("Начало обучения ансамблей...")
print("="*60)

for name, base_clf in base_classifiers.items():
    print(f"\nБазовый классификатор: {name}")
    print("-"*40)
    
    for n in n_estimators_range:
        start_time = time.time()
        
        bagging = BaggingClassifier(
            estimator=base_clf,
            n_estimators=n,
            random_state=42,
            n_jobs=-1
        )
        
        scores = cross_val_score(bagging, X, y, cv=5, scoring='accuracy')
        mean_score = scores.mean()
        std_score = scores.std()
        
        elapsed_time = time.time() - start_time
        
        results[name].append(mean_score)
        training_times[name].append(elapsed_time)
        
        print(f"  n={n:2d}: accuracy = {mean_score:.4f} (+/- {std_score:.4f}), время = {elapsed_time:.2f}с")
    
    best_idx = np.argmax(results[name])
    print(f"\n  Лучший результат: {results[name][best_idx]:.4f} при n={n_estimators_range[best_idx]}")
    print(f"  Улучшение: {(results[name][-1] - results[name][0])*100:.2f}%")
    print()

print("="*60)
print("\nПостроение графиков...")

plt.figure(figsize=(12, 8))
for name, scores in results.items():
    plt.plot(n_estimators_range, scores, marker='o', label=name, linewidth=2)

plt.xlabel('Количество классификаторов в ансамбле')
plt.ylabel('Точность (cross-validation)')
plt.title('Зависимость качества классификации от размера ансамбля')
plt.legend()
plt.grid(True, alpha=0.3)
plt.savefig('../images/bagging_comparison.png', dpi=300, bbox_inches='tight')
plt.show()

plt.figure(figsize=(12, 8))
for name, times in training_times.items():
    plt.plot(n_estimators_range, times, marker='s', label=name, linewidth=2)

plt.xlabel('Количество классификаторов в ансамбле')
plt.ylabel('Время обучения (секунды)')
plt.title('Зависимость времени обучения от размера ансамбля')
plt.legend()
plt.grid(True, alpha=0.3)
plt.savefig('../images/bagging_timing.png', dpi=300, bbox_inches='tight')
plt.show()

plt.figure(figsize=(15, 10))
for i, (name, scores) in enumerate(results.items(), 1):
    plt.subplot(2, 2, i)
    plt.plot(n_estimators_range, scores, marker='o', color='blue', linewidth=2)
    plt.xlabel('Количество классификаторов')
    plt.ylabel('Точность')
    plt.title(f'Бэггинг с {name}')
    plt.grid(True, alpha=0.3)
    
plt.tight_layout()
plt.savefig('../images/bagging_individual.png', dpi=300, bbox_inches='tight')
plt.show()

print("\nИтоговые результаты:")
print("="*60)
for name, scores in results.items():
    best_n = n_estimators_range[np.argmax(scores)]
    best_score = max(scores)
    first_score = scores[0]
    final_score = scores[-1]
    improvement = (final_score - first_score) * 100
    
    print(f"\n{name}:")
    print(f"  Начальная точность (n=1):  {first_score:.4f}")
    print(f"  Лучшая точность:            {best_score:.4f} (n={best_n})")
    print(f"  Финальная точность (n=50):  {final_score:.4f}")
    print(f"  Общее улучшение:            {improvement:.2f}%")
    
    if best_n < 50:
        print(f"  Оптимальный размер ансамбля: {best_n}")
    else:
        print(f"  Точность продолжает расти при n=50")

print("\nГрафики сохранены в директорию ../images/")
