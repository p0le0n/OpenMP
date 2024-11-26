#include <iostream>
#include <vector>
#include <omp.h>
#include <chrono>
#include <fstream>
#include <windows.h>
#include <stdlib.h>

// Функция для инициализации векторов случайными значениями
void initialize_vector(std::vector<int>& vec, int max_value) {
#pragma omp parallel for
    for (int i = 0; i < vec.size(); ++i) {
        vec[i] = rand() % max_value + rand() / 100;
    }
}

// Функция вычисления скалярного произведения с редукцией
int scalar_product_with_reduction(const std::vector<int>& vec1, const std::vector<int>& vec2) {
    int result = 0;
#pragma omp parallel for reduction(+: result)
    for (int i = 0; i < vec1.size(); ++i) {
        result += vec1[i] * vec2[i];
    }
    return result;
}

// Функция вычисления скалярного произведения без редукции
int scalar_product_without_reduction(const std::vector<int>& vec1, const std::vector<int>& vec2) {
    int result = 0;
#pragma omp parallel
    {
        int local_result = 0;
#pragma omp for
        for (int i = 0; i < vec1.size(); ++i) {
            local_result += vec1[i] * vec2[i];
        }
#pragma omp critical
        result += local_result;
    }
    return result;
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

    const int vector_size = 10000000;  // Размер векторов
    const int max_value = 10000;       // Максимальное значение элементов
    std::vector<int> vec1(vector_size), vec2(vector_size);

    initialize_vector(vec1, max_value);
    initialize_vector(vec2, max_value);

    // Измерение времени выполнения с редукцией
    double avg_time_with_reduction = 0;
    int result_with_reduction;
    for (int i = 0; i < 100; ++i) {
        auto start = std::chrono::high_resolution_clock::now();
        result_with_reduction = scalar_product_with_reduction(vec1, vec2);
        auto end = std::chrono::high_resolution_clock::now();
        avg_time_with_reduction += std::chrono::duration<double>(end - start).count();
    }
    avg_time_with_reduction /= 100;
    std::cout << "Scalar product with reduction: " << result_with_reduction << ", time: " << avg_time_with_reduction << " sec" << std::endl;

    // Измерение времени выполнения без редукции
    double avg_time_without_reduction = 0;
    int result_without_reduction;
    for (int i = 0; i < 100; ++i) {
        auto start = std::chrono::high_resolution_clock::now();
        result_without_reduction = scalar_product_without_reduction(vec1, vec2);
        auto end = std::chrono::high_resolution_clock::now();
        avg_time_without_reduction += std::chrono::duration<double>(end - start).count();
    }
    avg_time_without_reduction /= 100;
    std::cout << "Scalar product without reduction: " << result_without_reduction << ", time: " << avg_time_without_reduction << " sec" << std::endl;

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
