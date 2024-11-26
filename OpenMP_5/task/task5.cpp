#include <iostream>
#include <vector>
#include <omp.h>
#include <chrono>
#include <fstream>
#include <string>
#include <algorithm>
#include <sstream>

// Функция для генерации ленточной матрицы
void generate_band_matrix(std::vector<std::vector<int>>& matrix, int bandwidth, int max_value) {
    int N = matrix.size();
    for (int i = 0; i < N; ++i) {
        for (int j = std::max(0, i - bandwidth); j <= std::min(N - 1, i + bandwidth); ++j) {
            matrix[i][j] = rand() % max_value;
        }
    }
}

// Функция для генерации нижнетреугольной матрицы
void generate_lower_triangular_matrix(std::vector<std::vector<int>>& matrix, int max_value) {
    int N = matrix.size();
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j <= i; ++j) {
            matrix[i][j] = rand() % max_value;
        }
    }
}

// Функция для поиска максимального значения среди минимальных элементов строк с заданной политикой распределения
int find_max_of_row_mins(const std::vector<std::vector<int>>& matrix, std::string schedule_type, int chunk_size) {
    int N = matrix.size();
    int global_max = INT_MIN;

#pragma omp parallel
    {
        int local_max = INT_MIN;

        if (schedule_type == "static") {
#pragma omp for schedule(static, chunk_size)
            for (int i = 0; i < N; ++i) {
                int row_min = *std::min_element(matrix[i].begin(), matrix[i].end());
                local_max = std::max(local_max, row_min);
            }
        }
        else if (schedule_type == "dynamic") {
#pragma omp for schedule(dynamic, chunk_size)
            for (int i = 0; i < N; ++i) {
                int row_min = *std::min_element(matrix[i].begin(), matrix[i].end());
                local_max = std::max(local_max, row_min);
            }
        }
        else if (schedule_type == "guided") {
#pragma omp for schedule(guided, chunk_size)
            for (int i = 0; i < N; ++i) {
                int row_min = *std::min_element(matrix[i].begin(), matrix[i].end());
                local_max = std::max(local_max, row_min);
            }
        }

#pragma omp critical
        global_max = std::max(global_max, local_max);
    }

    return global_max;
}

// Функция для считывания времени выполнения из предыдущих результатов
double read_previous_time(int num_threads, const std::string& filename = "task/previous_results.csv") {
    std::ifstream infile(filename);
    if (!infile) {
        std::cerr << "File " << filename << " not found!" << std::endl;
        return -1;
    }

    std::string line;
    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        int threads;
        char delimiter;
        double time;
        if (iss >> threads >> delimiter >> time && delimiter == ';' && threads == num_threads) {
            return time;
        }
    }
    return -1;  // Return -1 if no matching num_threads found
}

int main() {
    const int N = 10000;        // Размер матрицы
    const int max_value = 100;  // Максимальное значение элементов
    const int bandwidth = 10;   // Ширина ленты для ленточной матрицы
    int num_threads;

    std::cout << "Number of threads: ";
    std::cin >> num_threads;
    omp_set_num_threads(num_threads);

    // Создание файлов для записи результатов
    std::ofstream results("task/results.csv", std::ios_base::app);

    // Массив для хранения результатов ускорения
    std::vector<std::string> schedules = { "static", "dynamic", "guided" };

    for (const auto& schedule : schedules) {
        int chunk_size = (schedule == "static") ? N / num_threads : 1;

        // Ленточная матрица
        std::vector<std::vector<int>> band_matrix(N, std::vector<int>(N, 0));
        generate_band_matrix(band_matrix, bandwidth, max_value);

        auto start = std::chrono::high_resolution_clock::now();
        int band_result = find_max_of_row_mins(band_matrix, schedule, chunk_size);
        auto end = std::chrono::high_resolution_clock::now();
        double band_time = std::chrono::duration<double>(end - start).count();

        // Треугольная матрица
        std::vector<std::vector<int>> lower_triangular_matrix(N, std::vector<int>(N, 0));
        generate_lower_triangular_matrix(lower_triangular_matrix, max_value);

        start = std::chrono::high_resolution_clock::now();
        int tri_result = find_max_of_row_mins(lower_triangular_matrix, schedule, chunk_size);
        end = std::chrono::high_resolution_clock::now();
        double tri_time = std::chrono::duration<double>(end - start).count();

        // Получение времени последовательного выполнения из предыдущих результатов
        double previous_time = read_previous_time(num_threads);
        double band_speedup = previous_time / band_time;
        double tri_speedup = previous_time / tri_time;

        // Запись результатов
        results << "Band," << schedule << "," << num_threads << "," << band_time << "," << band_speedup << "\n";
        results << "Triangular," << schedule << "," << num_threads << "," << tri_time << "," << tri_speedup << "\n";

        std::cout << "Schedule: " << schedule
            << "\nBand Matrix - Time: " << band_time << " sec, Speedup: " << band_speedup
            << "\nTriangular Matrix - Time: " << tri_time << " sec, Speedup: " << tri_speedup << "\n\n";
    }

    results.close();
    return 0;
}
