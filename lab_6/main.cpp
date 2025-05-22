#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>
#include <fstream>
#include <cassert>
#include <openacc.h>
// #include <boost/program_options.hpp>

#define IND(i, j) ((i) * size + (j))
#define max(a, b) ((a) > (b)?(a):(b))

// namespace bpo = boost::program_options;

int get_block_size(int n, int rank, int nprocs){
    int s = n / nprocs;
    if(n % nprocs > rank)
        s++;
    return s;
}

int init(std::vector<double> &A, int size){
    int left_top = 10, right_top = 20, left_bottom = 30, right_bottom = 20;
    for(int i = 0; i < size; i++){
        A[IND(0, i)]        = left_top + (right_top - left_top) * i / (size - 1); // top border
        A[IND(size - 1, i)] = left_bottom + (right_bottom - left_bottom) * i / (size - 1); // bottom border
        A[IND(i, 0)]        = left_top + (left_bottom - left_top) * i / (size - 1); // left border
        A[IND(i, size - 1)] = right_top + (right_bottom - right_top) * i / (size - 1); // right border
    }
    return 0;
}

std::vector<double> calculate_next(std::vector<double> A, int size){
    std::vector<double> new_A(A);

    for(int i = 1; i < size - 1; i++){
        for(int j = 1; j < size - 1; j++){
            new_A[IND(i, j)] = (A[IND(i - 1, j)] + A[IND(i, j - 1)] + A[IND(i, j + 1)] + A[IND(i + 1, j)]) * 0.25;
            // std::cout << "new A[" << i << ", " << j << "] = " << new_A[IND(i, j)] << std::endl;
        }
    }
    return new_A;
}

double calculate_error(std::vector<double> A, std::vector<double> new_A, int size){
    double maxdiff = 0;

    for(int i = 1; i < size - 1; i++){
        for(int j = 1; j < size - 1; j++){
            double diff = new_A[IND(i, j)] - A[IND(i, j)];
            // std::cout << "Diff = " << diff << std::endl;
            maxdiff = max(maxdiff, diff);
        }
    }

    return maxdiff;
}

int solve(std::vector<double> &A, int size, double eps, int num_iters){
    double maxdiff = 1; // initialize with any value that greater or equal eps
    int iter = 0;

    do{
        std::vector<double> new_A = calculate_next(A, size);
        if(iter % 5000 == 0)
            maxdiff = calculate_error(A, new_A, size);
        A.swap(new_A);
        // std::cout << "Iter " << iter << ", maxdiff = " << maxdiff << std::endl;
        iter++;
    } while(iter <= num_iters && maxdiff >= eps);

    std::cout << "Finished in " << iter << " iterations with error " << maxdiff << std::endl;
    return iter;
}

int save_to_file(std::vector<double> A, int size, std::string filename){
    std::ofstream f(filename);

    if(!f.is_open()){
        std::cerr << "Error while opening a file " << filename << std::endl;
        return 1;
    }

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            f << A[IND(i, j)] << " ";
        }
        f << std::endl;
    }
    f.close();
    return 0;
}

int main(int argc, const char** argv){
    // bpo::options_descriptions desc("Available options");

    // desc.add_options()
    //     ("help,h", "Show help")
    //     ("size", bpo::value<int>->default_value(128), "Grid size")
    //     ("num_iters", bpo::value<int>->default_value(1e6), "Max number of iterations")
    //     ("eps", bpo::value<double>->default_value(1e-3), "Max error to stop")

    // bpo::variables_map vmap;
    // bpo::store(bpo::parse_command_line(argc, argv, desc), vmap);
    // bpo::notify(vm);

    // if(vm.count("help")){
    //     std::cout << desc << "\n";
    //     return 0;
    // }

    // int size = vm["N"].as<int>();
    // int num_iters = vm["num_iters"].as<int>();
    // double eps = vm["eps"].as<double>();
    int size = 512;
    int num_iters = 1e6;
    double eps = 1e-6;
    
    assert(size >= 3 && num_iters >= 1 && eps >= 0 && "Wrong parameters specified\n");

    std::vector<double> A(size * size);

    init(A, size);

    auto start = std::chrono::high_resolution_clock::now();

    int iters = solve(A, size, eps, num_iters);
    
    auto end = std::chrono::high_resolution_clock::now();
    double time = std::chrono::duration<double>(end - start).count();

    std::cout << "Elapsed time (sec): " << time << std::endl;
    save_to_file(A, size, "output.txt");
}