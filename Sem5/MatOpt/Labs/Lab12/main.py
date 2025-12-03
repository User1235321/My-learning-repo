import numpy as np
import matplotlib.pyplot as plt

def relaxation_gd(lambda_val, alpha=0.01):
    return 1 - alpha * lambda_val

def relaxation_newton(lambda_val):
    return 0

def relaxation_levenberg(lambda_val, h):
    return (h - lambda_val) / (h + lambda_val)

def plot_relaxation_function_k18():
    k = 18
    h = k / (k + 10)
    
    lambda_left = np.linspace(-10, -h-0.01, 200)
    R_left = relaxation_levenberg(lambda_left, h)
    
    lambda_right = np.linspace(-h+0.01, 10, 400)
    R_right = relaxation_levenberg(lambda_right, h)
    
    plt.figure(figsize=(12, 8))
    
    plt.plot(lambda_left, R_left, 'b-', linewidth=3)
    plt.plot(lambda_right, R_right, 'b-', linewidth=3, label=rf'$R(\lambda) = \frac{{{h:.3f}-\lambda}}{{{h:.3f}+\lambda}}$')
    
    plt.axvline(x=-h, color='red', linestyle='--', linewidth=2, alpha=0.7, label=rf'Асимптота $\lambda = -h = -{h:.3f}$')
    plt.axhline(y=0, color='black', linestyle=':', linewidth=1, alpha=0.5)
    plt.axhline(y=1, color='green', linestyle='--', linewidth=1.5, alpha=0.5)
    plt.axhline(y=-1, color='orange', linestyle='--', linewidth=1.5, alpha=0.5)
    plt.axvline(x=h, color='purple', linestyle=':', linewidth=2, alpha=0.7, label=rf'$R=0$ при $\lambda = h = {h:.3f}$')
    
    mask_relax = (lambda_right > 0) & (np.abs(R_right) < 1)
    if np.any(mask_relax):
        plt.fill_between(lambda_right[mask_relax], -1, 1, alpha=0.15, color='green', label='Область релаксационности (|R|<1)')
    
    plt.xlabel(r'$\lambda$')
    plt.ylabel(r'$R(\lambda)$')
    plt.title(f'Функция релаксации для k={k} (h={h:.6f})')
    plt.xlim(-10, 10)
    plt.ylim(-10, 10)
    plt.grid(True, alpha=0.3, linestyle='--')
    plt.legend(loc='upper right')
    
    plt.plot(0, 1, 'ro', markersize=10, label=r'$R(0)=1$')
    plt.plot(h, 0, 'go', markersize=10, label=rf'$R({h:.2f})=0$')
    
    plt.tight_layout()
    plt.show()
    
    return h

def plot_paraboloid_relaxation():
    m = 0.1
    M = 10.0
    h = 18/28
    
    lambda_vals = np.linspace(m, M, 500)
    
    plt.figure(figsize=(12, 8))
    
    R_gd = relaxation_gd(lambda_vals, alpha=2/(m+M))
    R_newton = np.zeros_like(lambda_vals)
    R_levenberg = relaxation_levenberg(lambda_vals, h)
    
    plt.plot(lambda_vals, R_gd, 'r-', linewidth=3, label=f'ПГС (α={2/(m+M):.3f})')
    plt.plot(lambda_vals, R_newton, 'g--', linewidth=3, label='Метод Ньютона')
    plt.plot(lambda_vals, R_levenberg, 'b-', linewidth=3, label=f'Метод Левенберга (h={h:.3f})')
    
    plt.axhline(y=0, color='k', linestyle=':', alpha=0.5)
    plt.axhline(y=1, color='r', linestyle='--', alpha=0.3, label='R=1')
    plt.axhline(y=-1, color='r', linestyle='--', alpha=0.3, label='R=-1')
    plt.axvline(x=m, color='purple', linestyle=':', alpha=0.7, label=rf'$m = {m}$')
    plt.axvline(x=M, color='orange', linestyle=':', alpha=0.7, label=rf'$M = {M}$')
    
    plt.fill_between(lambda_vals, -1, 1, alpha=0.1, color='green', label='Область релаксационности (|R|<1)')
    
    plt.xlabel(r'$\lambda$')
    plt.ylabel(r'$R(\lambda)$')
    plt.title('Анализ скорости сходимости на параболоиде')
    plt.grid(True, alpha=0.3)
    plt.legend(loc='best')
    
    plt.tight_layout()
    plt.show()
    
    return m, M

def analyze_methods():
    k = 18
    h = k / (k + 10)
    
    print(f"Параметры для k={k}: h={h:.6f}")
    print("Функция релаксации: R(λ) = (h-λ)/(h+λ)")
    print()
    print("Сравнение методов для данной задачи:")
    print()
    print("1. Простой градиентный спуск (ПГС):")
    print("   - Проблема: R(λ) ≈ 1 для малых λ → очень медленная сходимость")
    print("   - Чувствителен к выбору шага α")
    print("   - Не подходит для овражных функций")
    print()
    print("2. Метод Ньютона:")
    print("   - R(λ) = 0 → мгновенная сходимость в теории")
    print("   - Проблема: неустойчив при λ → 0 (вырожденный гессиан)")
    print("   - Требует вычисления и обращения гессиана")
    print()
    print("3. Метод Левенберга (для k=18):")
    print(f"   - h = {h:.6f} → автоматическая регуляризация")
    print("   - |R(λ)| < 1 при λ > 0 → гарантированная сходимость")
    print(f"   - R(h) = 0 → оптимальная сходимость при λ = {h:.3f}")
    print("   - Устойчив к вырожденному гессиану")
    print()
    print("ВЫВОД ДЛЯ k=18:")
    print("Лучший метод - Левенберг-Маркуардт, потому что:")
    print("1. Обеспечивает устойчивость благодаря hE в знаменателе")
    print("2. Автоматически адаптируется к разным λ")
    print("3. Не требует ручного подбора шага как ПГС")
    print("4. Работает с овражными функциями лучше метода Ньютона")
    print("5. Гарантирует сходимость для любых λ > 0")

if __name__ == "__main__":
    h = plot_relaxation_function_k18()
    m, M = plot_paraboloid_relaxation()
    analyze_methods()
