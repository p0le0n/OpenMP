import pandas as pd
import matplotlib.pyplot as plt

# Чтение данных из CSV файла
try:
    data = pd.read_csv('results.csv', delimiter=';', names=['num_threads', 'time_with_reduction', 'time_without_reduction'])
except:
    print("Невозможно найти или открыть results.csv!")
    exit()

# Сортировка данных по количеству потоков
data = data.sort_values(by='num_threads')

# Построение графика времени выполнения с редукцией
plt.figure(figsize=(10, 5))
plt.plot(data['num_threads'], data['time_with_reduction'], marker='o', linestyle='-', color='b', label='С редукцией')
plt.xlabel('Количество потоков')
plt.ylabel('Время выполнения (сек)')
plt.title('Время выполнения с редукцией')
plt.legend()
plt.grid(True)
plt.show()

# Построение графика времени выполнения без редукции
plt.figure(figsize=(10, 5))
plt.plot(data['num_threads'], data['time_without_reduction'], marker='o', linestyle='-', color='r', label='Без редукции')
plt.xlabel('Количество потоков')
plt.ylabel('Время выполнения (сек)')
plt.title('Время выполнения без редукции')
plt.legend()
plt.grid(True)
plt.show()

# Построение комбинированного графика
plt.figure(figsize=(10, 5))
plt.plot(data['num_threads'], data['time_with_reduction'], marker='o', linestyle='-', color='b', label='С редукцией')
plt.plot(data['num_threads'], data['time_without_reduction'], marker='o', linestyle='-', color='r', label='Без редукции')
plt.xlabel('Количество потоков')
plt.ylabel('Время выполнения (сек)')
plt.title('Сравнение времени выполнения с редукцией и без редукции')
plt.legend()
plt.grid(True)
plt.show()
