import pandas as pd
import matplotlib.pyplot as plt

# Чтение данных из файла
try:
    data = pd.read_csv('results.csv', delimiter=';', names=['num_threads', 'method', 'time'])
except FileNotFoundError:
    print("Файл results.csv не найден.")
    exit()

# Группировка данных по методам редукции
methods = data['method'].unique()

plt.figure(figsize=(12, 6))

# Построение графиков для каждого метода
for method in methods:
    method_data = data[data['method'] == method]
    plt.plot(method_data['num_threads'], method_data['time'], marker='o', label=method)

plt.xlabel('Количество потоков')
plt.ylabel('Время выполнения (сек)')
plt.title('Сравнение методов редукции')
plt.legend()
plt.grid(True)
plt.show()
