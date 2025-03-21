#include <iostream>
#include <cmath>
#include <vector>
#include <omp.h>

#define eps 1e-5 

double tau = 1e-2;
double nthreads;

int solve(std::vector<double>& A, std::vector<double>& b, std::vector<double>& x) {
    double criterion;
    int num_iters = 0;
    double num = 0, denum = 0;
    int N = b.size();
    std::vector<double> x_upd(N, 0.0);

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
                x_upd[i] = x[i] - tau * (sum - b[i]);
            }

            // num = ||Ax - b||, denum = ||b||
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
            criterion = sqrt(num) / sqrt(denum);
            
            #pragma omp for
            for (int i = 0; i < N; i++) {
                x[i] = x_upd[i];
            }

            #pragma omp single
            {
                num_iters++;
                std::cout << "Iter: " << num_iters << ", criterion = " << criterion << std::endl;
            }
        } while (criterion >= eps);
    }

    #pragma omp single
    std::cout << "\tDone in " << num_iters << "iterations, criterion = " << criterion << std::endl;
    return 0;
}

int main(const int argc, const char** argv) {
    int n = 20000;

    std::vector<double> A(n * n, 0.0);
    std::vector<double> b(n, 1.0);
    std::vector<double> x(n, 0.0);

    // Major diagonal init
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            A[i * n + j] = (i == j) ? 2 : 0.1;
        }
    }

    if(argc == 2){
        nthreads = atoi(argv[1]);

        double start = omp_get_wtime();
        solve(A, b, x);
        double end = omp_get_wtime();

        std::cout << "Threads: " << nthreads << ", time = " << end - start << std::endl;

    }
    return 0;
}