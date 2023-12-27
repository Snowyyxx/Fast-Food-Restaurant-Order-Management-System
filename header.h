#include <iostream>
#include <queue>
#include <stack>
#include <thread>
#include"database.h"
#include <vector>
#include <sqlite3.h>
#include <chrono>
#include <cstdlib>
#include<atomic>
#include <string>
#include <random>
#include <unistd.h>
#include <mutex>

class order {
public:
    static int count;
    bool burger;
    std::string store_name;
    bool french_fries;
    bool coke;
    bool finished;
    int order_number;
    order(std::string store_name,bool burger = false, bool french_fries = false, bool coke = true) {
        this->burger = burger;
        this->french_fries = french_fries;
        this->coke = coke;
        this->finished = false;
        order_number = count++;
        this->store_name=store_name;
    }
};

int order::count = 0;
std::queue<order> order_queue;
std::mutex order_queue_mutex;
std::stack<order> display_stack;
std::vector<std::thread> cook_thread_vector;
std::mutex display_mutex; // Added for synchronization in display_completed_order

constexpr int max_cooks = 30;
const std::string RED = "\033[31m";
const std::string GREEN = "\033[32m";
const std::string YELLOW = "\033[33m";
const std::string BLUE = "\033[34m";
const std::string RESET = "\033[0m";
std::string date = "27/12/2023";
std::mutex db_mutex;


std::atomic<bool> running(true); // Global flag to control the running state

void display_completed_order() {
    while (running) {
        std::unique_lock<std::mutex> lock(display_mutex);
        if (!display_stack.empty()) {
            std::cout << "Completed Order: " << display_stack.top().order_number << std::endl;
            display_stack.pop();
        }
        lock.unlock();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}



void place_order(std::string store_name,bool burger = true, bool coke = false, bool french_fries = true) {
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(1, 3);
    while (running) {
        int random_number = distribution(generator);
        std::this_thread::sleep_for(std::chrono::seconds(5));
        for (int i = 0; i < random_number; i++) {
            
            order meal(store_name,rand() % 2, rand() % 2, rand() % 2);

            std::unique_lock <std::mutex> db_lock(db_mutex);
            insert_query(meal.order_number,meal.store_name,meal.burger,meal.french_fries,meal.coke,"Received",date);
            db_lock.unlock();
            {
                std::lock_guard<std::mutex> lock(order_queue_mutex);
                order_queue.push(meal);
            }
            std::cout << GREEN << "Order Placed for ";
            meal.burger ? std::cout << "One Burger, " : std::cout << "";
            meal.french_fries ? std::cout << "One French Fries, " : std::cout << "";
            meal.coke ? std::cout << "One Coke" : std::cout << "";
            std::cout << " with order Number " << meal.order_number << std::endl;
        }
    }
}
