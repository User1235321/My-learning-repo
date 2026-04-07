import os
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import itertools
import statsmodels.api as sm

data_path = os.path.join('..', 'data', 'reglab.txt')
images_path = os.path.join('..', 'images')
os.makedirs(images_path, exist_ok=True)

df = pd.read_csv(data_path, sep=r'\s+')
print('Предварительная информация:')
print('Первые 5 строк данных:')
print(df.head())
print('\nОписательные статистики:')
print(df.describe())

features = ['x1', 'x2', 'x3', 'x4']
target = 'y'
X_full = df[features]
y = df[target]

results = []

for k in range(1, len(features)):
    best_rss = np.inf
    best_combo = None
    best_model = None
    for combo in itertools.combinations(features, k):
        X = sm.add_constant(df[list(combo)])
        model = sm.OLS(y, X).fit()
        rss = model.ssr
        if rss < best_rss:
            best_rss = rss
            best_combo = combo
            best_model = model
    results.append({
        'k': k,
        'features': best_combo,
        'rss': best_rss,
        'r2': best_model.rsquared,
        'aic': best_model.aic,
        'bic': best_model.bic
    })
    print(f'\nЛучшая модель с {k} признаками:')
    print(f'Признаки: {best_combo}')
    print(f'RSS = {best_rss:.4f}')
    print(f'R^2 = {best_model.rsquared:.4f}')
    print(f'AIC = {best_model.aic:.4f}')
    print(f'BIC = {best_model.bic:.4f}')

best_by_bic = min(results, key=lambda x: x['bic'])
print('\nОкончательная информация:')
print('Оптимальное подмножество признаков по минимальному BIC:')
print(f'K = {best_by_bic["k"]}')
print(f'Признаки: {best_by_bic["features"]}')
print(f'RSS = {best_by_bic["rss"]:.4f}')
print(f'R^2 = {best_by_bic["r2"]:.4f}')
print(f'BIC = {best_by_bic["bic"]:.4f}')
print('Объяснение выбора: BIC штрафует за сложность модели, поэтому выбрана модель с наименьшим BIC, '
      'обеспечивающая компромисс между точностью и количеством признаков.')

ks = [r['k'] for r in results]
rss_vals = [r['rss'] for r in results]
bic_vals = [r['bic'] for r in results]

plt.figure(figsize=(10,4))
plt.subplot(1,2,1)
plt.plot(ks, rss_vals, marker='o')
plt.xlabel('Количество признаков')
plt.ylabel('RSS')
plt.title('RSS лучших моделей')
plt.grid(True)

plt.subplot(1,2,2)
plt.plot(ks, bic_vals, marker='o', color='red')
plt.xlabel('Количество признаков')
plt.ylabel('BIC')
plt.title('BIC лучших моделей')
plt.grid(True)

plt.tight_layout()
plt.savefig(os.path.join(images_path, 'reglab_feature_selection.png'))
plt.show()
