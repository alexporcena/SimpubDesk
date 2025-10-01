#include "simpubli_database.h"

std::unique_ptr<sql::Connection> SimpubliDatabase::Connect(std::string& url, std::string& user, std::string password)
{
    sql::Driver* driver = sql::mariadb::get_driver_instance();

    try {
        std::unique_ptr<sql::Connection> connection(driver->connect(url, user, password));
        return connection;
    }
    catch (sql::SQLException& e) {
        return nullptr;
    }
}
