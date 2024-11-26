import pandas as pd
import matplotlib.pyplot as plt

# Чтение данных из файла results.csv
try:
    data = pd.read_csv('results.csv', delimiter=';', names=['N', 'M', 'time'])
except FileNotFoundError:
    print("Файл results.csv не найден!")
    exit()

# Создание столбца для пар (N, M)
data['N, M'] = data['N'].astype(str) + ',' + data['M'].astype(str)

# Построение гистограммы
plt.figure(figsize=(10, 6))
plt.bar(data['N, M'], data['time'], color='skyblue')

# Настройка графика
plt.xlabel('Пары (N, M)', fontsize=12)
plt.ylabel('Время (с)', fontsize=12)
plt.title('Время вычислений для различных наборов векторов', fontsize=14)
plt.xticks(rotation=45, ha='right')
plt.tight_layout()

# Отображение графика
plt.show()

