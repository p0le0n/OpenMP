#include <iostream>
#include <omp.h>
#include <vector>
#include <chrono>
#include <fstream>
#include <cmath>
#include <string>

// Функция для вычисления f(x)
double f(double x) {
    return sin(x) * cos(x) + log(x + 1);
}

// Функция для вычисления интеграла методом прямоугольников
double integrate(double a, double b, int N) {
    double h = (b - a) / N;
    double sum = 0.0;

#pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < N; ++i) {
        double x_i = a + i * h;
        sum += f(x_i);
    }
    return h * sum;
}

int main() {
    double a, b;  // Пределы интегрирования
    int N;        // Число прямоугольников (количество разбиений)
    int num_threads;  // Количество потоков

    std::cout << "Enter the value of the lower limit (a): ";
    std::cin >> a;
    std::cout << "Enter the value of the upper limit (b): ";
    std::cin >> b;
    std::cout << "Enter the number of splits (N): ";
    std::cin >> N;
    std::cout << "Number of threads: ";
    std::cin >> num_threads;

    omp_set_num_threads(num_threads); // Установка количества потоков

    // Измерение времени выполнения интегрирования
    auto start = std::chrono::high_resolution_clock::now();
    double result = integrate(a, b, N);
    auto end = std::chrono::high_resolution_clock::now();
    double time_taken = std::chrono::duration<double>(end - start).count();

    std::cout << "Result of integration: " << result << std::endl;
    std::cout << "Time of execution: " << time_taken << " sec" << std::endl;

    // Формирование имени файла
    std::string filename = "task/results_" + std::to_string(static_cast<int>(a)) + "_" +
        std::to_string(static_cast<int>(b)) + "_" + std::to_string(N) + ".csv";

    // Запись результатов в CSV файл
    std::ofstream outfile(filename, std::ios_base::app); // Открываем файл на добавление
    if (outfile.is_open()) {
        outfile << num_threads << ";" << time_taken << "\n";
        outfile.close();
        std::cout << "Results are saved in results.csv" << std::endl;
    }
    else {
        std::cerr << "Can't open the file" << std::endl;
    }

    return 0;
}
