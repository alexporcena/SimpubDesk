#ifndef __SIMPUBLI_DATABASE_H__
#define __SIMPUBLI_DATABASE_H__

#include <mariadb/conncpp.hpp>
#include <iostream>
#include <memory>

class SimpubliDatabase {
public:
	SimpubliDatabase() {};
	~SimpubliDatabase() {};

	std::unique_ptr<sql::Connection> Connect(std::string& url, std::string& user, std::string password);

};

#endif //__SIMPUBLI_DATABASE_H__
