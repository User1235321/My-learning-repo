import os
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import statsmodels.api as sm

data_path = os.path.join('..', 'data', 'reglab1.txt')
images_path = os.path.join('..', 'images')
os.makedirs(images_path, exist_ok=True)

df = pd.read_csv(data_path, sep=r'\s+')
print('Предварительная информация:')
print('Первые 5 строк данных:')
print(df.head())
print('\nОписательные статистики:')
print(df.describe())

variables = ['z', 'x', 'y']
models = {}
best_model = None
best_r2 = -np.inf

for dep_var in variables:
    indep_vars = [v for v in variables if v != dep_var]
    X = df[indep_vars]
    X = sm.add_constant(X)
    y = df[dep_var]
    model = sm.OLS(y, X).fit()
    models[dep_var] = model
    
    print(f'\n--- Модель с зависимой переменной "{dep_var}" ---')
    print(model.summary().tables[1])
    print(f'R^2 = {model.rsquared:.4f}')
    print(f'p-value коэффициентов: {model.pvalues.values}')
    
    y_pred = model.predict(X)
    plt.figure(figsize=(6,6))
    plt.scatter(y, y_pred, alpha=0.7)
    min_val = min(y.min(), y_pred.min())
    max_val = max(y.max(), y_pred.max())
    plt.plot([min_val, max_val], [min_val, max_val], 'r--')
    plt.xlabel('Фактические значения')
    plt.ylabel('Предсказанные значения')
    plt.title(f'Зависимая переменная: {dep_var}')
    plt.tight_layout()
    plt.savefig(os.path.join(images_path, f'reglab1_model_{dep_var}.png'))
    plt.show()
    plt.close()
    
    if model.rsquared > best_r2:
        best_r2 = model.rsquared
        best_model = dep_var

print('\nОкончательная информация:')
print(f'Наиболее подходящая модель — с зависимой переменной "{best_model}"')
print(f'Её R^2 = {best_r2:.4f}')
