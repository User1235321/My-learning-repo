import os
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import statsmodels.api as sm

data_path = os.path.join('..', 'data', 'cygage.txt')
images_path = os.path.join('..', 'images')
os.makedirs(images_path, exist_ok=True)

df = pd.read_csv(data_path, sep=r'\s+')
print('Предварительная информация:')
print('Первые 5 строк данных:')
print(df.head())
print('\nОписательные статистики:')
print(df.describe())

X = df['Depth']
y = df['calAge']
weights = df['Weight']

X_const = sm.add_constant(X)

model_ols = sm.OLS(y, X_const).fit()
model_wls = sm.WLS(y, X_const, weights=weights).fit()

print('\n--- Модель без весов (OLS) ---')
print(model_ols.summary().tables[1])
print(f'R^2 = {model_ols.rsquared:.4f}')
print(f'Коэффициенты: {model_ols.params.values}')
print(f'Стандартные ошибки: {model_ols.bse.values}')

print('\n--- Модель с весами (WLS) ---')
print(model_wls.summary().tables[1])
print(f'R^2 = {model_wls.rsquared:.4f}')
print(f'Коэффициенты: {model_wls.params.values}')
print(f'Стандартные ошибки: {model_wls.bse.values}')

plt.figure(figsize=(10,6))
plt.scatter(df['Depth'], df['calAge'], s=df['Weight']*100, alpha=0.6, label='Данные (размер = вес)')
Depth_range = np.linspace(df['Depth'].min(), df['Depth'].max(), 100)
X_pred = sm.add_constant(Depth_range)
plt.plot(Depth_range, model_ols.predict(X_pred), 'b-', label='OLS (без весов)')
plt.plot(Depth_range, model_wls.predict(X_pred), 'r--', label='WLS (с весами)')
plt.xlabel('Глубина')
plt.ylabel('Возраст')
plt.title('Зависимость возраста от глубины')
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig(os.path.join(images_path, 'cygage_regression.png'))
plt.show()

print('\nОкончательная информация:')
print('Модель с весами (WLS) учитывает разную точность наблюдений, что даёт более надёжные оценки.')
print('Коэффициенты WLS более устойчивы к наблюдениям с малым весом.')
