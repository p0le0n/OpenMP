#include <omp.h>
#include <iostream>
#include <vector>
#include <fstream>

int main() {
    int num_iterations = 10000000;
    std::vector<int> data(num_iterations, 1); // ���������� ������� ���������� 1 ��� ��������

    std::ofstream results_file("task/results.csv");
    if (!results_file.is_open()) {
        std::cerr << "Can't open file!" << std::endl;
        return 1;
    }

    // ������� ��������� ����� �������
    for (int num_threads = 1; num_threads <= 10; num_threads++) {
        double start_time, end_time;

        // �������� � �������������� ��������� ��������
        int sum_atomic = 0;
        start_time = omp_get_wtime();
#pragma omp parallel for num_threads(num_threads)
        for (int i = 0; i < num_iterations; i++) {
#pragma omp atomic
            sum_atomic += data[i];
        }
        end_time = omp_get_wtime();
        results_file << num_threads << ";atomic;" << (end_time - start_time) << std::endl;

        // �������� � �������������� ����������� ������
        int sum_critical = 0;
        start_time = omp_get_wtime();
#pragma omp parallel for num_threads(num_threads)
        for (int i = 0; i < num_iterations; i++) {
#pragma omp critical
            sum_critical += data[i];
        }
        end_time = omp_get_wtime();
        results_file << num_threads << ";critical;" << (end_time - start_time) << std::endl;

        // �������� � �������������� ������
        int sum_lock = 0;
        omp_lock_t lock;
        omp_init_lock(&lock);
        start_time = omp_get_wtime();
#pragma omp parallel for num_threads(num_threads)
        for (int i = 0; i < num_iterations; i++) {
            omp_set_lock(&lock);
            sum_lock += data[i];
            omp_unset_lock(&lock);
        }
        end_time = omp_get_wtime();
        omp_destroy_lock(&lock);
        results_file << num_threads << ";lock;" << (end_time - start_time) << std::endl;

        // �������� � �������������� ����������� ��������� reduction
        int sum_reduction = 0;
        start_time = omp_get_wtime();
#pragma omp parallel for reduction(+:sum_reduction) num_threads(num_threads)
        for (int i = 0; i < num_iterations; i++) {
            sum_reduction += data[i];
        }
        end_time = omp_get_wtime();
        results_file << num_threads << ";reduction;" << (end_time - start_time) << std::endl;
    }

    results_file.close();
    std::cout << "Results are saved in task/results.csv" << std::endl;

    return 0;
}
