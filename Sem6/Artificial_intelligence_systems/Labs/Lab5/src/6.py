import os
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import statsmodels.api as sm

data_path = os.path.join('..', 'data', 'JohnsonJohnson.csv')
images_path = os.path.join('..', 'images')
os.makedirs(images_path, exist_ok=True)

df = pd.read_csv(data_path)
print('Предварительная информация:')
print('Первые 5 строк данных:')
print(df.head())
print('\nОписательные статистики:')
print(df.describe())

df['Year'] = df['index'].str[:4].astype(int)
df['Quarter'] = df['index'].str[5:7]

quarters = ['Q1', 'Q2', 'Q3', 'Q4']
plt.figure(figsize=(12,6))
for q in quarters:
    q_data = df[df['Quarter'] == q]
    plt.plot(q_data['Year'], q_data['value'], marker='o', label=q)
plt.xlabel('Год')
plt.ylabel('Прибыль')
plt.title('Поквартальная прибыль Johnson & Johnson')
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig(os.path.join(images_path, 'JohnsonJohnson_series.png'))
plt.show()

print('\n--- Линейная регрессия для каждого квартала отдельно ---')
slopes = {}
models_q = {}
for q in quarters:
    q_data = df[df['Quarter'] == q]
    X = sm.add_constant(q_data['Year'])
    y = q_data['value']
    model = sm.OLS(y, X).fit()
    models_q[q] = model
    slopes[q] = model.params['Year']
    print(f'\nКвартал {q}:')
    print(model.summary().tables[1])
    print(f'R^2 = {model.rsquared:.4f}')
    print(f'Наклон (динамика) = {model.params["Year"]:.4f}')

print('\n--- Линейная регрессия для всех кварталов вместе ---')
X_all = sm.add_constant(df['Year'])
y_all = df['value']
model_all = sm.OLS(y_all, X_all).fit()
print(model_all.summary().tables[1])
print(f'R^2 = {model_all.rsquared:.4f}')
print(f'Наклон (динамика) = {model_all.params["Year"]:.4f}')

plt.figure(figsize=(12,6))
for q in quarters:
    q_data = df[df['Quarter'] == q]
    plt.scatter(q_data['Year'], q_data['value'], label=f'{q} данные')
    year_range = np.linspace(q_data['Year'].min(), 2016, 100)
    X_pred = sm.add_constant(year_range)
    y_pred = models_q[q].predict(X_pred)
    plt.plot(year_range, y_pred, linestyle='--', label=f'{q} тренд')
plt.xlabel('Год')
plt.ylabel('Прибыль')
plt.title('Регрессионные линии по кварталам')
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig(os.path.join(images_path, 'JohnsonJohnson_regressions.png'))
plt.show()

max_q = max(slopes, key=slopes.get)
min_q = min(slopes, key=slopes.get)
print(f'\nОкончательная информация:')
print(f'Наибольшую динамику имеет квартал {max_q} с наклоном {slopes[max_q]:.4f}')
print(f'Наименьшую динамику имеет квартал {min_q} с наклоном {slopes[min_q]:.4f}')

print('\nПрогноз на 2016 год:')
year_2016 = 2016
pred_2016 = {}
for q in quarters:
    X_pred = np.array([[1, year_2016]])
    pred = models_q[q].predict(X_pred)[0]
    pred_2016[q] = pred
    print(f'{q}: {pred:.4f}')
mean_pred = np.mean(list(pred_2016.values()))
print(f'Средняя прибыль в 2016 году: {mean_pred:.4f}')
