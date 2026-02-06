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

# Метод наискорейшего спуска
def steepest_descent(x0, max_iter=5):
    x = x0.copy()
    trajectory = [x.copy()]
    
    for i in range(max_iter):
        g = grad_f(x)
        numerator = g.T @ g
        denominator = g.T @ H @ g
        alpha = numerator / denominator
        x_new = x - alpha * g
        trajectory.append(x_new.copy())
        x = x_new
    
    return np.array(trajectory)

# Метод Ньютона
def newton_method(x0, max_iter=5):
    x = x0.copy()
    trajectory = [x.copy()]
    H_inv = np.linalg.inv(H)
    
    for i in range(max_iter):
        g = grad_f(x)
        delta_x = -H_inv @ g
        x_new = x + delta_x
        trajectory.append(x_new.copy())
        x = x_new
    
    return np.array(trajectory)

# Вычисление траекторий
x0 = np.array([0.0, 0.0])
trajectory_sd = steepest_descent(x0, 5)
trajectory_newton = newton_method(x0, 5)

# Визуализация
x1 = np.linspace(-2, 2, 1000)
x2 = np.linspace(-2, 2, 1000)
X1, X2 = np.meshgrid(x1, x2)
Z = f([X1, X2])

plt.figure(figsize=(14, 6))

# Метод наискорейшего спуска
plt.subplot(1, 2, 1)
levels = np.linspace(np.min(Z), np.max(Z), 500)
plt.contour(X1, X2, Z, levels=levels, colors='blue', alpha=0.4, linewidths=0.3)
plt.plot(trajectory_sd[:, 0], trajectory_sd[:, 1], 'ro-', markersize=6, linewidth=2, label='Траектория спуска')

# Подписи координат для метода наискорейшего спуска
for i, (x, y) in enumerate(trajectory_sd):
    plt.annotate(f'({x:.3f}, {y:.3f})', (x, y), 
                xytext=(5, 5), textcoords='offset points',
                fontsize=8, alpha=0.8,
                bbox=dict(boxstyle='round,pad=0.2', facecolor='red', alpha=0.2))

plt.plot(x_min[0], x_min[1], 'g*', markersize=12, label=f'Минимум ({x_min[0]:.3f}, {x_min[1]:.3f})')
plt.xlabel('x₁')
plt.ylabel('x₂')
plt.title('Метод наискорейшего спуска (5 итераций)')
plt.grid(True, alpha=0.3)
plt.axis('equal')
plt.xlim(-2, 2)
plt.ylim(-2, 2)
plt.legend()

# Метод Ньютона
plt.subplot(1, 2, 2)
plt.contour(X1, X2, Z, levels=levels, colors='blue', alpha=0.4, linewidths=0.3)
plt.plot(trajectory_newton[:, 0], trajectory_newton[:, 1], 'mo-', markersize=6, linewidth=2, label='Траектория Ньютона')
plt.plot(x_min[0], x_min[1], 'g*', markersize=12, label=f'Минимум ({x_min[0]:.3f}, {x_min[1]:.3f})')
plt.xlabel('x₁')
plt.ylabel('x₂')
plt.title('Метод Ньютона (5 итераций)')
plt.grid(True, alpha=0.3)
plt.axis('equal')
plt.xlim(-2, 2)
plt.ylim(-2, 2)
plt.legend()

plt.tight_layout()
plt.show()
