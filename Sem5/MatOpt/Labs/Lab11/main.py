def calculate_params(R1, R2, R3, R4, k):
    # Расчет выходных параметров по формулам из PDF
    N = 5 + 2 * R1 * (4 / R2 - 1 / R1)  # Предположение: Ru = R1
    U = 1 - 0.03 * R4
    P = 20 / R1 + 10 / R2
    t2 = 0.5 + 0.1 * R2 / R4 + R2 * (6 + 0.1 * N) + 7 / (843 * R3)
    S = 10 * R2 / R4
    
    # Требования (t_i) и допуски (δ_i = 0.2 * |t_i|)
    t_N = k  # N ≥ k
    t_U = 0.8  # ΔU ≥ 0.8
    t_P = 20   # P ≤ 20
    t_t2 = 15  # t₂ ≤ 15
    t_S = 1.5  # S ≥ 1.5
    
    delta_N = 0.2 * abs(t_N)
    delta_U = 0.2 * abs(t_U)
    delta_P = 0.2 * abs(t_P)
    delta_t2 = 0.2 * abs(t_t2)
    delta_S = 0.2 * abs(t_S)
    
    # Расчет запасов z_i
    z1 = (N - t_N) / delta_N
    z2 = (U - t_U) / delta_U
    z3 = (t_P - P) / delta_P  # Инвертирован, т.к. P ≤ 20
    z4 = (t_t2 - t2) / delta_t2
    z5 = (S - t_S) / delta_S
    
    return [N, U, P, t2, S], [z1, z2, z3, z4, z5]

# Параметр k
k = 18

# Точка A: R1 = R2 = R3 = R4 = 1
params_A, z_A = calculate_params(1, 1, 1, 1, k)
J_A = min(z_A)

# Точка B: R1 = R2 = 0.5, R3 = 0.05, R4 = 0.1
params_B, z_B = calculate_params(0.5, 0.5, 0.05, 0.1, k)
J_B = min(z_B)

# Вывод результатов
print("Точка A (R1=R2=R3=R4=1):")
print(f"Параметры: N={params_A[0]:.2f}, ΔU={params_A[1]:.2f}, P={params_A[2]:.2f}, t₂={params_A[3]:.2f}, S={params_A[4]:.2f}")
print(f"Запасы z_i: {[f'{z:.2f}' for z in z_A]}")
print(f"J(A) = {J_A:.2f}\n")

print("Точка B (R1=R2=0.5, R3=0.05, R4=0.1):")
print(f"Параметры: N={params_B[0]:.2f}, ΔU={params_B[1]:.2f}, P={params_B[2]:.2f}, t₂={params_B[3]:.2f}, S={params_B[4]:.2f}")
print(f"Запасы z_i: {[f'{z:.2f}' for z in z_B]}")
print(f"J(B) = {J_B:.2f}\n")

# Сравнение точек
if J_A > J_B:
    print("Точка A лучше.")
else:
    print("Точка B лучше.")
