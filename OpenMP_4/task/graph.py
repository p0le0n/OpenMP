import pandas as pd
import matplotlib.pyplot as plt
import os
import re

# Путь к папке с результатами
path = '.'

# Шаблон для поиска файлов с названием "results_a_b_N.csv"
pattern = r'results_(\d+)_(\d+)\.csv'

# Получение всех файлов, начинающихся с "results_" в папке
files = [f for f in os.listdir(path) if re.match(pattern, f)]

if not files:
    print("Файлы с именем 'results_' не найдены в папке!")
    exit()

# Построение графиков для каждого файла
for file in files:
    # Извлечение значений a, b, N из имени файла
    match = re.match(pattern, file)
    if match:
        a, b = match.groups()
    
        # Чтение данных из CSV файла
        data = pd.read_csv(os.path.join(path, file), delimiter=';', names=['num_threads', 'time'])
    
        # Сортировка данных по количеству потоков
        data = data.sort_values(by='num_threads')
    
        # Построение графика
        plt.figure(figsize=(10, 5))
        plt.plot(data['num_threads'], data['time'], marker='o', linestyle='-', color='r', label='Время выполнения')
        plt.xlabel('Количество потоков')
        plt.ylabel('Время выполнения (сек)')
        plt.title(f'Время выполнения вычислений для rows={a}, cols={b}')
        plt.legend()
        plt.grid(True)
                
        plt.show()
