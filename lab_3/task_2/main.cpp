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
        task_queue.push({task_id, std::bind(std::forward<Func>(func), std::forward<Args>(args)...)});
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

template<typename T>
void client(Server<T>& server, std::function<T()> task, std::string filename){
    size_t task_id = server.add_task(task);
    std::ofstream file(filename, std::ios::app);
    file << "Id: " << task_id << ", result = " << server.request_result(task_id) << "\n";
    file.close();
}

template<typename T>
T fun_square(T x){
    return x * x;
}

template <typename T>
T fun_sin(T x){
    return sin(x);
}

template <typename T>
T fun_sqrt(T x){
    return sqrt(x);
}

template<typename T>
T fun_pow(T x, T y){
    return pow(x, y);
}

int main(){
    Server<double> server;
    server.start();

    std::vector<std::thread> threads;

    auto start = std::chrono::system_clock::now();

    for(int i = 0; i < 2000; i++){
        // std::thread client1(client<double>, std::ref(server), std::bind(fun_pow<double>, 2, 2), "pows.txt");
        threads.emplace_back(client<double>, std::ref(server), std::bind(fun_pow<double>, 2, 2), "pows.txt");
    }
    
    for(int i = 0; i < 2000; i++){
        threads.emplace_back(client<double>, std::ref(server), std::bind(fun_sqrt<double>, 3), "sqrts.txt");
    }

    for(int i = 0; i < 2000; i++){
        threads.emplace_back(client<double>, std::ref(server), std::bind(fun_sin<double>, 30), "sins.txt");
    }

    for(int i = 0; i < 2000; i++){
        threads.emplace_back(client<double>, std::ref(server), std::bind(fun_square<double>, 5), "squares.txt");
    }


    for(auto& t:threads)
        t.join();

    server.stop();
    
    auto end = std::chrono::system_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << elapsed.count() << std::endl;

    return 0;
}