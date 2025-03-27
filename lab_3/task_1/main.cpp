#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <cmath>

#define tau 0.00016
#define eps 1e-4

std::mutex mtx;

void update_x(std::vector<double> &A, std::vector<double> &x, std::vector<double> &b, int lb, int ub){
    int n = x.size();

    for (int i = lb; i <= ub; i++) {
        double sum = 0;
        for (int j = 0; j < n; j++) {
            sum += A[i * n + j] * x[j];
        }
        x[i] -= tau * (sum - b[i]);
    }
}

void compute_criterion(std::vector<double> &A, std::vector<double> &x, std::vector<double> &b, int lb, int ub, double& num, double& denum){
    int n = x.size();

    double local_num = 0, local_denum = 0;

    for (int i = lb; i <= ub; i++) {
        double sum = 0;
        for (int j = 0; j < n; j++) {
            sum += A[i * n + j] * x[j];
        }
        local_num += (sum - b[i]) * (sum - b[i]);
        local_denum += b[i] * b[i];
    }

    std::lock_guard<std::mutex> lock(mtx);
    num += local_num;
    denum += local_denum;
}

int solve(int n, int num_threads, std::vector<double> &A, std::vector<double> &x, std::vector<double> &b){
    int num_iters = 0;

    double critetion;

    

    do{
        double num = 0, denum = 0;
        std::vector<std::thread> threads;

        // parallel update x
        int items_per_thread = n / num_threads;
        for(int i = 0; i < num_threads; i++){
            int lb = i * items_per_thread;
            int ub = (i == num_threads - 1) ? (n - 1) : (lb + items_per_thread - 1);
            threads.emplace_back(update_x, std::ref(A), std::ref(x), std::ref(b), lb, ub);
        }

        for(auto &t: threads)
            t.join();

        threads.clear();

        // parallel criterion
        for(int i = 0; i < num_threads; i++){
            int lb = i * items_per_thread;
            int ub = (i == num_threads - 1) ? (n - 1) : (lb + items_per_thread - 1);
            threads.emplace_back(compute_criterion, std::ref(A), std::ref(x), std::ref(b), lb, ub, std::ref(num), std::ref(denum));
        }

        for(auto &t: threads)
            t.join();

        critetion = sqrt(num) / sqrt(denum);
        num_iters++;
        // std::cout << "Iter " << num_iters << ", criterion: " << critetion << std::endl;
    } while(critetion > eps);

    return 0;
}

int main(const int argc, const char** argv){
    int n;
    int num_threads;

    if(argc == 3){
        num_threads = atoi(argv[1]);
        n = atoi(argv[2]);

        std::vector<double> A(n * n, 0);
        std::vector<double> x(n, 0);
        std::vector<double> b(n, 1);

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                A[i * n + j] = (i == j) ? 2 : 0.1;
            }
        }
    
        auto start = std::chrono::system_clock::now();
        solve(n, num_threads, A, x, b);
        auto end = std::chrono::system_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << elapsed.count() << std::endl;
    }
    return 0;
}