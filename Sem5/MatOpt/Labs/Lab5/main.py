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

# Определяем диапазон вокруг минимума
x1_range = 0.3
x2_range = 0.5
x1 = np.linspace(x_min[0] - x1_range, x_min[0] + x1_range, 800)
x2 = np.linspace(x_min[1] - x2_range, x_min[1] + x2_range, 800)
X1, X2 = np.meshgrid(x1, x2)
Z = f(X1, X2)

# Значение в точке минимума
f_min = f(x_min[0], x_min[1])

# Построение графика
plt.figure(figsize=(14, 10))

# Густые линии уровня с плавными контурами
levels = np.linspace(f_min + 0.5, f_min + 20, 25)
contour = plt.contour(X1, X2, Z, levels=levels, colors='blue', alpha=0.8, linewidths=1.5)
plt.clabel(contour, inline=True, fontsize=9)

# Добавляем больше уровней для плавности
levels_dense = np.linspace(f_min + 0.1, f_min + 30, 80)
plt.contour(X1, X2, Z, levels=levels_dense, colors='blue', alpha=0.4, linewidths=0.8)

# Заполненный контур с плавными переходами
contourf = plt.contourf(X1, X2, Z, levels=100, alpha=0.5, cmap='viridis')

# Точка минимума
plt.plot(x_min[0], x_min[1], 'ro', markersize=10, label=f'Минимум ({x_min[0]:.3f}, {x_min[1]:.3f})')

# Собственные векторы (масштабированные для видимости)
scale = 0.1  # Масштаб для векторов
plt.arrow(x_min[0], x_min[1], u1[0]*scale, u1[1]*scale, 
          head_width=0.01, head_length=0.01, fc='red', ec='red', linewidth=2,
          label=f'u₁ (λ={lambda1:.1f})')
plt.arrow(x_min[0], x_min[1], u2[0]*scale, u2[1]*scale, 
          head_width=0.01, head_length=0.01, fc='green', ec='green', linewidth=2,
          label=f'u₂ (λ={lambda2:.1f})')

# Настройка графика
plt.xlabel('x₁', fontsize=14)
plt.ylabel('x₂', fontsize=14)
plt.title('Линии уровня квадратичной функции\n(замкнутые эллиптические контуры)', fontsize=16)
plt.grid(True, alpha=0.3)
plt.axis('equal')
plt.legend(fontsize=12)

# Цветовая шкала
cbar = plt.colorbar(contourf, label='Значение функции f(x₁,x₂)')
cbar.set_label('Значение функции f(x₁,x₂)', fontsize=12)

# Увеличиваем шрифты на осях
plt.xticks(fontsize=11)
plt.yticks(fontsize=11)

plt.tight_layout()
plt.show()

# Дополнительный график с разными уровнями
plt.figure(figsize=(14, 6))

# Подграфик 1: Близкие к минимуму уровни
plt.subplot(1, 2, 1)
levels_close = np.linspace(f_min + 0.1, f_min + 5, 15)
contour1 = plt.contour(X1, X2, Z, levels=levels_close, colors='darkblue', linewidths=1.5)
plt.clabel(contour1, inline=True, fontsize=8)
plt.plot(x_min[0], x_min[1], 'ro', markersize=8)
plt.xlabel('x₁')
plt.ylabel('x₂')
plt.title('Линии уровня близкие к минимуму')
plt.grid(True, alpha=0.3)
plt.axis('equal')

# Подграфик 2: Более широкий диапазон
plt.subplot(1, 2, 2)
levels_wide = np.linspace(f_min + 1, f_min + 40, 20)
contour2 = plt.contour(X1, X2, Z, levels=levels_wide, colors='darkred', linewidths=1.5)
plt.clabel(contour2, inline=True, fontsize=8)
plt.plot(x_min[0], x_min[1], 'ro', markersize=8)
plt.xlabel('x₁')
plt.ylabel('x₂')
plt.title('Линии уровня - широкий диапазон')
plt.grid(True, alpha=0.3)
plt.axis('equal')

plt.tight_layout()
plt.show()

# Дополнительная информация
print("\n" + "="*60)
print("ДОПОЛНИТЕЛЬНАЯ ИНФОРМАЦИЯ:")
print("="*60)
print(f"Значение функции в точке минимума: f(x_min) = {f_min:.6f}")
print(f"Определитель матрицы Гессе: det(H) = {np.linalg.det(H):.4f}")
print(f"След матрицы Гессе: tr(H) = {np.trace(H):.4f}")

# Проверка выпуклости
if lambda1 > 0 and lambda2 > 0:
    print("Матрица Гессе положительно определена - функция строго выпукла")
    print("Линии уровня представляют собой эллипсы")
else:
    print("Внимание: функция не является строго выпуклой!")

# Геометрические свойства эллипсов
print(f"\nОтношение осей эллипсов: {np.sqrt(max(lambda1, lambda2)/min(lambda1, lambda2)):.4f}")
print(f"Ориентация главной оси: {np.degrees(np.arctan2(u1[1], u1[0])):.2f}°")
