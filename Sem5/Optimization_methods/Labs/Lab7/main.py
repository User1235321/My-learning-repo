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

# Собственные значения и векторы
eigenvalues, eigenvectors = np.linalg.eig(H)
lambda1, lambda2 = eigenvalues
u1, u2 = eigenvectors[:, 0], eigenvectors[:, 1]

print("Собственные значения:")
print(f"λ₁ = {lambda1:.4f}")
print(f"λ₂ = {lambda2:.4f}")
print("\nСобственные векторы (нормированные):")
print(f"u₁ = [{u1[0]:.4f}, {u1[1]:.4f}]")
print(f"u₂ = [{u2[0]:.4f}, {u2[1]:.4f}]")

# Точка минимума (решение системы)
A = np.array([[2*a, b],
              [b, 2*c]])
b_vec = np.array([-d, -e])
x_min = np.linalg.solve(A, b_vec)

print(f"\nТочка минимума:")
print(f"x₁ = {x_min[0]:.6f}")
print(f"x₂ = {x_min[1]:.6f}")

# Построение линий уровня
def f(x1, x2):
    return (a * x1**2 + b * x1 * x2 + c * x2**2 + 
            d * x1 + e * x2)

# Градиент функции
def grad_f(x1, x2):
    return np.array([2*a*x1 + b*x2 + d, b*x1 + 2*c*x2 + e])

# Метод обобщенного покоординатного спуска
def generalized_coordinate_descent(x_start, eigenvectors, eigenvalues, max_iter=100, tol=1e-6):
    x = x_start.copy()
    trajectory = [x.copy()]
    
    for k in range(max_iter):
        x_prev = x.copy()
        
        # Последовательный спуск по собственным направлениям
        for i in range(len(eigenvectors)):
            # Текущее направление (собственный вектор)
            direction = eigenvectors[:, i]
            # Оптимальный шаг вдоль направления
            step = -np.dot(grad_f(x[0], x[1]), direction) / eigenvalues[i]
            x = x + step * direction
            trajectory.append(x.copy())
        
        # Проверка условия остановки
        if np.linalg.norm(x - x_prev) < tol:
            break
            
    return np.array(trajectory)

# Начальная точка
x_start = np.array([0.0, 0.0])

# Выполнение спуска
trajectory = generalized_coordinate_descent(x_start, eigenvectors, eigenvalues)

print(f"\nМетод обобщенного покоординатного спуска:")
print(f"Начальная точка: ({x_start[0]}, {x_start[1]})")
print(f"Количество итераций: {len(trajectory) // 2}")
print(f"Конечная точка: ({trajectory[-1][0]:.6f}, {trajectory[-1][1]:.6f})")
print(f"Значение функции в конечной точке: {f(trajectory[-1][0], trajectory[-1][1]):.6f}")

# Визуализация траектории спуска с увеличенным количеством линий уровня
x1_range = 2.0  # Диапазон от -2 до 2
x2_range = 2.0  # Диапазон от -2 до 2
x1 = np.linspace(-2, 2, 1000)  # Увеличим количество точек для более гладких линий
x2 = np.linspace(-2, 2, 1000)
X1, X2 = np.meshgrid(x1, x2)
Z = f(X1, X2)

# Значение в точке минимума
f_min = f(x_min[0], x_min[1])

plt.figure(figsize=(12, 10))

# Увеличиваем количество линий уровня в 100 раз
# Вместо 25 уровней используем 100, и вместо 80 используем 200
levels = np.linspace(f_min, f_min + 1000, 100)  # Увеличиваем диапазон значений для учета большего области
contour = plt.contour(X1, X2, Z, levels=levels, colors='blue', alpha=0.6, linewidths=0.8)
plt.clabel(contour, inline=True, fontsize=6)  # Уменьшаем шрифт для большего количества меток

# Еще более густые линии для плавности (в 2.5 раза больше чем было)
levels_dense = np.linspace(f_min, f_min + 1000, 200)
plt.contour(X1, X2, Z, levels=levels_dense, colors='blue', alpha=0.3, linewidths=0.4)

# Заполненный контур
contourf = plt.contourf(X1, X2, Z, levels=200, alpha=0.5, cmap='viridis')

# Траектория спуска
plt.plot(trajectory[:, 0], trajectory[:, 1], 'ro-', markersize=4, linewidth=1.5, label='Траектория спуска')
plt.plot(x_start[0], x_start[1], 'go', markersize=8, label='Начальная точка')
plt.plot(x_min[0], x_min[1], 'r*', markersize=12, label='Точка минимума')

# Собственные векторы
scale = 0.1
plt.arrow(x_min[0], x_min[1], u1[0]*scale, u1[1]*scale, 
          head_width=0.02, head_length=0.02, fc='red', ec='red', linewidth=2,
          label=f'u₁ (λ={lambda1:.1f})')
plt.arrow(x_min[0], x_min[1], u2[0]*scale, u2[1]*scale, 
          head_width=0.02, head_length=0.02, fc='green', ec='green', linewidth=2,
          label=f'u₂ (λ={lambda2:.1f})')

plt.xlabel('x₁', fontsize=14)
plt.ylabel('x₂', fontsize=14)
plt.title('Метод обобщенного покоординатного спуска\n(спуск вдоль собственных векторов)', fontsize=16)
plt.grid(True, alpha=0.3)
plt.axis('equal')
plt.xlim(-2, 2)
plt.ylim(-2, 2)
plt.legend(fontsize=12)

# Цветовая шкала
cbar = plt.colorbar(contourf, label='Значение функции f(x₁,x₂)')
cbar.set_label('Значение функции f(x₁,x₂)', fontsize=12)

plt.tight_layout()
plt.show()

# Дополнительный график с разными уровнями в новом диапазоне
plt.figure(figsize=(14, 6))

# Подграфик 1: Близкие к минимуму уровни
plt.subplot(1, 2, 1)
levels_close = np.linspace(f_min, f_min + 100, 50)  # Увеличиваем количество уровней
contour1 = plt.contour(X1, X2, Z, levels=levels_close, colors='darkblue', linewidths=1.0)
plt.clabel(contour1, inline=True, fontsize=6)
plt.plot(trajectory[:, 0], trajectory[:, 1], 'ro-', markersize=2, linewidth=1.0)
plt.plot(x_start[0], x_start[1], 'go', markersize=6)
plt.plot(x_min[0], x_min[1], 'r*', markersize=8)
plt.xlabel('x₁')
plt.ylabel('x₂')
plt.title('Линии уровня близкие к минимуму')
plt.grid(True, alpha=0.3)
plt.axis('equal')
plt.xlim(-2, 2)
plt.ylim(-2, 2)

# Подграфик 2: Более широкий диапазон
plt.subplot(1, 2, 2)
levels_wide = np.linspace(f_min, f_min + 1000, 100)  # Увеличиваем количество уровней
contour2 = plt.contour(X1, X2, Z, levels=levels_wide, colors='darkred', linewidths=1.0)
plt.clabel(contour2, inline=True, fontsize=6)
plt.plot(trajectory[:, 0], trajectory[:, 1], 'ro-', markersize=2, linewidth=1.0)
plt.plot(x_start[0], x_start[1], 'go', markersize=6)
plt.plot(x_min[0], x_min[1], 'r*', markersize=8)
plt.xlabel('x₁')
plt.ylabel('x₂')
plt.title('Линии уровня - широкий диапазон')
plt.grid(True, alpha=0.3)
plt.axis('equal')
plt.xlim(-2, 2)
plt.ylim(-2, 2)

plt.tight_layout()
plt.show()

# Анализ результатов
print("\n" + "="*60)
print("АНАЛИЗ РЕЗУЛЬТАТОВ:")
print("="*60)
print("1. Метод обобщенного покоординатного спуска использует направления")
print("   собственных векторов матрицы Гессе для эффективного спуска.")
print("2. Для квадратичной функции метод сходится за конечное число шагов.")
print("3. В данном случае сходимость достигнута за минимальное количество")
print("   итераций благодаря выбору оптимальных направлений спуска.")
print("4. Собственные векторы указывают направления главных осей эллипсов,")
print("   что позволяет методу эффективно 'спускаться по оврагу' функции.")
print("5. Линии уровня теперь отображаются в диапазоне от -2 до 2 по обеим осям")
print("   с увеличенным в 100 раз количеством уровней для лучшей детализации.")
