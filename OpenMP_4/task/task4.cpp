#include <iostream>
#include <vector>
#include <omp.h>
#include <chrono>
#include <fstream>
#include <cstdlib>
#include <algorithm>
#include <string>

// Функция для генерации случайной матрицы
void initialize_matrix(std::vector<std::vector<int>>& matrix, int max_value) {
#pragma omp parallel for
    for (int i = 0; i < matrix.size(); ++i) {
        for (int j = 0; j < matrix[i].size(); ++j) {
            matrix[i][j] = rand() % max_value;
        }
    }
}

// Функция для нахождения максимума среди минимальных элементов строк
int max_of_row_minimums(const std::vector<std::vector<int>>& matrix) {
    int max_min = INT_MIN;

#pragma omp parallel
    {
        int local_max_min = INT_MIN;

#pragma omp for
        for (int i = 0; i < matrix.size(); ++i) {
            int row_min = *std::min_element(matrix[i].begin(), matrix[i].end());
            if (row_min > local_max_min) {
                local_max_min = row_min;
            }
        }

#pragma omp critical
        {
            if (local_max_min > max_min) {
                max_min = local_max_min;
            }
        }
    }
    return max_min;
}

int main() {
    int num_threads;
    int rows, cols;

    std::cout << "Enter the number of threads: ";
    std::cin >> num_threads;
    omp_set_num_threads(num_threads);

    std::cout << "Enter the number of rows (a): ";
    std::cin >> rows;
    std::cout << "Enter the number of columns (b): ";
    std::cin >> cols;

    // Создание и инициализация матрицы
    std::vector<std::vector<int>> matrix(rows, std::vector<int>(cols));
    initialize_matrix(matrix, 1000); // Инициализация значениями от 0 до 999

    // Измерение времени выполнения
    auto start = std::chrono::high_resolution_clock::now();
    int result = max_of_row_minimums(matrix);
    auto end = std::chrono::high_resolution_clock::now();
    double time_taken = std::chrono::duration<double>(end - start).count();

    std::cout << "Max of row minimums: " << result << std::endl;
    std::cout << "Execution time: " << time_taken << " seconds" << std::endl;

    // Сохранение результатов в файл
    std::string filename = "task/results_" + std::to_string(rows) + "_" + std::to_string(cols) + ".csv";
    std::ofstream outfile(filename, std::ios_base::app);
    if (outfile.is_open()) {
        outfile << num_threads << ";" << time_taken << "\n";
        outfile.close();
        std::cout << "Results saved to " << filename << std::endl;
    }
    else {
        std::cerr << "Unable to open file for writing!" << std::endl;
    }

    return 0;
}
