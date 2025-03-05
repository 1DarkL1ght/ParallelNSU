#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>


int matrix_vector_product(double *a, double *b, double *c, int m, int n, int nthreads){
    #pragma omp parallel num_threads(nthreads)
    {
        #pragma omp for
        for (int i = 0; i < m; i++) {
            c[i] = 0.0;
            for (int j = 0; j < n; j++)
                c[i] += a[i * n + j] * b[j];
        }
    }

    return 0;
}


int run_parallel(int m, int n, int nthreads){
    double *a, *b, *c;

    a = (double*)malloc(sizeof(*a) * m * n);
    b = (double*)malloc(sizeof(*b) * n);
    c = (double*)malloc(sizeof(*c) * m);

    #pragma omp parallel num_threads(nthreads)
    {
        #pragma omp parallel for
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++)
                a[i * n + j] = i + j;
            c[i] = 0.0;
        }
    }
    for (int j = 0; j < n; j++)
        b[j] = j;

    double t = omp_get_wtime();
    matrix_vector_product(a, b, c, m, n, nthreads);
    t = omp_get_wtime() - t;

    printf("%.6f", t);
    free(a);
    free(b);
    free(c);

    return 0;
}

int main(const int argc, const char** argv){
    int nthreads;
    int m = 20000, n = 20000;


    if(argc == 2){
        m = atoi(argv[1]);
        n = m;
        run_parallel(m, n, 1);
    }
    else if (argc == 3){
        nthreads = atoi(argv[1]);
        m = atoi(argv[2]);
        n = m;
        run_parallel(m, n, nthreads);
    }
        
    return 0;
}