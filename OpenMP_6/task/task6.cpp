#include <omp.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <chrono>
#include <string>

// Функция для создания неравномерной нагрузки
void perform_task(int iteration) {
    int load = (iteration % 3 == 0) ? 100000 : 1000; // Неравномерная нагрузка
    std::vector<int> random_numbers(load);
    std::mt19937 gen(iteration);
    std::uniform_int_distribution<> dis(1, 100);

    for (int i = 0; i < load; ++i) {
        random_numbers[i] = dis(gen); // Генерация случайных чисел
    }
    // Дополнительные вычисления для нагрузки
    int sum = 0;
    for (int i = 0; i < load; ++i) {
        sum += random_numbers[i];
    }
}

int main() {
    int num_threads, num_iterations;

    std::cout << "Number of threads: ";
    std::cin >> num_threads;
    std::cout << "Number of iterations: ";
    std::cin >> num_iterations;

    std::vector<std::string> schedule_types = { "static", "dynamic", "guided" };
    std::ofstream results_file("task/results.csv", std::ios::app);

    // Проверка, удалось ли открыть файл
    if (!results_file.is_open()) {
        std::cerr << "Can't open file!" << std::endl;
        return 1;
    }

    // Если файл только что создан (пустой), добавим заголовок
    results_file.seekp(0, std::ios::end);
    if (results_file.tellp() == 0) {
        results_file << "num_threads;num_iterations;mode;time\n";
    }

    // Запуск экспериментов для каждого режима распределения итераций
    for (const auto& mode : schedule_types) {
        omp_sched_t schedule_type;
        if (mode == "static") schedule_type = omp_sched_static;
        else if (mode == "dynamic") schedule_type = omp_sched_dynamic;
        else if (mode == "guided") schedule_type = omp_sched_guided;

        // Устанавливаем режим распределения итераций
        omp_set_schedule(schedule_type, 1);
        omp_set_num_threads(num_threads);

        auto start = std::chrono::high_resolution_clock::now();

        // Параллельный цикл с неравномерной нагрузкой
#pragma omp parallel for schedule(runtime)
        for (int j = 0; j < num_iterations; ++j) {
            perform_task(j);
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;

        // Запись результата в файл
        results_file << num_threads << ";" << num_iterations << ";" << mode << ";" << elapsed.count() << "\n";

        std::cout << mode << " mode took " << elapsed.count() << " seconds" << std::endl;
    }

    results_file.close();
    return 0;
}
