#include "simpubli_files.h"

std::string SimpubFile::ReadTextFile(std::string& file_path)
{
	std::string ret;
	std::ifstream input_file(file_path);
	if (!input_file.is_open()) {
		return ret;
	}

	std::string text_line;

	while (std::getline(input_file, text_line))
	{
		ret.append(text_line);
	}

	input_file.close();
	return ret;
}

std::vector<std::string> SimpubFile::ReadTextFileLines(std::string& file_path)
{
	std::vector<std::string> ret;
	std::ifstream input_file(file_path);
	if (!input_file.is_open()) {
		std::cout << "Não é possível abrir o arquivo" << std::endl;
		return ret;
	}

	std::string text_line;

	while (std::getline(input_file, text_line))
	{
		ret.push_back(text_line);
	}

	input_file.close();
	return ret;
}

bool SimpubFile::FileExists(std::string& file_path)
{
	std::filesystem::path path(file_path);
	return std::filesystem::exists(path);
}

bool SimpubFile::CreateTextFile(std::string file_path, std::string& str)
{
	std::ofstream outputFile(file_path);

	if (outputFile.is_open()) {
		outputFile.clear();
		outputFile << str;
		outputFile.close();
	}
	else {
		throw SimpubliFileError("Não foi possível abrir o arquivo.");
	}

	return true;
}

int SimpubFile::GetFileSize(std::string file_path)
{
	try {
		uintmax_t size = std::filesystem::file_size(file_path);
		return size;
	}
	catch (std::filesystem::filesystem_error const& e) {
		throw SimpubliFileError(e.what());
	}
}

std::string SimpubFile::FileNameFromPath(std::string& file_path)
{
	std::filesystem::path path(file_path);
	return path.filename().stem().string();
}

