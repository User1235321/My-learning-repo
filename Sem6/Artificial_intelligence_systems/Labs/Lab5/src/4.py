import os
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from sklearn.linear_model import LinearRegression, Ridge
from sklearn.model_selection import train_test_split
from sklearn.metrics import mean_squared_error

data_path = os.path.join('..', 'data', 'longley.csv')
images_path = os.path.join('..', 'images')
os.makedirs(images_path, exist_ok=True)

df = pd.read_csv(data_path)
print('Предварительная информация:')
print('Первые 5 строк данных:')
print(df.head())
print('\nОписательные статистики:')
print(df.describe())

df = df.drop(columns=['Population'])
X = df.drop(columns=['Employed'])
y = df['Employed']

X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.5, random_state=42)

lr = LinearRegression()
lr.fit(X_train, y_train)
y_train_pred_lr = lr.predict(X_train)
y_test_pred_lr = lr.predict(X_test)
mse_train_lr = mean_squared_error(y_train, y_train_pred_lr)
mse_test_lr = mean_squared_error(y_test, y_test_pred_lr)

print('\n--- Линейная регрессия (OLS) ---')
print(f'MSE на обучающей выборке: {mse_train_lr:.4f}')
print(f'MSE на тестовой выборке: {mse_test_lr:.4f}')

lambdas = np.logspace(-2, 4, 50)
mse_train_ridge = []
mse_test_ridge = []

for alpha in lambdas:
    ridge = Ridge(alpha=alpha)
    ridge.fit(X_train, y_train)
    y_train_pred = ridge.predict(X_train)
    y_test_pred = ridge.predict(X_test)
    mse_train_ridge.append(mean_squared_error(y_train, y_train_pred))
    mse_test_ridge.append(mean_squared_error(y_test, y_test_pred))

plt.figure(figsize=(10,6))
plt.semilogx(lambdas, mse_train_ridge, label='Ridge обучающая', color='blue')
plt.semilogx(lambdas, mse_test_ridge, label='Ridge тестовая', color='red')
plt.axhline(y=mse_train_lr, color='blue', linestyle='--', label='OLS обучающая')
plt.axhline(y=mse_test_lr, color='red', linestyle='--', label='OLS тестовая')
plt.xlabel('λ (alpha)')
plt.ylabel('MSE')
plt.title('Зависимость ошибки от λ для гребневой регрессии')
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig(os.path.join(images_path, 'longley_ridge.png'))
plt.show()

print('\nОкончательная информация:')
print('При малых λ гребневая регрессия близка к OLS. С ростом λ ошибка на обучающей выборке растёт (смещение увеличивается),')
print('но ошибка на тестовой может сначала уменьшаться за счёт снижения переобучения, а затем также расти.')
print('Оптимальное λ находится в области минимума тестовой ошибки.')
