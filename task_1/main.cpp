#include <iostream>
#include <vector>
#include <cmath>
#include <string.h>

// #define USE_DOUBLE

template <typename T>
void count_sum(T* sum){
    float pi = 3.1415926535;
    T step = 2 * pi / 1e7;
    std::vector<T> vec(1e7);

    for(int i = 0; i < vec.size(); i++){
        vec[i] = sin(step * i);
        *sum += vec[i];
    }
}


int main(int argc, const char** argv){
    #ifdef USE_DOUBLE
        std::cout << "Using double\n";
        double sum = 0;
    #endif
    #ifndef USE_DOUBLE
        std::cout << "Using float\n";
        float sum = 0;
    #endif

    count_sum(&sum);
    std::cout << "Sum = " << sum << std::endl;
    return 0;
}