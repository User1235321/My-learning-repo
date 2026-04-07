import os
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from sklearn.svm import SVR
from sklearn.metrics import mean_squared_error

data_path = os.path.join('..', 'data', 'svmdata6.txt')
images_path = os.path.join('..', 'images')
os.makedirs(images_path, exist_ok=True)

df = pd.read_csv(data_path, sep='\t', skiprows=1, names=['idx', 'X', 'Y'])
X = df['X'].values.reshape(-1, 1)
y = df['Y'].values

print('Предварительная информация:')
print('Первые 5 строк данных:')
print(df.head())
print(f'\nРазмер данных: X shape {X.shape}, y shape {y.shape}')
print(f'Диапазон X: [{X.min()}, {X.max()}]')
print(f'Диапазон y: [{y.min()}, {y.max()}]')

epsilons = np.linspace(0, 0.5, 50)
mse_scores = []

for eps in epsilons:
    model = SVR(kernel='rbf', C=1, epsilon=eps)
    model.fit(X, y)
    y_pred = model.predict(X)
    mse = mean_squared_error(y, y_pred)
    mse_scores.append(mse)
    print(f'epsilon = {eps:.3f}, MSE = {mse:.6f}')

plt.figure(figsize=(8,5))
plt.plot(epsilons, mse_scores, marker='o', linestyle='-', markersize=3)
plt.xlabel('epsilon')
plt.ylabel('MSE на обучающей выборке')
plt.title('Зависимость MSE от epsilon в SVR (C=1, ядро rbf)')
plt.grid(True)
plt.tight_layout()
plt.savefig(os.path.join(images_path, 'svmdata6_epsilon_mse.png'))
plt.show()

print('\nОкончательная информация:')
print('При малых epsilon модель точно подгоняется под данные, MSE близка к 0.')
print('С ростом epsilon увеличивается допустимая погрешность, модель становится проще,')
print('и ошибка на обучающей выборке монотонно возрастает.')
