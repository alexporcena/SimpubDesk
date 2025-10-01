#ifndef __SIMPUBLI_FILES_H__
#define __SIMPUBLI_FILES_H__

#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>
#include <fstream>
#include <filesystem>

class SimpubliFileError : public std::runtime_error {
public:
	SimpubliFileError(std::string err) : std::runtime_error(err) {};
	~SimpubliFileError() {};
};

class SimpubFile {
public:
	SimpubFile() {}
	~SimpubFile() {};
	static std::string ReadTextFile(std::string& file_path);
	static std::vector<std::string> ReadTextFileLines(std::string& file_path);
	static bool FileExists(std::string& file_path);
	static bool CreateTextFile(std::string file_path, std::string& str);
	static int GetFileSize(std::string file_path);
	static std::string FileNameFromPath(std::string& file_path);
};

#endif //__SIMPUBLI_FILES_H__