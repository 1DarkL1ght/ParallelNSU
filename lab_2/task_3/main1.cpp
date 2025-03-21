#include <iostream>
#include <vector>
#include <cmath>
#include <omp.h>

#define eps 1e-5

double tau = 1e-2;
int nthreads = 1;

/*
    PRAGMA OMP PARALLEL verion
*/


// Matrix-vector production
std::vector<double> matrix_vector_prod(std::vector<double> a, std::vector<double> b){
    std::vector<double> result(b.size(), 0);
    #pragma omp for
    for (int i = 0; i <= b.size(); i++) {
        for (int j = 0; j < b.size(); j++)
            result[i] += a[i * b.size() + j] * b[j];
    }
    return result;
}

// Norm of the vector
double vector_norm(std::vector<double> a){
    double result = 0;
    #pragma omp for reduction(+:result)
    for(double elem:a)
        result += elem * elem;
    return sqrt(result);
}

// Vector substraction
std::vector<double> vector_sub(std::vector<double> a, std::vector<double> b){
    std::vector<double> result(a.size(), 0);
    #pragma omp for
    for(int i = 0; i < a.size(); i++)
        result[i] = a[i] - b[i];  
    return result;
}


std::vector<double> vector_const_mul(std::vector<double> a, double t){
    std::vector<double> result(a.size(), 0);
    #pragma omp for
    for(int i = 0; i < a.size(); i++)
        result[i] = a[i] * t;  
    return result;
}


// Solver
int solve(std::vector<double> &A, std::vector<double> &b, std::vector<double> &x){
    double num, denum;
    double criterion;
    int num_iters = 0;
    #pragma omp parallel
    {
        do{
            // A * x production
            std::vector<double> Ax = matrix_vector_prod(A, x);
            
            // Ax - b substraction
            Ax = vector_sub(Ax, b);
            
            // t(Ax - b)
            std::vector<double> tAx = vector_const_mul(Ax, tau);

            // Update x
            x = vector_sub(x, tAx);

            // ||Ax - b||
            num = vector_norm(Ax);
            denum = vector_norm(b);

            criterion = num / denum;
            num_iters++;
            std::cout << "Iter: " << num_iters << ", criterion = " << criterion << std::endl;
        } while(criterion > eps);
    }
    std::cout << "\tDone in " << num_iters << "iterations, criterion = " << criterion << std::endl;
    return 0;
}


int main(const int argc, const char** argv){
    int N = 20000;
    std::vector<double> A(N * N, 0);
    std::vector<double> x(N, 1);
    std::vector<double> b(N, 0);

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i * N + j] = (i == j) ? 2.0 : 0.1;
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