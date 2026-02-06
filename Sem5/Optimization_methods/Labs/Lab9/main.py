import numpy as np
import matplotlib.pyplot as plt

# Матрица Гессе
a = 605.45
b = 326.7
c = 51.05
H = np.array([[2*a, b],
              [b, 2*c]])

# Собственные значения и векторы
eigenvalues, eigenvectors = np.linalg.eig(H)
lambda1, lambda2 = eigenvalues
u1, u2 = eigenvectors[:, 0], eigenvectors[:, 1]
T = eigenvectors

print("МАТРИЦА ГЕССЕ:")
print(H)
print(f"\nСобственные значения: λ₁ = {lambda1:.4f}, λ₂ = {lambda2:.4f}")

# 1. Матричная функция F(t) = t^(-1)
print("\n" + "="*50)
print("1. МАТРИЧНАЯ ФУНКЦИЯ F(A) = A^(-1)")
F1_diag = np.diag([1/lambda1, 1/lambda2])
F1_A = T @ F1_diag @ T.T
print("Матрица A^(-1):")
print(F1_A)

# 2. Матричная функция F(t) = sin(t)
print("\n" + "="*50)
print("2. МАТРИЧНАЯ ФУНКЦИЯ F(A) = sin(A)")
F2_diag = np.diag([np.sin(lambda1), np.sin(lambda2)])
F2_A = T @ F2_diag @ T.T
print("Матрица sin(A):")
print(F2_A)

# 3. Матричная функция F(t) = ∫₀ʰ e^(-tτ)dτ, h=0.1
print("\n" + "="*50)
print("3. МАТРИЧНАЯ ФУНКЦИЯ F(A) = ∫₀ʰ e^(-Aτ)dτ, h=0.1")
h = 0.1
def integral_func(t):
    return (1 - np.exp(-t * h)) / t if abs(t) > 1e-12 else h

F3_diag = np.diag([integral_func(lambda1), integral_func(lambda2)])
F3_A = T @ F3_diag @ T.T
print("Матрица ∫₀ʰ e^(-Aτ)dτ:")
print(F3_A)

# Проверка равенства для sin(A)
print("\n" + "="*50)
print("ПРОВЕРКА РАВЕНСТВА sin(A)u_i = sin(λ_i)u_i")

for i, (lambda_val, u) in enumerate([(lambda1, u1), (lambda2, u2)], 1):
    left = F2_A @ u
    right = np.sin(lambda_val) * u
    diff = np.max(np.abs(left - right))
    print(f"Для u_{i}: разница = {diff:.2e} {'✓' if diff < 1e-10 else '✗'}")

# ВИЗУАЛИЗАЦИЯ В ДВУМЕРНОЙ ПРОЕКЦИИ
print("\n" + "="*50)
print("ВИЗУАЛИЗАЦИЯ МАТРИЧНЫХ ФУНКЦИЙ (ВИД СВЕРХУ)")

# Создаем сетку для визуализации
x = np.linspace(-0.15, 0.15, 100)
y = np.linspace(-0.15, 0.15, 100)
X, Y = np.meshgrid(x, y)

# Функция для вычисления квадратичной формы x^T M x
def quadratic_form(M, x1, x2):
    points = np.stack([x1.flatten(), x2.flatten()], axis=1)
    return np.array([p @ M @ p for p in points]).reshape(x1.shape)

# Вычисляем квадратичные формы для всех матриц
Z_H = quadratic_form(H, X, Y)
Z_inv = quadratic_form(F1_A, X, Y)
Z_sin = quadratic_form(F2_A, X, Y)
Z_int = quadratic_form(F3_A, X, Y)

# Создаем графики
fig, axes = plt.subplots(2, 2, figsize=(14, 12))

# 1. Исходная матрица Гессе
contour1 = axes[0,0].contour(X, Y, Z_H, levels=15, colors='blue', linewidths=1.5)
axes[0,0].clabel(contour1, inline=True, fontsize=8)
axes[0,0].set_title('Исходная матрица Гессе H\n$f(x) = x^T H x$', fontsize=14)
axes[0,0].set_xlabel('x₁')
axes[0,0].set_ylabel('x₂')
axes[0,0].grid(True, alpha=0.3)
axes[0,0].set_aspect('equal')

# Собственные векторы для H
scale = 0.08
axes[0,0].arrow(0, 0, u1[0]*scale, u1[1]*scale, 
               head_width=0.01, head_length=0.01, 
               fc='red', ec='red', linewidth=2,
               label=f'u₁ (λ={lambda1:.1f})')
axes[0,0].arrow(0, 0, u2[0]*scale, u2[1]*scale, 
               head_width=0.01, head_length=0.01, 
               fc='green', ec='green', linewidth=2,
               label=f'u₂ (λ={lambda2:.1f})')
axes[0,0].legend()

# 2. Обратная матрица A^(-1)
contour2 = axes[0,1].contour(X, Y, Z_inv, levels=15, colors='red', linewidths=1.5)
axes[0,1].clabel(contour2, inline=True, fontsize=8)
axes[0,1].set_title('Обратная матрица $H^{-1}$\n$f(x) = x^T H^{-1} x$', fontsize=14)
axes[0,1].set_xlabel('x₁')
axes[0,1].set_ylabel('x₂')
axes[0,1].grid(True, alpha=0.3)
axes[0,1].set_aspect('equal')

# Собственные векторы для H^(-1)
axes[0,1].arrow(0, 0, u1[0]*scale, u1[1]*scale, 
               head_width=0.01, head_length=0.01, 
               fc='red', ec='red', linewidth=2,
               label=f'u₁ (λ={1/lambda1:.3f})')
axes[0,1].arrow(0, 0, u2[0]*scale, u2[1]*scale, 
               head_width=0.01, head_length=0.01, 
               fc='green', ec='green', linewidth=2,
               label=f'u₂ (λ={1/lambda2:.3f})')
axes[0,1].legend()

# 3. Матрица sin(A)
contour3 = axes[1,0].contour(X, Y, Z_sin, levels=15, colors='green', linewidths=1.5)
axes[1,0].clabel(contour3, inline=True, fontsize=8)
axes[1,0].set_title('Матрица $\\sin(H)$\n$f(x) = x^T \\sin(H) x$', fontsize=14)
axes[1,0].set_xlabel('x₁')
axes[1,0].set_ylabel('x₂')
axes[1,0].grid(True, alpha=0.3)
axes[1,0].set_aspect('equal')

# Собственные векторы для sin(H)
axes[1,0].arrow(0, 0, u1[0]*scale, u1[1]*scale, 
               head_width=0.01, head_length=0.01, 
               fc='red', ec='red', linewidth=2,
               label=f'u₁ (λ={np.sin(lambda1):.3f})')
axes[1,0].arrow(0, 0, u2[0]*scale, u2[1]*scale, 
               head_width=0.01, head_length=0.01, 
               fc='green', ec='green', linewidth=2,
               label=f'u₂ (λ={np.sin(lambda2):.3f})')
axes[1,0].legend()

# 4. Интегральная матрица
contour4 = axes[1,1].contour(X, Y, Z_int, levels=15, colors='orange', linewidths=1.5)
axes[1,1].clabel(contour4, inline=True, fontsize=8)
axes[1,1].set_title('Интегральная матрица\n$f(x) = x^T (\\int_0^h e^{-H\\tau}d\\tau) x$', fontsize=14)
axes[1,1].set_xlabel('x₁')
axes[1,1].set_ylabel('x₂')
axes[1,1].grid(True, alpha=0.3)
axes[1,1].set_aspect('equal')

# Собственные векторы для интегральной матрицы
axes[1,1].arrow(0, 0, u1[0]*scale, u1[1]*scale, 
               head_width=0.01, head_length=0.01, 
               fc='red', ec='red', linewidth=2,
               label=f'u₁ (λ={integral_func(lambda1):.4f})')
axes[1,1].arrow(0, 0, u2[0]*scale, u2[1]*scale, 
               head_width=0.01, head_length=0.01, 
               fc='green', ec='green', linewidth=2,
               label=f'u₂ (λ={integral_func(lambda2):.4f})')
axes[1,1].legend()

plt.tight_layout()
plt.show()

# Дополнительный график - все функции вместе
plt.figure(figsize=(12, 10))

contour_H = plt.contour(X, Y, Z_H, levels=10, colors='blue', linewidths=2, linestyles='-', alpha=0.8)
contour_inv = plt.contour(X, Y, Z_inv, levels=10, colors='red', linewidths=2, linestyles='--', alpha=0.8)
contour_sin = plt.contour(X, Y, Z_sin, levels=10, colors='green', linewidths=2, linestyles='-.', alpha=0.8)
contour_int = plt.contour(X, Y, Z_int, levels=10, colors='orange', linewidths=2, linestyles=':', alpha=0.8)

plt.clabel(contour_H, inline=True, fontsize=8, colors='blue')
plt.clabel(contour_inv, inline=True, fontsize=8, colors='red')
plt.clabel(contour_sin, inline=True, fontsize=8, colors='green')
plt.clabel(contour_int, inline=True, fontsize=8, colors='orange')

# Собственные векторы (общие для всех)
scale = 0.08
plt.arrow(0, 0, u1[0]*scale, u1[1]*scale, 
         head_width=0.01, head_length=0.01, 
         fc='purple', ec='purple', linewidth=3,
         label='u₁')
plt.arrow(0, 0, u2[0]*scale, u2[1]*scale, 
         head_width=0.01, head_length=0.01, 
         fc='brown', ec='brown', linewidth=3,
         label='u₂')

plt.title('Сравнение линий уровня всех матричных функций', fontsize=16)
plt.xlabel('x₁', fontsize=14)
plt.ylabel('x₂', fontsize=14)
plt.grid(True, alpha=0.3)
plt.axis('equal')
plt.legend(['H', 'H⁻¹', 'sin(H)', '∫exp(-Hτ)dτ', 'u₁', 'u₂'], fontsize=12)

plt.tight_layout()
plt.show()

# Дополнительная информация о собственных значениях матричных функций
print("\n" + "="*50)
print("СОБСТВЕННЫЕ ЗНАЧЕНИЯ МАТРИЧНЫХ ФУНКЦИЙ:")
print(f"Исходные: λ₁ = {lambda1:.4f}, λ₂ = {lambda2:.4f}")
print(f"H⁻¹: λ₁ = {1/lambda1:.6f}, λ₂ = {1/lambda2:.6f}")
print(f"sin(H): λ₁ = {np.sin(lambda1):.6f}, λ₂ = {np.sin(lambda2):.6f}")
print(f"∫exp(-Hτ)dτ: λ₁ = {integral_func(lambda1):.6f}, λ₂ = {integral_func(lambda2):.6f}")

print("\n" + "="*50)
print("ВЫВОДЫ:")
print("✓ Все матричные функции успешно построены через спектральное разложение")
print("✓ Проверенное равенство подтверждает теоретический результат")
print("✓ Собственные векторы сохраняются для всех матричных функций")
print("✓ Собственные значения преобразуются согласно скалярной функции")
