import os
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import statsmodels.api as sm
from statsmodels.formula.api import ols

data_path = os.path.join('..', 'data', 'eustock.csv')
images_path = os.path.join('..', 'images')
os.makedirs(images_path, exist_ok=True)

df = pd.read_csv(data_path)
print('Предварительная информация:')
print('Первые 5 строк данных:')
print(df.head())
print('\nОписательные статистики:')
print(df.describe())

days = np.arange(1, len(df) + 1)
df['Day'] = days

plt.figure(figsize=(12,6))
for col in ['DAX', 'SMI', 'CAC', 'FTSE']:
    plt.plot(df['Day'], df[col], label=col)
plt.xlabel('День')
plt.ylabel('Котировки')
plt.title('Изменение котировок во времени')
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig(os.path.join(images_path, 'eustock_curves.png'))
plt.show()

print('\n--- Линейная регрессия для каждой биржи отдельно ---')
slopes = {}
for col in ['DAX', 'SMI', 'CAC', 'FTSE']:
    X = sm.add_constant(df['Day'])
    y = df[col]
    model = sm.OLS(y, X).fit()
    slopes[col] = model.params['Day']
    print(f'\n{col}:')
    print(model.summary().tables[1])
    print(f'R^2 = {model.rsquared:.4f}')
    print(f'Наклон (динамика) = {model.params["Day"]:.4f}')

print('\n--- Линейная регрессия для всех бирж вместе (с фиктивными переменными) ---')
df_long = pd.melt(df, id_vars=['Day'], value_vars=['DAX','SMI','CAC','FTSE'],
                  var_name='Index', value_name='Value')
model_pooled = ols('Value ~ Day * Index', data=df_long).fit()
print(model_pooled.summary().tables[1])
print('Коэффициенты наклона для каждой биржи (из взаимодействия):')
for idx in ['DAX', 'SMI', 'CAC', 'FTSE']:
    if idx == 'CAC':
        slope = model_pooled.params['Day']
    else:
        slope = model_pooled.params['Day'] + model_pooled.params[f'Day:Index[T.{idx}]']
    print(f'{idx}: {slope:.4f}')

best = max(slopes, key=slopes.get)
print(f'\nОкончательная информация:')
print(f'Наибольшую динамику имеет биржа {best} с наклоном {slopes[best]:.4f}')
