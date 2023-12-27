#include"employee.h"


int main()

{
    // Initialize threads
    std:: string store_name = "B";
    std::thread placing_order_thread(place_order,store_name, false, true, true);
    manager manager_obj;
    std::thread manager_thread(&manager::motivate, &manager_obj);
    std::thread display_thread(display_completed_order);
    std::thread manager_add_cook(&manager::add_cook, &manager_obj);
    // Wait for user input or some condition to stop the simulation
    std::cin.get(); // For example, wait for enter key
    running = false; // Signal all threads to stop

    // Join threads
    manager_add_cook.join();
    placing_order_thread.join();
    manager_thread.join();
    display_thread.join(); // Now joining instead of detaching
    // Clean up and close
    return 0;
}