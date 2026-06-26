import csv
import matplotlib.pyplot as plt
import numpy as np

# Если файл лежит в /tmp – укажите путь '/tmp/jsonb_measure.csv'
# Если скопировали в текущую папку – оставьте 'jsonb_measure.csv'
file_path = '/tmp/jsonb_measure.csv'   # или 'jsonb_measure.csv'

lengths = []
times = []

with open(file_path, 'r') as f:
    reader = csv.DictReader(f)
    for row in reader:
        lengths.append(int(row['data_length']))
        times.append(float(row['access_time_ms']))

plt.figure(figsize=(10, 6))
plt.scatter(lengths, times, s=10, alpha=0.5, color='blue', label='Измерения')

# Скользящее среднее (окно 30, т.к. 500 записей)
window = 30
if len(times) >= window:
    avg_len = []
    avg_time = []
    for i in range(len(times) - window + 1):
        avg_len.append(np.mean(lengths[i:i+window]))
        avg_time.append(np.mean(times[i:i+window]))
    plt.plot(avg_len, avg_time, 'r-', linewidth=2, label='Скользящее среднее (окно 30)')

# Порог TOAST
plt.axvline(x=2000, color='green', linestyle='--', alpha=0.7, label='Порог TOAST (2 КБ)')

# Принудительный масштаб Y – именно то, что нужно (0–0.6 мс)
plt.ylim(0, 0.05)

plt.xlabel('Длина JSONB (байт)')
plt.ylabel('Время доступа (мс)')
plt.title('Зависимость времени доступа к полю "year" от длины JSONB (масштаб 0–0.6 мс)')
plt.legend()
plt.grid(True, alpha=0.3)

plt.savefig('jsonb_step.png', dpi=150)
plt.show()
