import pandas as pd
import matplotlib.pyplot as plt

# Чтение данных из файла
data = pd.read_csv('results.csv', delimiter=',', names=['matrix_type', 'schedule_type', 'num_threads', 'time', 'speedup'])

# Список режимов распределения и типов матриц
schedule_types = data['schedule_type'].unique()
matrix_types = data['matrix_type'].unique()

# 1. Графики времени и ускорения для каждого режима распределения итераций
for schedule in schedule_types:
    fig, axes = plt.subplots(2, 2, figsize=(14, 10))
    fig.suptitle(f'Режим распределения итераций: {schedule}', fontsize=16)

    for i, matrix in enumerate(matrix_types):
        # Фильтруем данные для текущего типа матрицы и режима распределения
        subset = data[(data['schedule_type'] == schedule) & (data['matrix_type'] == matrix)]
        subset = subset.sort_values(by='num_threads')  # Сортируем по количеству потоков

        # Время выполнения
        ax_time = axes[0, i]
        ax_time.plot(subset['num_threads'], subset['time'], marker='o', linestyle='-')
        ax_time.set_title(f'{matrix} Matrix - Время выполнения')
        ax_time.set_xlabel('Количество потоков')
        ax_time.set_ylabel('Время (сек)')
        ax_time.grid(True)

        # Ускорение
        ax_speedup = axes[1, i]
        ax_speedup.plot(subset['num_threads'], subset['speedup'], marker='o', linestyle='-')
        ax_speedup.set_title(f'{matrix} Matrix - Ускорение')
        ax_speedup.set_xlabel('Количество потоков')
        ax_speedup.set_ylabel('Ускорение')
        ax_speedup.grid(True)

    # Настраиваем расположение графиков
    plt.tight_layout(rect=[0, 0, 1, 0.96])
    plt.show()

# 2. Сравнительные графики для каждого типа матрицы по всем режимам распределения
fig, axes = plt.subplots(2, 2, figsize=(14, 10))
fig.suptitle('Сравнение времени и ускорения между режимами распределения', fontsize=16)

for i, matrix in enumerate(matrix_types):
    # Время выполнения для разных режимов распределения
    ax_time_compare = axes[0, i]
    for schedule in schedule_types:
        subset = data[(data['schedule_type'] == schedule) & (data['matrix_type'] == matrix)]
        subset = subset.sort_values(by='num_threads')
        ax_time_compare.plot(subset['num_threads'], subset['time'], marker='o', linestyle='-', label=f'{schedule}')
    
    ax_time_compare.set_title(f'{matrix} Matrix - Сравнение времени выполнения')
    ax_time_compare.set_xlabel('Количество потоков')
    ax_time_compare.set_ylabel('Время (сек)')
    ax_time_compare.legend()
    ax_time_compare.grid(True)

    # Ускорение для разных режимов распределения
    ax_speedup_compare = axes[1, i]
    for schedule in schedule_types:
        subset = data[(data['schedule_type'] == schedule) & (data['matrix_type'] == matrix)]
        subset = subset.sort_values(by='num_threads')
        ax_speedup_compare.plot(subset['num_threads'], subset['speedup'], marker='o', linestyle='-', label=f'{schedule}')
    
    ax_speedup_compare.set_title(f'{matrix} Matrix - Сравнение ускорения')
    ax_speedup_compare.set_xlabel('Количество потоков')
    ax_speedup_compare.set_ylabel('Ускорение')
    ax_speedup_compare.legend()
    ax_speedup_compare.grid(True)

# Настраиваем расположение графиков
plt.tight_layout(rect=[0, 0, 1, 0.96])
plt.show()
