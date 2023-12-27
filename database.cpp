#include <iostream>
#include <sqlite3.h>
#include <string>

void insert_query(int order_id, int store_id, bool burger, bool fries, bool coke, const std::string& status, const std::string& date) {
    sqlite3* db;
    sqlite3_stmt* stmt;
    int rc;

    // Open database
    rc = sqlite3_open("organization.db", &db);
    if (rc != SQLITE_OK) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    }

    // SQL INSERT statement with placeholders
    const char* sql = "INSERT INTO order_table (order_id, store_id, burger, fries, coke, status, date) VALUES (?, ?, ?, ?, ?, ?, ?);";

    // Prepare SQL statement
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    }

    // Bind parameters to the prepared statement
    sqlite3_bind_int(stmt, 1, order_id);
    sqlite3_bind_int(stmt, 2, store_id);
    sqlite3_bind_int(stmt, 3, burger ? 1 : 0); // SQLite does not have a native boolean type, so integers 0 (false) and 1 (true) are used
    sqlite3_bind_int(stmt, 4, fries ? 1 : 0);
    sqlite3_bind_int(stmt, 5, coke ? 1 : 0);
    sqlite3_bind_text(stmt, 6, status.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 7, date.c_str(), -1, SQLITE_TRANSIENT);

    // Execute the statement
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Insertion failed: " << sqlite3_errmsg(db) << std::endl;
    }

    // Finalize the statement to prevent resource leaks
    sqlite3_finalize(stmt);

    // Close database
    sqlite3_close(db);
}

void update_query(int order_id, std::string status){
    sqlite3* db;
    sqlite3_stmt* stmt;
    int rc;

    // Open database
    rc = sqlite3_open("organization.db", &db);
    if (rc != SQLITE_OK) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    }

    const char* sql = "UPDATE order_table SET status = ? where order_id =?";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    }

    sqlite3_bind_text(stmt, 1, status.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, order_id);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Insertion failed: " << sqlite3_errmsg(db) << std::endl;
    }
    // Finalize the statement to prevent resource leaks
    sqlite3_finalize(stmt);
    // Close database
    sqlite3_close(db);
}
