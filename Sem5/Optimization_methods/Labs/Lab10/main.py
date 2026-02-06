import numpy as np
import matplotlib.pyplot as plt
from scipy.linalg import expm

# Коэффициенты функции
a = 605.45
b = 326.7
c = 51.05
d = 11.8
e = 11.8

# Матрица Гессе
H = np.array([[2*a, b],
              [b, 2*c]])

def f(x):
    return a*x[0]**2 + b*x[0]*x[1] + c*x[1]**2 + d*x[0] + e*x[1]

def grad_f(x):
    return np.array([2*a*x[0] + b*x[1] + d,
                     b*x[0] + 2*c*x[1] + e])

# Точка минимума
A = np.array([[2*a, b],
              [b, 2*c]])
b_vec = np.array([-d, -e])
x_min = np.linalg.solve(A, b_vec)

# Метод экспоненциальной релаксации (МЭР)
def exponential_relaxation_method(x0, h=1.0, max_iter=3):
    x = x0.copy()
    trajectory = [x.copy()]
    
    # Вычисляем матричную экспоненту
    H_h = H * h
    exp_minus_Hh = expm(-H_h)
    I = np.eye(2)
    
    # Формула МЭР: x_{k+1} = x_k - [I - exp(-Hh)] * H^{-1} * grad_f(x_k)
    H_inv = np.linalg.inv(H)
    M = I - exp_minus_Hh
    
    for i in range(max_iter):
        g = grad_f(x)
        direction = M @ H_inv @ g
        x_new = x - direction
        trajectory.append(x_new.copy())
        x = x_new
    
    return np.array(trajectory)

# Функция релаксации R(λ) для МЭР
def relaxation_function_MER(lambd, h):
    return 1 - np.exp(-lambd * h)

# Вычисление траектории МЭР
x0 = np.array([0.0, 0.0])
trajectory_er = exponential_relaxation_method(x0, h=0.5, max_iter=3)

print("Траектория метода экспоненциальной релаксации:")
for i, point in enumerate(trajectory_er):
    print(f"Шаг {i}: ({point[0]:.6f}, {point[1]:.6f})")

# Собственные значения матрицы Гессе
eigenvals = np.linalg.eigvals(H)
print("\nСобственные значения матрицы Гессе:")
for i, eigval in enumerate(eigenvals):
    print(f"λ{i+1} = {eigval:.4f}")

# Создаем фигуру с двумя субплогами
fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(16, 6))

# Первый субплот: траектория МЭР
x1_range = np.linspace(-2, 2, 1000)
x2_range = np.linspace(-2, 2, 1000)
X1, X2 = np.meshgrid(x1_range, x2_range)
Z = f([X1, X2])

levels = np.linspace(np.min(Z), np.max(Z), 50)  # Уменьшил количество уровней для лучшей видимости
contour = ax1.contour(X1, X2, Z, levels=levels, colors='blue', alpha=0.6, linewidths=0.8)
ax1.clabel(contour, inline=True, fontsize=8)  # Добавил подписи уровней

ax1.plot(trajectory_er[:, 0], trajectory_er[:, 1], 'ro-', markersize=8, linewidth=2, label='Траектория МЭР')

# Подписи координат
for i, (x, y) in enumerate(trajectory_er):
    ax1.annotate(f'Шаг {i}\n({x:.3f}, {y:.3f})', (x, y), 
                xytext=(10, 10), textcoords='offset points',
                fontsize=9, alpha=0.9,
                bbox=dict(boxstyle='round,pad=0.3', facecolor='yellow', alpha=0.7))

ax1.plot(x_min[0], x_min[1], 'g*', markersize=15, label=f'Минимум ({x_min[0]:.3f}, {x_min[1]:.3f})')
ax1.set_xlabel('x₁')
ax1.set_ylabel('x₂')
ax1.set_title('Метод экспоненциальной релаксации (3 итерации)')
ax1.grid(True, alpha=0.3)
ax1.axis('equal')
ax1.set_xlim(-2, 2)
ax1.set_ylim(-2, 2)
ax1.legend()

# Второй субплот: график функции релаксации МЭР
lambda_values = np.linspace(0, 2500, 500)  # Увеличил диапазон для лучшего обзора

# Параметры для графиков
h_values = [0.001, 0.005, 0.01, 0.05]  # Подобрал значения h для лучшей видимости

# Область релаксационности (где R(λ) < 1)
ax2.axhspan(0, 1, alpha=0.2, color='green', label='Область релаксационности')

# Функция релаксации для МЭР с разными h
colors = ['red', 'blue', 'orange', 'purple']
for i, h in enumerate(h_values):
    R_MER = relaxation_function_MER(lambda_values, h)
    ax2.plot(lambda_values, R_MER, color=colors[i], linewidth=2, label=f'МЭР, h={h}')

# Вертикальные линии для собственных значений
for i, eigval in enumerate(eigenvals):
    ax2.axvline(x=eigval, color='red', linestyle='--', linewidth=2,
                label=f'λ{i+1} = {eigval:.1f}')

# Горизонтальная линия R=1 (граница релаксационности)
ax2.axhline(y=1, color='black', linestyle='-', linewidth=1, alpha=0.5)

ax2.set_xlabel('λ')
ax2.set_ylabel('R(λ)')
ax2.set_title('Функция релаксации метода экспоненциальной релаксации')
ax2.grid(True, alpha=0.3)
ax2.set_ylim(-0.1, 1.5)
ax2.set_xlim(0, 2500)
ax2.legend()

plt.tight_layout()
plt.show()
