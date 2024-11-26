import matplotlib.pyplot as plt
import csv

# Считывание данных из файла results.csv
rows_cols = []
not_nested_times = []
nested_times = []
coefs = []

with open('results.csv', 'r') as csvfile:
    reader = csv.reader(csvfile, delimiter=';')
    for row in reader:
        not_nested_times.append(float(row[0]))
        nested_times.append(float(row[1]))
        coefs.append(float(row[2]))
        rows_cols.append(row[3])

# Преобразование данных в список кортежей для сортировки
data = list(zip(rows_cols, not_nested_times, nested_times, coefs))
data.sort(key=lambda x: int(x[0]))  # Сортировка по rows_cols

# Распаковка отсортированных данных
rows_cols, not_nested_times, nested_times, coefs = zip(*data)

# Построение первого графика: сравнение времени выполнения
plt.figure(figsize=(14, 7))
plt.plot(rows_cols, not_nested_times, label='Not Nested Time', marker='o')
plt.plot(rows_cols, nested_times, label='Nested Time', marker='o')
plt.xlabel('Matrix Size (rows_cols)')
plt.ylabel('Execution Time (seconds)')
plt.title('Comparison of Execution Time (Not Nested vs Nested)')
plt.xticks(rotation=45)
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.show()

# Построение второго графика: коэффициент ускорения
plt.figure(figsize=(14, 7))
plt.plot(rows_cols, coefs, label='Speedup Coefficient (not_nested_time/nested_time)', marker='o', color='orange')
plt.xlabel('Matrix Size (rows_cols)')
plt.ylabel('Coefficient')
plt.title('Speedup Coefficient for Nested vs Not Nested Parallelism')
plt.xticks(rotation=45)
plt.grid(True)
plt.tight_layout()
plt.show()
