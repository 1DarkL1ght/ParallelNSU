#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <omp.h>


double a = -4.0; /* [a, b] */
double b = 4.0;
int nsteps; /* n */

double func(double x){
    return exp(-x * x);
}

double integrate_omp(double (*func)(double), double a, double b, int n, int nthreads){
    double h = (b - a) / n;
    double sum = 0.0;
    #pragma omp parallel num_threads(nthreads)
    {
        int nthreads = omp_get_num_threads();
        int threadid = omp_get_thread_num();
        int items_per_thread = n / nthreads;
        int lb = threadid * items_per_thread;
        int ub = (threadid == nthreads - 1) ? (n - 1) : (lb + items_per_thread - 1);

        double sumloc = 0;
        
        // #pragma omp for schedule(dynamic, 100) reduction(+:sum)
        for (int i = lb; i <= ub; i++)
            sumloc += func(a + h * (i + 0.5));
        
        #pragma omp atiomic
        sum += sumloc;
    }
    sum *= h;
    // printf("Area = %.3f\n", sum);
    return sum;
}


int run_parallel(int nthreads){
    double t = omp_get_wtime();
    double res = integrate_omp(func, a, b, nsteps, nthreads);
    t = omp_get_wtime() - t;
    printf("%.6f", t);
    return 0;
}

int main(const int argc, const char** argv){
    
    if(argc == 2){
        nsteps = atoi(argv[1]);
        run_parallel(1);
    }

    else if (argc == 3){
        int nthreads = atoi(argv[1]);
        nsteps = atoi(argv[2]);
        run_parallel(nthreads);
    }
    return 0;
}