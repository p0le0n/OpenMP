#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <omp.h>
#include <iomanip>

using namespace std;
using namespace chrono;

int main() {
    // Имя файла с векторами
    string vectors_filename = "task/vectors.txt";
    // Имя файла с результатами
    string results_filename = "task/results.csv";

    // Чтение векторов из файла
    ifstream vectors_file(vectors_filename);
    if (!vectors_file.is_open()) {
        cerr << "Error: can't open file with vectors!" << endl;
        return 1;
    }

    int n, m;
    vectors_file >> n >> m;

    vector<vector<double>> vectors(n, vector<double>(m));

    auto start_time = high_resolution_clock::now();

#pragma omp parallel sections
    {
#pragma omp section
        {
            // Первая секция чтения
            for (int i = 0; i < n / 2; ++i) {
                for (int j = 0; j < m; ++j) {
                    vectors_file >> vectors[i][j];
                }
            }
        }

#pragma omp section
        {
            // Вторая секция чтения
            for (int i = n / 2; i < n; ++i) {
                for (int j = 0; j < m; ++j) {
                    vectors_file >> vectors[i][j];
                }
            }
        }

#pragma omp section
        {
            // Вычисление скалярного произведения
            vector<double> dot_products(n - 1);
            for (int i = 0; i < n - 1; ++i) {
                double dot_product = 0.0;
                for (int j = 0; j < m; ++j) {
                    dot_product += vectors[i][j] * vectors[i + 1][j];
                }
                dot_products[i] = dot_product;
            }
        }
    }

    vectors_file.close();
    auto end_time = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end_time - start_time);
    double duration_sec = duration.count() * 1e-6;

    // Запись результатов в файл
    ofstream results_file(results_filename, ios::app);
    if (!results_file.is_open()) {
        cerr << "Error: can't open file with results!" << endl;
        return 1;
    }

    results_file << n << ";" << m << ";" << fixed << setprecision(6) << duration_sec << endl;
    results_file.close();

    cout << "Results are saved in " << results_filename << endl;

    return 0;
}