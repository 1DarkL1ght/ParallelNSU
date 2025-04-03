#include <iostream>
#include <cmath>
#include <vector>
#include <thread>

#define eps 1e-4

double tau = 0.00016;
double nthreads;

int solve(std::vector<double>& A, std::vector<double>& b, std::vector<double>& x) {
    double criterion;
    int num_iters = 0;
    double num = 0, denum = 0;
    int N = b.size();

    #pragma omp parallel
    {
        do {
            // Update x
            #pragma omp for
            for (int i = 0; i < N; i++) {
                double sum = 0;
                for (int j = 0; j < N; j++) {
                    sum += A[i * N + j] * x[j];
                }
                x[i] -= tau * (sum - b[i]);
            }

            // num = ||Ax - b||, denum = ||b||
            num = 0.0, denum = 0.0;
            #pragma omp for reduction(+:num, denum)
            for (int i = 0; i < N; i++) {
                double sum = 0;
                for (int j = 0; j < N; j++) {
                    sum += A[i * N + j] * x[j];
                }
                num += (sum - b[i]) * (sum - b[i]);
                denum += b[i] * b[i];
            }
            
            #pragma omp single
            {
                criterion = sqrt(num) / sqrt(denum);
                num_iters++;
                // std::cout << "Iter: " << num_iters << ", criterion = " << criterion << std::endl;
            }
        } while (criterion > eps);
    }

    // std::cout << "\tDone in " << num_iters << "iterations, criterion = " << criterion << std::endl;
    return 0;
}

int main(const int argc, const char** argv) {
    int n = 20000;

    std::vector<double> A(n * n, 0);
    std::vector<double> b(n, 1);
    std::vector<double> x(n, 0);

    if(argc == 2){
        // Major diagonal init
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                A[i * n + j] = (i == j) ? 2 : 0.1;
            }
        }

        nthreads = atoi(argv[1]);
        omp_set_num_threads(nthreads);

        double start = omp_get_wtime();
        solve(A, b, x);
        double end = omp_get_wtime();

        // std::cout << "Threads: " << nthreads << ", time = " << end - start << std::endl;
        std::cout << end - start;

    }
    return 0;
}