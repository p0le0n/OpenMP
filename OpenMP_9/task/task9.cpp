#include <iostream>
#include <omp.h>
#include <vector>
#include <limits>
#include <fstream>
#include <cstdlib>
#include <ctime>

// ������� ��� ��������� ������� �������
void generateMatrix(std::vector<std::vector<int>>& matrix, int rows_cols) {
    srand(time(0)); // ������������� ���������� ��������� �����
    for (int i = 0; i < rows_cols; ++i) {
        for (int j = 0; j < rows_cols; ++j) {
            matrix[i][j] = rand() % 1000; // ��������� ����� �� 0 �� 999
        }
    }
}

int main() {
    const int rows_cols = 1000;
    std::vector<std::vector<int>> matrix(rows_cols, std::vector<int>(rows_cols));
    generateMatrix(matrix, rows_cols);

    std::vector<int> min_in_rows(rows_cols, std::numeric_limits<int>::max());
    double not_nested_time, nested_time;

    // ����� ������� ��� ��������� ��� ���������� ������������
    double start_time = omp_get_wtime();
#pragma omp parallel for
    for (int i = 0; i < rows_cols; i++) {
        for (int j = 0; j < rows_cols; j++) {
            if (matrix[i][j] < min_in_rows[i]) {
#pragma omp critical
                min_in_rows[i] = matrix[i][j];
            }
        }
    }

    int max_value_not_nested = std::numeric_limits<int>::min();
#pragma omp parallel for reduction(max:max_value_not_nested)
    for (int i = 0; i < rows_cols; i++) {
        if (min_in_rows[i] > max_value_not_nested) {
            max_value_not_nested = min_in_rows[i];
        }
    }
    not_nested_time = omp_get_wtime() - start_time;

    // ������� ����������� ��� ������ ������
    std::fill(min_in_rows.begin(), min_in_rows.end(), std::numeric_limits<int>::max());

    // ����� ������� ��� ��������� � ��������� �������������
    omp_set_max_active_levels(1); // �������� ��������� �����������
    start_time = omp_get_wtime();
#pragma omp parallel for
    for (int i = 0; i < rows_cols; i++) {
#pragma omp parallel for
        for (int j = 0; j < rows_cols; j++) {
            if (matrix[i][j] < min_in_rows[i]) {
#pragma omp critical
                min_in_rows[i] = matrix[i][j];
            }
        }
    }

    int max_value_nested = std::numeric_limits<int>::min();
#pragma omp parallel for reduction(max:max_value_nested)
    for (int i = 0; i < rows_cols; i++) {
        if (min_in_rows[i] > max_value_nested) {
            max_value_nested = min_in_rows[i];
        }
    }
    nested_time = omp_get_wtime() - start_time;

    // ������ ������������
    double coef = not_nested_time / nested_time;

    // ������ ����������� � ����
    std::ofstream results_file("task/results.csv", std::ios_base::app);
    if (results_file.is_open()) {
        results_file << not_nested_time << ";" << nested_time << ";" << coef << ";" << rows_cols << "\n";
        results_file.close();
    }
    else {
        std::cerr << "Error occured while opening results.csv" << std::endl;
    }

    std::cout << "Results are saved in results.csv" << std::endl;
    return 0;
}
