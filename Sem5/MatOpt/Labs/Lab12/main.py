import numpy as np
import matplotlib.pyplot as plt

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

# Функция релаксации метода Левенберга
def relaxation_function_Levenberg(lambd, alpha):
    """Функция релаксации метода Левенберга"""
    return lambd / (lambd + alpha)

# Реализация метода Левенберга
def levenberg_method(x0, alpha=1000, max_iter=3):
    """Метод Левенберга"""
    x = x0.copy()
    trajectory = [x.copy()]
    
    for i in range(max_iter):
        g = grad_f(x)
        
        # Матрица для метода Левенберга: H + αI
        H_lev = H + alpha * np.eye(2)
        
        # Решение системы (H + αI)*direction = g
        direction = np.linalg.solve(H_lev, g)
        
        # Обновление точки
        x_new = x - direction
        trajectory.append(x_new.copy())
        x = x_new
    
    return np.array(trajectory)

# Вычисление траектории метода Левенберга
x0 = np.array([0.0, 0.0])
trajectory_lev = levenberg_method(x0, alpha=1000, max_iter=3)

print("Точка минимума:", x_min)
print("\nСобственные значения матрицы Гессе:")
eigenvals = np.linalg.eigvals(H)
for i, eigval in enumerate(eigenvals):
    print(f"λ{i+1} = {eigval:.4f}")

print("\nТраектория метода Левенберга:")
for i, point in enumerate(trajectory_lev):
    print(f"Шаг {i}: ({point[0]:.6f}, {point[1]:.6f})")

# Создаем фигуру с двумя субплогами
fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(16, 6))

# Первый субплот: траектория метода Левенберга
x1_range = np.linspace(-2, 2, 400)
x2_range = np.linspace(-2, 2, 400)
X1, X2 = np.meshgrid(x1_range, x2_range)
Z = f([X1, X2])

levels = np.linspace(np.min(Z), np.max(Z), 30)
contour = ax1.contour(X1, X2, Z, levels=levels, colors='blue', alpha=0.6, linewidths=0.8)
ax1.clabel(contour, inline=True, fontsize=8)

# Отображение траектории
ax1.plot(trajectory_lev[:, 0], trajectory_lev[:, 1], 'go-', markersize=8, linewidth=2, label='Метод Левенберга')

# Подписи координат
for i, (x, y) in enumerate(trajectory_lev):
    ax1.annotate(f'Шаг {i}\n({x:.3f}, {y:.3f})', (x, y), 
                xytext=(10, 10), textcoords='offset points',
                fontsize=9, alpha=0.9,
                bbox=dict(boxstyle='round,pad=0.3', facecolor='lightgreen', alpha=0.7))

ax1.plot(x_min[0], x_min[1], 'b*', markersize=15, label=f'Минимум ({x_min[0]:.3f}, {x_min[1]:.3f})')
ax1.set_xlabel('x₁')
ax1.set_ylabel('x₂')
ax1.set_title('Метод Левенберга (3 итерации)')
ax1.grid(True, alpha=0.3)
ax1.axis('equal')
ax1.set_xlim(-2, 2)
ax1.set_ylim(-2, 2)
ax1.legend()

# Второй субплот: график функции релаксации метода Левенберга
lambda_values = np.linspace(0, 2500, 500)

# Область релаксационности
ax2.axhspan(0, 1, alpha=0.2, color='green', label='Область релаксационности')

# Функции релаксации для различных значений параметра α
alpha_values = [100, 500, 1000, 2000]
colors = ['red', 'blue', 'orange', 'purple']

for i, alpha in enumerate(alpha_values):
    R_lev = [relaxation_function_Levenberg(l, alpha) for l in lambda_values]
    ax2.plot(lambda_values, R_lev, color=colors[i], linewidth=2, label=f'МЛ, α={alpha}')

# Вертикальные линии для собственных значений
for i, eigval in enumerate(eigenvals):
    ax2.axvline(x=eigval, color='red', linestyle='--', linewidth=2,
                label=f'λ{i+1} = {eigval:.1f}')

# Горизонтальная линия R=1
ax2.axhline(y=1, color='black', linestyle='-', linewidth=1, alpha=0.5)

ax2.set_xlabel('λ')
ax2.set_ylabel('R(λ)')
ax2.set_title('Функция релаксации метода Левенберга')
ax2.grid(True, alpha=0.3)
ax2.set_ylim(-0.1, 1.5)
ax2.set_xlim(0, 2500)
ax2.legend()

plt.tight_layout()
plt.show()

# Анализ сходимости и работоспособности метода
print("\nАнализ метода Левенберга:")
print("Функция релаксации: R(λ) = λ/(λ + α)")
print("Условие релаксационности: |R(λ)| < 1 для всех λ > 0")
print("Преимущества:")
print("- Сочетает устойчивость градиентного спуска и скорость метода Ньютона")
print("- Автоматически адаптируется к овражной структуре функции")
print("- Устойчив к плохой обусловленности матрицы Гессе")
print("Недостатки:")
print("- Требует подбора параметра α")
print("- Вычислительная сложность сравнима с методом Ньютона")
print("\nВывод: Метод Левенберга эффективен для задач с плохо обусловленной матрицей Гессе")
