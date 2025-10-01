#ifndef __SIMPUBLI_ZLIB_H__
#define __SIMPUBLI_ZLIB_H__

#include <zlib.h>
#include <minizip/unzip.h>
#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>
#include <filesystem>

#define READ_SIZE 8192

class SimpubliZlibError : public std::runtime_error {
public:
	SimpubliZlibError(std::string err) : runtime_error(err) {};
	~SimpubliZlibError() {};
};

class SimpubliZlib {
public:
	SimpubliZlib() {};
	~SimpubliZlib() {};
	static bool UnzipFile(std::string& file_path_zip, std::string& file_path_csv);
};

#endif // !__SIMPUBLI_ZLIB_H__
