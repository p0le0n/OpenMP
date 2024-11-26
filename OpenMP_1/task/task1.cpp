#include <iostream>
#include <vector>
#include <omp.h>
#include <algorithm>
#include <chrono>
#include <windows.h>
#include <stdlib.h> 
#include <fstream>

// Функция для инициализации вектора случайными значениями
void initialize_vector(std::vector<int>& vec, int max_value) {
#pragma omp parallel for
    for (int i = 0; i < vec.size(); ++i) {
        vec[i] = rand() % max_value + rand() / 100;
    }
}

// Функция нахождения минимума с редукцией
int find_min_with_reduction(const std::vector<int>& vec) {
    int min_value = vec[0];
#pragma omp parallel for reduction(min: min_value)
    for (int i = 0; i < vec.size(); ++i) {
        if (vec[i] < min_value) {
            min_value = vec[i];
        }
    }
    return min_value;
}

// Функция нахождения минимума без редукции
int find_min_without_reduction(const std::vector<int>& vec) {
    int min_value = vec[0];
#pragma omp parallel
    {
        int local_min = vec[0];
#pragma omp for
        for (int i = 0; i < vec.size(); ++i) {
            if (vec[i] < local_min) {
                local_min = vec[i];
            }
        }
#pragma omp critical
        {
            if (local_min < min_value) {
                min_value = local_min;
            }
        }
    }
    return min_value;
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    srand(time(NULL));

    int num_threads; // Желаемое количество потоков
    std::cout << "Number of threads: ";
    std::cin >> num_threads;
    omp_set_num_threads(num_threads);  // Установка количества потоков
    std::cout << std::endl;

    const int vector_size = 10000000;  // Размер вектора
    const int max_value = 10000;      // Максимальное значение элементов
    std::vector<int> vec(vector_size);

    initialize_vector(vec, max_value);

    // Вывод вектора
    std::cout << "Vector: [ ";
    for (int i = 1; i < 10; ++i) {
        std::cout << vec[i] << " ";
    }
    std::cout << "... ";
    for (int i = vec.size() - 10; i < vec.size(); ++i) {
        std::cout << vec[i] << " ";
    }
    std::cout << "]" << std::endl;

    // Измерение времени выполнения с редукцией
    double avg_time_with_reduction = 0;
    int min_with_reduction;
    for (int i = 0; i < 100; ++i) {
        auto start = std::chrono::high_resolution_clock::now();
        min_with_reduction = find_min_with_reduction(vec);
        auto end = std::chrono::high_resolution_clock::now();
        avg_time_with_reduction += std::chrono::duration<double>(end - start).count();
    }
    avg_time_with_reduction /= 100;
    std::cout << "MIN with reduction: " << min_with_reduction << ", time: " << avg_time_with_reduction << " sec" << std::endl;

    // Измерение времени выполнения без редукции
    double avg_time_without_reduction = 0;
    int min_without_reduction;
    for (int i = 0; i < 100; ++i) {
        auto start = std::chrono::high_resolution_clock::now();
        min_without_reduction = find_min_without_reduction(vec);
        auto end = std::chrono::high_resolution_clock::now();
        avg_time_without_reduction += std::chrono::duration<double>(end - start).count();
    }
    avg_time_without_reduction /= 100;
    std::cout << "MIN without reduction: " << min_without_reduction << ", time: " << avg_time_without_reduction << " sec" << std::endl;

    // Запись результатов в CSV файл
    std::ofstream outfile("task/results.csv", std::ios_base::app); // Открываем файл на добавление
    if (outfile.is_open()) {
        outfile << num_threads << ";" << avg_time_with_reduction << ";" << avg_time_without_reduction << "\n";
        outfile.close();
        std::cout << "Results are saved in results.csv" << std::endl;
    }
    else {
        std::cerr << "Can't open a file" << std::endl;
    }

    return 0;
}