import os
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import statsmodels.api as sm

data_path = os.path.join('..', 'data', 'cars.csv')
images_path = os.path.join('..', 'images')
os.makedirs(images_path, exist_ok=True)

df = pd.read_csv(data_path)
print('Предварительная информация:')
print('Первые 5 строк данных:')
print(df.head())
print('\nОписательные статистики:')
print(df.describe())

X = df['speed']
y = df['dist']
X_const = sm.add_constant(X)
model = sm.OLS(y, X_const).fit()

print('\n--- Регрессионная модель ---')
print(model.summary().tables[1])
print(f'R^2 = {model.rsquared:.4f}')
print(f'Коэффициенты: {model.params.values}')

speed_new = 40
X_new = np.array([[1, speed_new]])
pred = model.predict(X_new)[0]
print(f'\nПредсказанный тормозной путь при скорости 40 миль/ч: {pred:.2f} футов')

plt.figure(figsize=(8,6))
plt.scatter(df['speed'], df['dist'], alpha=0.7, label='Данные')
speed_range = np.linspace(df['speed'].min(), df['speed'].max(), 100)
X_range = sm.add_constant(speed_range)
plt.plot(speed_range, model.predict(X_range), 'r-', label='Линейная регрессия')
plt.xlabel('Скорость (миль/ч)')
plt.ylabel('Тормозной путь (футы)')
plt.title('Зависимость тормозного пути от скорости')
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig(os.path.join(images_path, 'cars_regression.png'))
plt.show()
