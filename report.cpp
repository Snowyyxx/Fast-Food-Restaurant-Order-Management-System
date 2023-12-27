#include"database.h"


int main()

{

    std::thread display_store_A_thread(display_query_store_A);
    std::thread display_store_B_thread(display_query_store_B);
    display_store_A_thread.join();
    display_store_B_thread.join();
}