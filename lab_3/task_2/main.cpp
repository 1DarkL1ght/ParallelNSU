#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <cmath>
#include <queue>
#include <functional>
#include <future>

template <typename T>
class Server{
public:
    Server() : is_running(false){};

    void start(){
        this->is_running = true;
        thread_process = std::thread(Server::process_tasks, this);
    }

    void stop(){
        this->is_running = false;
        this->thread_process.join();
    }

    template <typename Func, typename... Args>
    int add_task(Func&& func, Args&&... args){
        using TaskType = std::packaged_task<T()>;

        auto task = std::make_shared<TaskType>(std::bind(std::forward<Func>(func), std::forward<Args>(args)...));
        std::future<T> result = task->get_future();
        
    }
    T request_result(int id_res){

    }
private:
    void process_tasks(){

    }

    bool is_running;
    std::thread thread_process;
    std::queue<std::function<T()>> tasks;
};

int main(){

    return 0;
}