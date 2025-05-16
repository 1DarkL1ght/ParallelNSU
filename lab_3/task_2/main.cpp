#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <cmath>
#include <queue>
#include <functional>
#include <condition_variable>
#include <unordered_map>
#include <fstream>
#include <string>
#include <chrono>
#include <random>
#include <future>

template <typename T>
class Server{
public:
    using Task = std::function<T()>;

    Server(){};

    void start(){
        this->is_running = true;
        thread_process = std::thread(Server::process_tasks, this);
    }

    void stop(){
        // std::cout << "Called stop\n";
        this->is_running = false;
        cv.notify_all();
        this->thread_process.join();
    }

    template <typename Func, typename... Args>
    size_t add_task(Func&& func, Args&&... args){
        std::lock_guard<std::mutex> lock(queue_mutex);
        size_t task_id = task_counter++;
        task_queue.emplace(
            task_id,
            std::bind(
                std::forward<Func>(func),
                std::forward<Args>(args)...
            )
        );
        cv.notify_one();
        return task_id;
        
    }

    T request_result(size_t task_id){
        std::unique_lock<std::mutex> lock(result_mutex);
        result_cv.wait(lock, [&]{return results.find(task_id) != results.end();});
        return results[task_id];
    }
private:
    void process_tasks(){
        while(true){
            std::unique_lock<std::mutex> lock(queue_mutex);
            cv.wait(lock, [&]{return !task_queue.empty() || !is_running;});

            if(!is_running || task_queue.empty()){
                // std::cout << "Stopping server\n";
                break;
            }
                

            auto [task_id, task] = task_queue.front();
            task_queue.pop();
            lock.unlock();

            T result = task();

            std::lock_guard<std::mutex> res_lock(result_mutex);
            results[task_id] = result;
            result_cv.notify_all();
        }
    }

    bool is_running = false;
    std::thread thread_process;
    std::queue<std::pair<size_t, Task>> task_queue;
    std::unordered_map<size_t, T> results;
    size_t task_counter = 0;
    std::mutex queue_mutex, result_mutex;
    std::condition_variable cv, result_cv;
};

// template<typename T>
// void client(Server<T>& server, std::function<T()> task, std::string filename){
//     size_t task_id = server.add_task(task);
//     std::ofstream file(filename, std::ios::app);
//     file << "Id: " << task_id << ", result = " << server.request_result(task_id) << "\n";
//     file.close();
// }

double get_random_argument(double from, double to) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(from, to);
    return dis(gen);
}

std::pair<std::function<double()>, std::string> get_sin_task(){
    double arg = get_random_argument(0, 100);
    return {
        [arg]() { return sin(arg); },
        "sin(" + std::to_string(arg) + ")"
    };
}

std::pair<std::function<double()>, std::string> get_sqrt_task(){
    double arg = get_random_argument(0, 100);
    return {
        [arg]() { return sqrt(arg); },
        "sqrt(" + std::to_string(arg) + ")"
    };
}

std::pair<std::function<double()>, std::string> get_square_task(){
    double arg = get_random_argument(0, 100);
    return {
        [arg]() { return arg * arg; },
        "(" + std::to_string(arg) + ")^2"
    };
}

std::pair<std::function<double()>, std::string> get_pow_task(){
    double base = get_random_argument(1, 5);
    double exponent = get_random_argument(1, 5);
    return {
        [base, exponent]() { return pow(base, exponent); },
        "pow(" + std::to_string(base) + ", " + std::to_string(exponent) + ")"
    };
}

void client_job(Server<double>& server, std::function<std::pair<std::function<double()>, std::string>()> task_gen, std::string filename){
    std::ofstream file(filename, std::ios::app);
    for (int i = 0; i < 2500; i++) {
        auto [task, description] = task_gen();
        size_t id = server.add_task(task);
        double result = server.request_result(id);
        file << "Task #" << id << ": " << description << " = " << result << "\n";
    }
    file.close();

}

int main(){
    Server<double> server;
    server.start();

    std::vector<std::thread> threads;

    auto start = std::chrono::system_clock::now();

    std::thread sin_thread(client_job, std::ref(server), get_sin_task, "sin.txt");
    std::thread sqrt_thread(client_job, std::ref(server), get_sqrt_task, "sqrt.txt");
    std::thread square_thread(client_job, std::ref(server), get_square_task, "square.txt");
    std::thread pow_thread(client_job, std::ref(server), get_pow_task, "pow.txt");

    sin_thread.join();
    sqrt_thread.join();
    square_thread.join();
    pow_thread.join();

    server.stop();
    
    auto end = std::chrono::system_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << elapsed << std::endl;

    return 0;
}