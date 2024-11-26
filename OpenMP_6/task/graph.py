import pandas as pd
import matplotlib.pyplot as plt

# Чтение данных из файла results.csv
data = pd.read_csv('results.csv', delimiter=';')

# Преобразование данных для удобства анализа
data['mode'] = data['mode'].astype(str)
data['num_threads'] = data['num_threads'].astype(int)
data['num_iterations'] = data['num_iterations'].astype(int)
data['time'] = data['time'].astype(float)

# Построение графиков для каждого значения num_iterations
for num_iterations in data['num_iterations'].unique():
    plt.figure(figsize=(10, 6))
    subset = data[data['num_iterations'] == num_iterations]
    
    # Построение графиков для каждого режима
    for mode in subset['mode'].unique():
        mode_data = subset[subset['mode'] == mode]
        plt.plot(mode_data['num_threads'], mode_data['time'], marker='o', label=f'{mode}')
    
    # Настройки графика
    plt.xlabel('Количество потоков')
    plt.ylabel('Время выполнения (сек)')
    plt.title(f'Сравнение времени выполнения для {num_iterations} итераций')
    plt.legend(title="Режим распределения")
    plt.grid(True)
    
    # Отображение графика
    plt.show()
