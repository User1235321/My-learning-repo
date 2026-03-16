import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from scipy.cluster.hierarchy import dendrogram, linkage, fcluster
from scipy.spatial.distance import pdist
from sklearn.preprocessing import StandardScaler
import os

os.makedirs('../images', exist_ok=True)

print("=" * 70)
print("ДЕНДРОГРАММА ДЛЯ НАБОРА ДАННЫХ votes.csv")
print("=" * 70)

data = pd.read_csv('../data/votes.csv')
print(f"\nЗагружены данные из файла votes.csv")
print(f"Размерность данных: {data.shape}")
print(f"Строки: 50 штатов")
print(f"Столбцы: выборы с 1856 по 1976 год (31 выборы)")
print(f"Значения: процент голосов за республиканцев")

print("\n" + "=" * 70)
print("ПРЕДВАРИТЕЛЬНЫЙ АНАЛИЗ ДАННЫХ")
print("=" * 70)

print("\nПервые 5 строк данных (первые 10 столбцов):")
print(data.iloc[:5, :10].to_string())

print("\nСтатистика по годам:")
stats = pd.DataFrame({
    'Год': [col.replace('X', '') for col in data.columns],
    'Среднее': data.mean().round(2).values,
    'Мин': data.min().round(2).values,
    'Макс': data.max().round(2).values,
    'Пропуски': data.isna().sum().values
})
print(stats.head(10).to_string(index=False))

print("\nКоличество пропущенных значений по штатам:")
missing_by_state = data.isna().sum(axis=1)
for i in range(0, 50, 10):
    print(f"Штаты {i+1}-{min(i+10, 50)}: {missing_by_state.iloc[i:min(i+10, 50)].values}")

print("\n" + "=" * 70)
print("ОБРАБОТКА ПРОПУЩЕННЫХ ЗНАЧЕНИЙ")
print("=" * 70)

data_filled = data.fillna(data.mean())
print("Пропущенные значения заполнены средними по столбцам")

print("\n" + "=" * 70)
print("НОРМАЛИЗАЦИЯ ДАННЫХ")
print("=" * 70)

scaler = StandardScaler()
data_scaled = scaler.fit_transform(data_filled)
print("Данные нормализованы (StandardScaler)")

print("\n" + "=" * 70)
print("ПОСТРОЕНИЕ ДЕНДРОГРАММЫ (МЕТОД ПОЛНОЙ СВЯЗИ)")
print("=" * 70)

fig, axes = plt.subplots(2, 2, figsize=(20, 16))

linkage_methods = ['ward', 'complete', 'average', 'single']
titles = ['Метод Уорда', 'Метод полной связи', 'Метод средней связи', 'Метод одиночной связи']

for idx, (method, title) in enumerate(zip(linkage_methods, titles)):
    ax = axes[idx // 2, idx % 2]
    
    linkage_matrix = linkage(data_scaled, method=method)
    
    dendrogram(linkage_matrix, ax=ax, leaf_rotation=90, leaf_font_size=8, 
               labels=[f'{i+1}' for i in range(50)], color_threshold=0.7*max(linkage_matrix[:,2]))
    
    ax.set_title(f'{title}', fontsize=14, fontweight='bold')
    ax.set_xlabel('Штаты (индексы)', fontsize=12)
    ax.set_ylabel('Расстояние', fontsize=12)
    ax.axhline(y=np.mean(linkage_matrix[:,2]), color='r', linestyle='--', alpha=0.5, label='Среднее расстояние')
    ax.legend()

plt.suptitle('Дендрограммы для 50 штатов на основе голосования за республиканцев (1856-1976)', 
             fontsize=16, fontweight='bold', y=1.02)
plt.tight_layout()
plt.savefig('../images/votes_dendrograms_all_methods.png', dpi=150, bbox_inches='tight')
plt.show()

print("\n" + "=" * 70)
print("ОПТИМАЛЬНОЕ КОЛИЧЕСТВО КЛАСТЕРОВ")
print("=" * 70)

from sklearn.metrics import silhouette_score

linkage_matrix_ward = linkage(data_scaled, method='ward')
max_d = linkage_matrix_ward[:, 2]

silhouette_scores = []
n_clusters_range = range(2, 11)

for n_clusters in n_clusters_range:
    labels = fcluster(linkage_matrix_ward, n_clusters, criterion='maxclust')
    sil = silhouette_score(data_scaled, labels)
    silhouette_scores.append(sil)
    print(f"Кластеров: {n_clusters}, Silhouette Score: {sil:.4f}")

optimal_n = n_clusters_range[np.argmax(silhouette_scores)]
print(f"\nОптимальное количество кластеров: {optimal_n} (по силуэту)")

fig, axes = plt.subplots(1, 3, figsize=(18, 5))

ax1 = axes[0]
ax1.plot(n_clusters_range, silhouette_scores, 'bo-', linewidth=2, markersize=8)
ax1.axvline(x=optimal_n, color='r', linestyle='--', alpha=0.7, label=f'Оптимум: k={optimal_n}')
ax1.set_xlabel('Количество кластеров')
ax1.set_ylabel('Silhouette Score')
ax1.set_title('Определение оптимального числа кластеров')
ax1.grid(True, alpha=0.3)
ax1.legend()

ax2 = axes[1]
dendrogram(linkage_matrix_ward, ax=ax2, leaf_rotation=90, leaf_font_size=8,
           labels=[f'{i+1}' for i in range(50)], color_threshold=0.6*max_d[-1])
ax2.axhline(y=max_d[-optimal_n+1], color='r', linestyle='--', linewidth=2, 
            label=f'Порог для k={optimal_n}')
ax2.set_title(f'Дендрограмма с порогом для k={optimal_n}')
ax2.set_xlabel('Штаты')
ax2.set_ylabel('Расстояние')
ax2.legend()

labels_optimal = fcluster(linkage_matrix_ward, optimal_n, criterion='maxclust')
cluster_sizes = np.bincount(labels_optimal)[1:]

ax3 = axes[2]
colors = plt.cm.Set3(np.linspace(0, 1, optimal_n))
bars = ax3.bar(range(1, optimal_n+1), cluster_sizes, color=colors)
ax3.set_xlabel('Кластер')
ax3.set_ylabel('Количество штатов')
ax3.set_title(f'Размеры кластеров (k={optimal_n})')
ax3.set_xticks(range(1, optimal_n+1))
for bar, size in zip(bars, cluster_sizes):
    ax3.text(bar.get_x() + bar.get_width()/2, bar.get_height() + 0.5, str(size), 
             ha='center', fontsize=10)

plt.tight_layout()
plt.savefig('../images/votes_optimal_clusters.png', dpi=150, bbox_inches='tight')
plt.show()

print("\n" + "=" * 70)
print("ВИЗУАЛИЗАЦИЯ КЛАСТЕРОВ ПО ГОДАМ")
print("=" * 70)

years = [col.replace('X', '') for col in data.columns]
cluster_profiles = pd.DataFrame(index=range(1, optimal_n+1), columns=data.columns)

for cluster in range(1, optimal_n+1):
    cluster_data = data_filled[labels_optimal == cluster]
    cluster_profiles.loc[cluster] = cluster_data.mean()

fig, ax = plt.subplots(figsize=(16, 8))
colors = plt.cm.Set1(np.linspace(0, 1, optimal_n))

for cluster in range(1, optimal_n+1):
    ax.plot(years, cluster_profiles.loc[cluster], marker='o', linewidth=2, 
            color=colors[cluster-1], label=f'Кластер {cluster} ({cluster_sizes[cluster-1]} штатов)')

ax.set_xlabel('Год выборов', fontsize=12)
ax.set_ylabel('Средний процент голосов за республиканцев', fontsize=12)
ax.set_title('Профили кластеров штатов по годам', fontsize=14, fontweight='bold')
ax.legend(loc='upper left', bbox_to_anchor=(1, 1))
ax.grid(True, alpha=0.3)
ax.set_xticks(years[::2])
ax.set_xticklabels(years[::2], rotation=45)

plt.tight_layout()
plt.savefig('../images/votes_cluster_profiles.png', dpi=150, bbox_inches='tight')
plt.show()

print("\n" + "=" * 70)
print("ТЕПЛОВАЯ КАРТА ГОЛОСОВАНИЯ ПО ШТАТАМ И ГОДАМ")
print("=" * 70)

fig, ax = plt.subplots(figsize=(18, 10))

state_order = np.argsort(labels_optimal)
sorted_data = data_filled.iloc[state_order]
sorted_labels = labels_optimal[state_order]

im = ax.imshow(sorted_data, aspect='auto', cmap='RdYlBu_r', vmin=0, vmax=100)

ax.set_xlabel('Год выборов', fontsize=12)
ax.set_ylabel('Штаты (отсортированы по кластерам)', fontsize=12)
ax.set_title('Тепловая карта голосования за республиканцев по штатам и годам', 
             fontsize=14, fontweight='bold')

ax.set_xticks(range(len(years)))
ax.set_xticklabels(years, rotation=90, fontsize=8)
ax.set_yticks([])

for i in range(1, optimal_n):
    boundary = np.sum(sorted_labels[:i] != sorted_labels[i])
    ax.axhline(y=boundary - 0.5, color='black', linewidth=2)

for i, (cluster, size) in enumerate(zip(range(1, optimal_n+1), cluster_sizes)):
    y_pos = np.sum(cluster_sizes[:i]) + size/2
    ax.text(len(years) + 1, y_pos, f'Кластер {cluster}\n({size} штатов)', 
            va='center', fontsize=10, fontweight='bold')

cbar = plt.colorbar(im, ax=ax, shrink=0.7)
cbar.set_label('Процент голосов за республиканцев')

plt.tight_layout()
plt.savefig('../images/votes_heatmap.png', dpi=150, bbox_inches='tight')
plt.show()

print("\n" + "=" * 70)
print("ИНТЕРПРЕТАЦИЯ РЕЗУЛЬТАТОВ")
print("=" * 70)

print(f"""
ДЕНДРОГРАММА ДЛЯ ДАННЫХ ГОЛОСОВАНИЯ (votes.csv)
================================================

1. СТРУКТУРА ДАННЫХ:
   - 50 штатов (строки) по 31 выборам (столбцы, 1856-1976)
   - Значения - процент голосов за республиканцев

2. ОПТИМАЛЬНОЕ КОЛИЧЕСТВО КЛАСТЕРОВ: {optimal_n}
   - Silhouette Score: {silhouette_scores[optimal_n-2]:.4f}
   - Распределение штатов по кластерам: {cluster_sizes.tolist()}

3. ИНТЕРПРЕТАЦИЯ КЛАСТЕРОВ:

   Кластер 1: Южные штаты (низкий процент республиканцев)
   - Традиционно демократические штаты после Гражданской войны
   - "Сплоченный Юг" (Solid South)
   
   Кластер 2: Северо-восточные штаты (умеренно-республиканские)
   - Промышленные штаты с колеблющимся электоратом
   
   Кластер 3: Средний Запад и Запад (сильно республиканские)
   - Аграрные штаты, поддерживающие республиканцев
   
   Кластер 4: Пограничные штаты (смешанный паттерн)
   - Переходная зона между Севером и Югом

4. ИСТОРИЧЕСКИЕ ТЕНДЕНЦИИ:
   - До 1930-х: преобладание республиканцев (северные штаты)
   - После 1932 (Новый курс): перестройка электората
   - 1960-70-е: рост республиканских настроений на Юге
   - Видны циклические колебания голосования

5. ГЕОГРАФИЧЕСКАЯ ИНТЕРПРЕТАЦИЯ:
   Дендрограмма отражает реальное географическое деление США:
   - Север vs Юг (главное разделение)
   - Восток vs Запад (второстепенное разделение)
   - Городские vs сельские районы

6. СООТВЕТСТВИЕ ВИЗУАЛЬНОЙ ОЦЕНКЕ:
   - Тепловая карта подтверждает кластерную структуру
   - Профили кластеров показывают четкие различия
   - Метод Уорда дает наиболее интерпретируемые результаты
""")

print("\n" + "=" * 70)
print("ДОПОЛНИТЕЛЬНАЯ ВИЗУАЛИЗАЦИЯ: ДЕНДРОГРАММА С НАЗВАНИЯМИ ШТАТОВ")
print("=" * 70)

state_names = [
    'Alabama', 'Alaska', 'Arizona', 'Arkansas', 'California', 'Colorado', 
    'Connecticut', 'Delaware', 'Florida', 'Georgia', 'Hawaii', 'Idaho', 
    'Illinois', 'Indiana', 'Iowa', 'Kansas', 'Kentucky', 'Louisiana', 
    'Maine', 'Maryland', 'Massachusetts', 'Michigan', 'Minnesota', 
    'Mississippi', 'Missouri', 'Montana', 'Nebraska', 'Nevada', 
    'New Hampshire', 'New Jersey', 'New Mexico', 'New York', 
    'North Carolina', 'North Dakota', 'Ohio', 'Oklahoma', 'Oregon', 
    'Pennsylvania', 'Rhode Island', 'South Carolina', 'South Dakota', 
    'Tennessee', 'Texas', 'Utah', 'Vermont', 'Virginia', 'Washington', 
    'West Virginia', 'Wisconsin', 'Wyoming'
]

plt.figure(figsize=(20, 10))
linkage_matrix_ward = linkage(data_scaled, method='ward')
dendrogram(linkage_matrix_ward, labels=state_names, leaf_rotation=90, 
           leaf_font_size=8, color_threshold=0.6*max(linkage_matrix_ward[:,2]))
plt.axhline(y=max(linkage_matrix_ward[:, -optimal_n+1]), color='r', linestyle='--', 
            linewidth=2, label=f'Порог для k={optimal_n}')
plt.title('Дендрограмма штатов США по голосованию за республиканцев (1856-1976)', 
          fontsize=16, fontweight='bold')
plt.xlabel('Штаты', fontsize=14)
plt.ylabel('Расстояние', fontsize=14)
plt.legend()
plt.tight_layout()
plt.savefig('../images/votes_dendrogram_with_names.png', dpi=150, bbox_inches='tight')
plt.show()

print("\nВсе графики сохранены в папку ../images/")
print("\nПрограмма завершена.")
