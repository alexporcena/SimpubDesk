#ifndef __SIMPUBLI_STRINGS_H__
#define __SIMPUBLI_STRINGS_H__

#include <string>
#include <vector>
#include <stdexcept>
#include <map>
#include <sstream>

class SimpublicStringError : public std::runtime_error {
public:
	SimpublicStringError(std::string err) : std::runtime_error(err) {};
	~SimpublicStringError() {};
};

class SimpubliString {
public:
	SimpubliString() {};
	~SimpubliString() {};
	
	static std::string RemoveDiacritcs(const std::string& str);
	static std::vector<std::string> Split(const std::string& str, char delimiter);
	static void ReplaceAll(std::string& in_str, std::string to_search, std::string replace_str);
	static std::string ToLower(std::string str);

	static std::string Encrypt(const std::string& str, int key);
	static std::string Decrypt(const std::string& str, int key);
	static std::string AnsiToUtf8(const std::string& ansiString, const char* sourceCharset = "windows-1252");
};

#endif // !__SIMPUBLI_STRINGS_H__

