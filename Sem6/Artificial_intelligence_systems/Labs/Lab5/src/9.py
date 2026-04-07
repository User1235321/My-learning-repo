import os
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from sklearn.model_selection import train_test_split, GridSearchCV
from sklearn.preprocessing import StandardScaler
from sklearn.linear_model import LinearRegression
from sklearn.tree import DecisionTreeRegressor
from sklearn.svm import SVR
from sklearn.metrics import mean_squared_error, r2_score

data_path = os.path.join('..', 'data', 'nsw74psid1.csv')
images_path = os.path.join('..', 'images')
os.makedirs(images_path, exist_ok=True)

df = pd.read_csv(data_path)
print('Предварительная информация:')
print('Первые 5 строк данных:')
print(df.head())
print('\nОписательные статистики:')
print(df.describe())
print('\nПропуски:')
print(df.isnull().sum())

X = df.drop(columns=['re78'])
y = df['re78']

X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)

scaler = StandardScaler()
X_train_scaled = scaler.fit_transform(X_train)
X_test_scaled = scaler.transform(X_test)

lr = LinearRegression()
lr.fit(X_train_scaled, y_train)
y_pred_lr = lr.predict(X_test_scaled)
mse_lr = mean_squared_error(y_test, y_pred_lr)
r2_lr = r2_score(y_test, y_pred_lr)

dt = DecisionTreeRegressor(random_state=42)
param_grid_dt = {'max_depth': [3, 5, 7, 10, None], 'min_samples_split': [2, 5, 10]}
grid_dt = GridSearchCV(dt, param_grid_dt, cv=5, scoring='r2')
grid_dt.fit(X_train, y_train)
best_dt = grid_dt.best_estimator_
y_pred_dt = best_dt.predict(X_test)
mse_dt = mean_squared_error(y_test, y_pred_dt)
r2_dt = r2_score(y_test, y_pred_dt)

svm = SVR(kernel='rbf')
param_grid_svm = {'C': [0.1, 1, 10, 100], 'gamma': ['scale', 'auto', 0.1, 1]}
grid_svm = GridSearchCV(svm, param_grid_svm, cv=5, scoring='r2')
grid_svm.fit(X_train_scaled, y_train)
best_svm = grid_svm.best_estimator_
y_pred_svm = best_svm.predict(X_test_scaled)
mse_svm = mean_squared_error(y_test, y_pred_svm)
r2_svm = r2_score(y_test, y_pred_svm)

print('\n--- Результаты на тестовой выборке ---')
print(f'Линейная регрессия: MSE = {mse_lr:.2f}, R^2 = {r2_lr:.4f}')
print(f'Дерево решений (лучшие параметры {grid_dt.best_params_}): MSE = {mse_dt:.2f}, R^2 = {r2_dt:.4f}')
print(f'SVM (лучшие параметры {grid_svm.best_params_}): MSE = {mse_svm:.2f}, R^2 = {r2_svm:.4f}')

plt.figure(figsize=(15,4))
plt.subplot(1,3,1)
plt.scatter(y_test, y_pred_lr, alpha=0.6)
plt.plot([y_test.min(), y_test.max()], [y_test.min(), y_test.max()], 'r--')
plt.xlabel('Фактические')
plt.ylabel('Предсказанные')
plt.title(f'Линейная регрессия (R^2={r2_lr:.3f})')

plt.subplot(1,3,2)
plt.scatter(y_test, y_pred_dt, alpha=0.6)
plt.plot([y_test.min(), y_test.max()], [y_test.min(), y_test.max()], 'r--')
plt.xlabel('Фактические')
plt.ylabel('Предсказанные')
plt.title(f'Дерево решений (R^2={r2_dt:.3f})')

plt.subplot(1,3,3)
plt.scatter(y_test, y_pred_svm, alpha=0.6)
plt.plot([y_test.min(), y_test.max()], [y_test.min(), y_test.max()], 'r--')
plt.xlabel('Фактические')
plt.ylabel('Предсказанные')
plt.title(f'SVM (R^2={r2_svm:.3f})')

plt.tight_layout()
plt.savefig(os.path.join(images_path, 'nsw74psid1_comparison.png'))
plt.show()

best_model = max([(r2_lr, 'Линейная регрессия'), (r2_dt, 'Дерево решений'), (r2_svm, 'SVM')])
print(f'\nОкончательная информация:')
print(f'Лучшая модель — {best_model[1]} с R^2 = {best_model[0]:.4f}')
print('Объяснение: модель с наибольшим R^2 на тестовой выборке показывает лучшее качество предсказания.')
print('Дополнительно можно учитывать интерпретируемость и сложность модели.')
