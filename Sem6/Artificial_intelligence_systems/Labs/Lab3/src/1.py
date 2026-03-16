import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from sklearn.cluster import KMeans
from sklearn.preprocessing import StandardScaler
from sklearn.metrics import silhouette_score, davies_bouldin_score
import os

os.makedirs('../images', exist_ok=True)

print("=" * 60)
print("Анализ кластеризации данных pluton.csv методом K-means")
print("=" * 60)

data = pd.read_csv('../data/pluton.csv')
print(f"\nЗагружены данные из файла pluton.csv")
print(f"Размерность данных: {data.shape}")
print(f"Колонки: {list(data.columns)}")
print(f"\nПервые 5 строк данных:")
print(data.head())

X = data.values

def plot_clusters(X, labels, centers, title, filename, show_plot=True):
    plt.figure(figsize=(10, 6))
    colors = ['red', 'green', 'blue']
    for i in range(3):
        plt.scatter(X[labels == i, 0], X[labels == i, 1], c=colors[i], label=f'Cluster {i}', alpha=0.6)
    plt.scatter(centers[:, 0], centers[:, 1], c='black', marker='X', s=200, label='Centroids', edgecolors='white', linewidths=2)
    plt.xlabel('Pu238')
    plt.ylabel('Pu239')
    plt.title(title)
    plt.legend()
    plt.grid(True, alpha=0.3)
    
    plt.savefig(f'../images/{filename}', dpi=150, bbox_inches='tight')
    if show_plot:
        plt.show()
    else:
        plt.close()

print("\n" + "=" * 60)
print("ЧАСТЬ 1: Кластеризация без стандартизации")
print("=" * 60)

max_iters = [5, 10, 50, 100, 200]
results = []

fig, axes = plt.subplots(2, 3, figsize=(18, 10))
axes = axes.flatten()

for i, max_iter in enumerate(max_iters):
    print(f"\n--- Итерация {i+1}/{len(max_iters)}: max_iter = {max_iter} ---")
    
    kmeans = KMeans(n_clusters=3, max_iter=max_iter, random_state=42, n_init=10)
    labels = kmeans.fit_predict(X)
    centers = kmeans.cluster_centers_
    
    sil = silhouette_score(X, labels)
    db = davies_bouldin_score(X, labels)
    
    print(f"  Количество итераций фактически выполнено: {kmeans.n_iter_}")
    print(f"  Распределение по кластерам: {np.bincount(labels)}")
    print(f"  Координаты центроидов:")
    for j, center in enumerate(centers):
        print(f"    Кластер {j}: Pu238={center[0]:.4f}, Pu239={center[1]:.4f}, Pu240={center[2]:.4f}, Pu241={center[3]:.4f}")
    print(f"  Silhouette Score: {sil:.4f}")
    print(f"  Davies-Bouldin Index: {db:.4f}")
    
    results.append({'max_iter': max_iter, 'scaled': False, 'silhouette': sil, 'davies_bouldin': db, 'n_iter': kmeans.n_iter_})
    
    colors = ['red', 'green', 'blue']
    for j in range(3):
        axes[i].scatter(X[labels == j, 0], X[labels == j, 1], c=colors[j], label=f'Cluster {j}', alpha=0.6)
    axes[i].scatter(centers[:, 0], centers[:, 1], c='black', marker='X', s=200, label='Centroids', edgecolors='white', linewidths=2)
    axes[i].set_xlabel('Pu238')
    axes[i].set_ylabel('Pu239')
    axes[i].set_title(f'max_iter={max_iter}, Sil={sil:.3f}, DB={db:.3f}')
    axes[i].legend()
    axes[i].grid(True, alpha=0.3)
    
    plot_clusters(X, labels, centers, f'K-means с max_iter={max_iter} (без стандартизации)', f'clusters_iter_{max_iter}.png', show_plot=False)

axes[5].axis('off')
plt.suptitle('Кластеризация без стандартизации', fontsize=16)
plt.tight_layout()
plt.savefig('../images/all_clusters_no_scaled.png', dpi=150, bbox_inches='tight')
plt.show()

print("\n" + "=" * 60)
print("ЧАСТЬ 2: Кластеризация со стандартизацией")
print("=" * 60)

scaler = StandardScaler()
X_scaled = scaler.fit_transform(X)

print("\nСтатистика после стандартизации:")
print(f"  Среднее (первые 2 признака): {X_scaled[:, 0].mean():.4f}, {X_scaled[:, 1].mean():.4f}")
print(f"  Стандартное отклонение: {X_scaled[:, 0].std():.4f}, {X_scaled[:, 1].std():.4f}")

fig, axes = plt.subplots(2, 3, figsize=(18, 10))
axes = axes.flatten()

for i, max_iter in enumerate(max_iters):
    print(f"\n--- Итерация {i+1}/{len(max_iters)}: max_iter = {max_iter} (со стандартизацией) ---")
    
    kmeans = KMeans(n_clusters=3, max_iter=max_iter, random_state=42, n_init=10)
    labels = kmeans.fit_predict(X_scaled)
    centers_scaled = kmeans.cluster_centers_
    
    sil = silhouette_score(X_scaled, labels)
    db = davies_bouldin_score(X_scaled, labels)
    
    print(f"  Количество итераций фактически выполнено: {kmeans.n_iter_}")
    print(f"  Распределение по кластерам: {np.bincount(labels)}")
    print(f"  Координаты центроидов в масштабированном пространстве:")
    for j, center in enumerate(centers_scaled):
        print(f"    Кластер {j}: {center[0]:.4f}, {center[1]:.4f}, {center[2]:.4f}, {center[3]:.4f}")
    print(f"  Silhouette Score: {sil:.4f}")
    print(f"  Davies-Bouldin Index: {db:.4f}")
    
    results.append({'max_iter': max_iter, 'scaled': True, 'silhouette': sil, 'davies_bouldin': db, 'n_iter': kmeans.n_iter_})
    
    centers_original = scaler.inverse_transform(centers_scaled)
    
    colors = ['red', 'green', 'blue']
    for j in range(3):
        axes[i].scatter(X[labels == j, 0], X[labels == j, 1], c=colors[j], label=f'Cluster {j}', alpha=0.6)
    axes[i].scatter(centers_original[:, 0], centers_original[:, 1], c='black', marker='X', s=200, label='Centroids', edgecolors='white', linewidths=2)
    axes[i].set_xlabel('Pu238')
    axes[i].set_ylabel('Pu239')
    axes[i].set_title(f'max_iter={max_iter}, Sil={sil:.3f}, DB={db:.3f}')
    axes[i].legend()
    axes[i].grid(True, alpha=0.3)
    
    plot_clusters(X, labels, centers_original, f'K-means с max_iter={max_iter} (со стандартизацией)', f'clusters_scaled_iter_{max_iter}.png', show_plot=False)

axes[5].axis('off')
plt.suptitle('Кластеризация со стандартизацией', fontsize=16)
plt.tight_layout()
plt.savefig('../images/all_clusters_scaled.png', dpi=150, bbox_inches='tight')
plt.show()

print("\n" + "=" * 60)
print("ЧАСТЬ 3: Сводная таблица результатов")
print("=" * 60)

results_df = pd.DataFrame(results)
results_df = results_df[['max_iter', 'scaled', 'n_iter', 'silhouette', 'davies_bouldin']]
results_df['scaled'] = results_df['scaled'].map({False: 'Нет', True: 'Да'})
print("\nСводная таблица результатов кластеризации:")
print(results_df.to_string(index=False))

print("\n" + "=" * 60)
print("ЧАСТЬ 4: Сравнительный анализ")
print("=" * 60)

best_silhouette = results_df.loc[results_df['silhouette'].idxmax()]
worst_silhouette = results_df.loc[results_df['silhouette'].idxmin()]
best_db = results_df.loc[results_df['davies_bouldin'].idxmin()]
worst_db = results_df.loc[results_df['davies_bouldin'].idxmax()]

print("\nЛучший результат по Silhouette Score:")
print(f"  max_iter={best_silhouette['max_iter']}, Стандартизация: {best_silhouette['scaled']}")
print(f"  Silhouette Score: {best_silhouette['silhouette']:.4f}")
print(f"  Davies-Bouldin Index: {best_silhouette['davies_bouldin']:.4f}")

print("\nЛучший результат по Davies-Bouldin Index:")
print(f"  max_iter={best_db['max_iter']}, Стандартизация: {best_db['scaled']}")
print(f"  Davies-Bouldin Index: {best_db['davies_bouldin']:.4f}")
print(f"  Silhouette Score: {best_db['silhouette']:.4f}")

print("\n" + "=" * 60)
print("ЧАСТЬ 5: Построение сравнительных графиков")
print("=" * 60)

fig = plt.figure(figsize=(16, 10))

ax1 = plt.subplot(2, 3, 1)
no_scaled = results_df[results_df['scaled'] == 'Нет']
scaled = results_df[results_df['scaled'] == 'Да']
ax1.plot(no_scaled['max_iter'], no_scaled['silhouette'], marker='o', linewidth=2, label='Без стандартизации')
ax1.plot(scaled['max_iter'], scaled['silhouette'], marker='s', linewidth=2, label='Со стандартизацией')
ax1.set_xlabel('Максимальное число итераций (max_iter)')
ax1.set_ylabel('Silhouette Score')
ax1.set_title('Зависимость Silhouette Score от max_iter')
ax1.legend()
ax1.grid(True, alpha=0.3)

ax2 = plt.subplot(2, 3, 2)
ax2.plot(no_scaled['max_iter'], no_scaled['davies_bouldin'], marker='o', linewidth=2, label='Без стандартизации')
ax2.plot(scaled['max_iter'], scaled['davies_bouldin'], marker='s', linewidth=2, label='Со стандартизацией')
ax2.set_xlabel('Максимальное число итераций (max_iter)')
ax2.set_ylabel('Davies-Bouldin Index')
ax2.set_title('Зависимость Davies-Bouldin Index от max_iter')
ax2.legend()
ax2.grid(True, alpha=0.3)

ax3 = plt.subplot(2, 3, 3)
sil_no_scaled = no_scaled['silhouette'].values
sil_scaled = scaled['silhouette'].values
box1 = ax3.boxplot([sil_no_scaled, sil_scaled], tick_labels=['Без\nстандартизации', 'Со\nстандартизацией'])
ax3.set_ylabel('Silhouette Score')
ax3.set_title('Сравнение Silhouette Score')
ax3.grid(True, alpha=0.3)

ax4 = plt.subplot(2, 3, 4)
db_no_scaled = no_scaled['davies_bouldin'].values
db_scaled = scaled['davies_bouldin'].values
box2 = ax4.boxplot([db_no_scaled, db_scaled], tick_labels=['Без\nстандартизации', 'Со\nстандартизацией'])
ax4.set_ylabel('Davies-Bouldin Index')
ax4.set_title('Сравнение Davies-Bouldin Index')
ax4.grid(True, alpha=0.3)

ax5 = plt.subplot(2, 3, 5)
x = np.arange(len(max_iters))
width = 0.35
ax5.bar(x - width/2, no_scaled['silhouette'], width, label='Без стандартизации')
ax5.bar(x + width/2, scaled['silhouette'], width, label='Со стандартизацией')
ax5.set_xlabel('max_iter')
ax5.set_ylabel('Silhouette Score')
ax5.set_title('Сравнение Silhouette Score')
ax5.set_xticks(x)
ax5.set_xticklabels(max_iters)
ax5.legend()
ax5.grid(True, alpha=0.3, axis='y')

ax6 = plt.subplot(2, 3, 6)
ax6.bar(x - width/2, no_scaled['davies_bouldin'], width, label='Без стандартизации')
ax6.bar(x + width/2, scaled['davies_bouldin'], width, label='Со стандартизацией')
ax6.set_xlabel('max_iter')
ax6.set_ylabel('Davies-Bouldin Index')
ax6.set_title('Сравнение Davies-Bouldin Index')
ax6.set_xticks(x)
ax6.set_xticklabels(max_iters)
ax6.legend()
ax6.grid(True, alpha=0.3, axis='y')

plt.suptitle('Сравнительный анализ качества кластеризации', fontsize=16)
plt.tight_layout()
plt.savefig('../images/comparison_analysis.png', dpi=150, bbox_inches='tight')
plt.show()

print("\nГрафики сохранены в папку ../images/")
print("=" * 60)
print("АНАЛИЗ РЕЗУЛЬТАТОВ:")
print("=" * 60)

avg_sil_no_scaled = no_scaled['silhouette'].mean()
avg_sil_scaled = scaled['silhouette'].mean()
avg_db_no_scaled = no_scaled['davies_bouldin'].mean()
avg_db_scaled = scaled['davies_bouldin'].mean()

print(f"\nСредний Silhouette Score без стандартизации: {avg_sil_no_scaled:.4f}")
print(f"Средний Silhouette Score со стандартизацией: {avg_sil_scaled:.4f}")
print(f"Средний Davies-Bouldin Index без стандартизации: {avg_db_no_scaled:.4f}")
print(f"Средний Davies-Bouldin Index со стандартизацией: {avg_db_scaled:.4f}")

print("\nВЫВОДЫ:")
if avg_sil_scaled > avg_sil_no_scaled:
    print("✓ Стандартизация улучшает качество кластеризации (более высокий Silhouette Score)")
else:
    print("✗ Стандартизация ухудшает качество кластеризации (более низкий Silhouette Score)")

if avg_db_scaled < avg_db_no_scaled:
    print("✓ Стандартизация улучшает качество кластеризации (более низкий Davies-Bouldin Index)")
else:
    print("✗ Стандартизация ухудшает качество кластеризации (более высокий Davies-Bouldin Index)")

print(f"\nАлгоритм K-means со стандартизацией сходится за {scaled['n_iter'].iloc[0]} итераций")
print(f"Алгоритм K-means без стандартизации сходится за {no_scaled['n_iter'].iloc[0]} итераций")

print("\nПрограмма завершена.")
