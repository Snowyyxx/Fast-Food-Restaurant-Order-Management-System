#include "header.h"

 int cook_time_burger = 10;
 int cook_time_fries = 5;
 int cook_time_coke = 1;

class cook {
public:
    std::string name;
    cook(std::string name) : name(name) {}

    void cook_food() {
        while (true) {
            std::unique_lock<std::mutex> lock(order_queue_mutex);
            if (!order_queue.empty()) {
                auto current_meal = order_queue.front();
                order_queue.pop();
                lock.unlock(); // Unlock before potentially long operations
                cook_order(current_meal);
                display_stack.push(current_meal);
            } else {
                lock.unlock();
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
        }
    }

private:
    void cook_order(order& meal) {
        std::cout << RED << "Order received with order number: " << meal.order_number << std::endl;
        std::unique_lock<std::mutex>db_lock (db_mutex);
        update_query(meal.order_number,"Cooking");
        db_lock.unlock();
        if (meal.burger) {
            std::this_thread::sleep_for(std::chrono::seconds(cook_time_burger));
            //std::cout << "Burger cooked for order number: " << meal.order_number << std::endl;
        }
        if (meal.coke) {
            std::this_thread::sleep_for(std::chrono::seconds(cook_time_coke));
            //std::cout << "Coke ready for order number: " << meal.order_number << std::endl;
        }
        if (meal.french_fries) {
            std::this_thread::sleep_for(std::chrono::seconds(cook_time_fries));
            //std::cout << "French Fries cooked for order number: " << meal.order_number << std::endl;
        }
        meal.finished = true;
        std::cout << "Cook: " << name << " has prepared meal with order number: " << meal.order_number << ", kindly collect!" << std::endl;
        db_lock.lock();
        update_query(meal.order_number,"Ready");
        db_lock.unlock();
    }
};

class manager {
public:
    std::atomic<bool>add_cooks_flag;
    manager(){
        std::atomic<bool>add_cooks_flag(true);
    }
    void motivate() {
        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(20));
            if(order_queue.size() > 8) {
                std::cout << YELLOW << "Manager: Hurry Up! " << order_queue.size() << " orders are remaining!" << std::endl;
                std::cout<<YELLOW<<"Order speeds shall be increased!"<<std::endl;
                cook_time_burger/=2;
                cook_time_fries/=2;
                cook_time_coke/=2;
            }
            sleep(15);
            cook_time_burger=cook_time_burger*2;
            cook_time_coke=cook_time_coke*2;
            cook_time_fries=cook_time_fries*2;
        }
    }

    void add_cook() {
        while(true){
            while (add_cooks_flag) {
                std::this_thread::sleep_for(std::chrono::seconds(10));
                int number_of_cooks_needed = order_queue.size() / 5;
                number_of_cooks_needed = std::min(number_of_cooks_needed, 100);
                std::cout<<YELLOW<<"Adding more cooks! Number of cooks added:"<<number_of_cooks_needed<<std::endl;
                for (int i = 0; i < number_of_cooks_needed; i++) {
                    std::string str = "cook_" + std::to_string(cook_thread_vector.size());
                    cook_thread_vector.emplace_back(&cook::cook_food, cook(str));
                }
                add_cooks_flag=false;
            }
            while(add_cooks_flag==false){
                if(order_queue.size()/4 > cook_thread_vector.size()){
                    add_cooks_flag=true;
                    std::cout<<YELLOW<<"Number of cooks less than 25\% of Queue size!"<<std::endl;
                    add_cook();
                }
            }
        }
    }
};
