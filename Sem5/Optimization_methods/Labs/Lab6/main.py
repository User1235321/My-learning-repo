import numpy as np
import matplotlib.pyplot as plt

a = 605.45
b = 326.7
c = 51.05
d = 11.8
e = 11.8

H = np.array([[2*a, b],
              [b, 2*c]])

A = np.array([[2*a, b],
              [b, 2*c]])
b_vec = np.array([-d, -e])
x_min = np.linalg.solve(A, b_vec)

def f(x1, x2):
    return (a * x1**2 + b * x1 * x2 + c * x2**2 + 
            d * x1 + e * x2)

def grad(x):
    x1, x2 = x
    return np.array([
        2*a*x1 + b*x2 + d,
        b*x1 + 2*c*x2 + e
    ])

def coordinate_descent_5_segments(x0, max_segments=5):
    points = [x0.copy()]
    x = x0.copy()
    
    for segment in range(max_segments):
        x[0] = -(b * x[1] + d) / (2 * a)
        x[1] = -(b * x[0] + e) / (2 * c)
        points.append(x.copy())
    
    return np.array(points)

def rosenbrock_corrected(f, grad, x0, alpha=0.0001, max_iter=3):
    x = np.array(x0, dtype=float)
    d = np.eye(2)  # Начальные направления - координатные оси
    path = [x.copy()]
    coordinate_systems = [(x.copy(), d[0].copy(), d[1].copy())]
    direction_lines = []
    
    for k in range(max_iter):
        x_old = x.copy()
        
        # Поиск по текущим направлениям
        for i in range(2):
            # Градиентный спуск вдоль направления d[i]
            g = grad(x)
            directional_derivative = np.dot(g, d[i])
            x_new = x - alpha * directional_derivative * d[i]
            
            # Проверяем, улучшилась ли функция
            if f(x_new[0], x_new[1]) < f(x[0], x[1]):
                x = x_new
                path.append(x.copy())
        
        # Определяем новые направления
        s = x - x_old
        if np.linalg.norm(s) > 1e-12:
            # Первое новое направление - направление движения
            d1_new = s / np.linalg.norm(s)
            # Второе направление - ортогональное первому
            d2_new = np.array([-d1_new[1], d1_new[0]])
            
            d = np.array([d1_new, d2_new])
            direction_lines.append((x_old.copy(), x.copy()))
            coordinate_systems.append((x.copy(), d1_new.copy(), d2_new.copy()))
    
    return np.array(path), coordinate_systems, direction_lines

x0 = np.array([0.0, 0.0])
coord_points = coordinate_descent_5_segments(x0)
rosenbrock_points, rosenbrock_systems, direction_lines = rosenbrock_corrected(f, grad, x0)

all_points = np.vstack([coord_points, rosenbrock_points])
x1_min, x1_max = all_points[:, 0].min(), all_points[:, 0].max()
x2_min, x2_max = all_points[:, 1].min(), all_points[:, 1].max()

margin = 0.1
x1_range = max(abs(x1_min), abs(x1_max)) + margin
x2_range = max(abs(x2_min), abs(x2_max)) + margin

x1 = np.linspace(-x1_range, x1_range, 1000)
x2 = np.linspace(-x2_range, x2_range, 1000)
X1, X2 = np.meshgrid(x1, x2)
Z = f(X1, X2)

f_values = [f(point[0], point[1]) for point in all_points]
f_min_traj, f_max_traj = min(f_values), max(f_values)

levels_main = np.linspace(f_min_traj - 5, f_max_traj + 5, 50)
levels_dense = np.linspace(f_min_traj - 2, f_max_traj + 2, 100)

plt.figure(figsize=(14, 10))

contour = plt.contour(X1, X2, Z, levels=levels_main, colors='blue', alpha=0.8, linewidths=1.0)
plt.clabel(contour, inline=True, fontsize=8)

plt.contour(X1, X2, Z, levels=levels_dense, colors='blue', alpha=0.4, linewidths=0.6)

contourf = plt.contourf(X1, X2, Z, levels=200, alpha=0.5, cmap='viridis')

plt.plot(x_min[0], x_min[1], 'ro', markersize=10, label=f'Минимум ({x_min[0]:.3f}, {x_min[1]:.3f})')

plt.plot(coord_points[:, 0], coord_points[:, 1], 's-', color='red', linewidth=2, 
         markersize=6, label='Покоординатный спуск (5 звеньев)')

for i, point in enumerate(coord_points):
    plt.annotate(str(i), (point[0], point[1]), textcoords="offset points", 
                 xytext=(0,10), ha='center', fontsize=8, color='red')

plt.xlabel('x₁', fontsize=14)
plt.ylabel('x₂', fontsize=14)
plt.title('Покоординатный спуск (5 звеньев)', fontsize=16)
plt.grid(True, alpha=0.3)
plt.axis('equal')
plt.legend(fontsize=12)

cbar = plt.colorbar(contourf)
cbar.set_label('Значение функции f(x₁,x₂)', fontsize=12)

plt.xticks(fontsize=11)
plt.yticks(fontsize=11)

plt.tight_layout()
plt.show()

plt.figure(figsize=(14, 10))

contour = plt.contour(X1, X2, Z, levels=levels_main, colors='blue', alpha=0.8, linewidths=1.0)
plt.clabel(contour, inline=True, fontsize=8)

plt.contour(X1, X2, Z, levels=levels_dense, colors='blue', alpha=0.4, linewidths=0.6)

contourf = plt.contourf(X1, X2, Z, levels=200, alpha=0.5, cmap='viridis')

plt.plot(x_min[0], x_min[1], 'ro', markersize=10, label=f'Минимум ({x_min[0]:.3f}, {x_min[1]:.3f})')

plt.plot(rosenbrock_points[:, 0], rosenbrock_points[:, 1], '^-', color='green', linewidth=2, 
         markersize=6, label='Розенброк (3 итерации)')

# Рисуем пунктирные линии, показывающие как определяются новые системы координат
for i, (start, end) in enumerate(direction_lines):
    plt.plot([start[0], end[0]], [start[1], end[1]], 'k--', linewidth=1, alpha=0.7,
             label='Направление нового базиса' if i == 0 else "")

for i, (start_point, d1, d2) in enumerate(rosenbrock_systems):
    scale = 0.05
    plt.arrow(start_point[0], start_point[1], d1[0]*scale, d1[1]*scale, 
              head_width=0.01, head_length=0.01, fc='red', ec='red', linewidth=2,
              label=f'Система {i}: d1' if i == 0 else "")
    plt.arrow(start_point[0], start_point[1], d2[0]*scale, d2[1]*scale, 
              head_width=0.01, head_length=0.01, fc='blue', ec='blue', linewidth=2,
              label=f'Система {i}: d2' if i == 0 else "")
    
    angle1 = np.degrees(np.arctan2(d1[1], d1[0]))
    angle2 = np.degrees(np.arctan2(d2[1], d2[0]))
    
    plt.annotate(f'Система {i}\nd1: {angle1:.1f}°\nd2: {angle2:.1f}°', 
                 (start_point[0], start_point[1]), textcoords="offset points", 
                 xytext=(15,15), ha='left', fontsize=9, 
                 bbox=dict(boxstyle="round,pad=0.3", fc="white", alpha=0.8))

for i, point in enumerate(rosenbrock_points):
    plt.annotate(str(i), (point[0], point[1]), 
                 textcoords="offset points", xytext=(0,-15), ha='center', 
                 fontsize=8, color='green')

plt.xlabel('x₁', fontsize=14)
plt.ylabel('x₂', fontsize=14)
plt.title('Метод Розенброка (3 итерации) с поворачивающимися системами координат', fontsize=16)
plt.grid(True, alpha=0.3)
plt.axis('equal')
plt.legend(fontsize=12)

cbar = plt.colorbar(contourf)
cbar.set_label('Значение функции f(x₁,x₂)', fontsize=12)

plt.xticks(fontsize=11)
plt.yticks(fontsize=11)

plt.tight_layout()
plt.show()

print("\nТочки покоординатного спуска (5 звеньев):")
print("Звено\t   x₁\t\t   x₂\t\t   f(x)")
print("-" * 55)
for i, point in enumerate(coord_points):
    print(f"{i}\t{point[0]:10.6f}\t{point[1]:10.6f}\t{f(point[0], point[1]):10.6f}")

print("\nТочки метода Розенброка:")
print("Шаг\t   x₁\t\t   x₂\t\t   f(x)")
print("-" * 55)
for i, point in enumerate(rosenbrock_points):
    print(f"{i}\t{point[0]:10.6f}\t{point[1]:10.6f}\t{f(point[0], point[1]):10.6f}")

print("\nСистемы координат в методе Розенброка:")
print("Система\t  Точка x\t  Точка y\t  d1_x\t\t  d1_y\t\t  d2_x\t\t  d2_y")
print("-" * 95)
for i, (point, d1, d2) in enumerate(rosenbrock_systems):
    print(f"{i}\t{point[0]:10.6f}\t{point[1]:10.6f}\t{d1[0]:10.6f}\t{d1[1]:10.6f}\t{d2[0]:10.6f}\t{d2[1]:10.6f}")

print("\nЛинии, определяющие новые системы координат:")
print("Линия\t  Начало x\t  Начало y\t  Конец x\t  Конец y")
print("-" * 75)
for i, (start, end) in enumerate(direction_lines):
    print(f"{i}\t{start[0]:10.6f}\t{start[1]:10.6f}\t{end[0]:10.6f}\t{end[1]:10.6f}")
