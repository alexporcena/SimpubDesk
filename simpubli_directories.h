#ifndef __SIMPUBLI_DERECTORIES_H__
#define __SIMPUBLI_DERECTORIES_H__

#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>

class SimpubliDirectoryError : public std::runtime_error {
public:
	SimpubliDirectoryError(std::string err) : runtime_error(err) {};
};

class SimpubliDirectory {
public:
	SimpubliDirectory() {}
	~SimpubliDirectory() {}

	static std::string GetHomeUserDirectory();
	static bool DirectoryExistsW(std::string& dir);
	static bool CreateDirectoryW(std::string& dir);
	static bool RemoveDirectoryW(std::string& dir);
	static std::vector<std::string> GetListFiles(std::string& dir, std::string ext);
	static std::vector<std::string> GetAllFiles(std::string& dir);
	static std::vector<std::string> GetAllFilesFullPath(std::string& dir);
	static void RemoveAllFiles(std::string& dir);
};

#endif __SIMPUBLI_DERECTORIES_H__