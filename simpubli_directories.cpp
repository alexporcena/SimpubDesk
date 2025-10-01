#include "simpubli_directories.h"
#include "simpubli_files.h"
#include <filesystem>

std::string SimpubliDirectory::GetHomeUserDirectory() {
#if defined(_WIN32)
	char* buffer = nullptr;
	size_t size = 0;
	errno_t err = _dupenv_s(&buffer, &size, "USERPROFILE");

	if (err == 0 && buffer != nullptr) {
		std::string home_dir(buffer);
		free(buffer);
		return home_dir;
	}
	else {
		throw SimpubliDirectoryError("Erro ao encontrar o diretório");
	}
#else
	if (const char* home_dir = std::getenv("HOME")) {
		return home_dir;
	}
#endif
	throw SimpubliDirectoryError("Erro ao encontrar o diretório");
}

bool SimpubliDirectory::DirectoryExistsW(std::string& dir) {
	std::filesystem::path path(dir);

	if (!std::filesystem::is_directory(path)) {
		return false;
	}

	return true;
}

bool SimpubliDirectory::CreateDirectoryW(std::string& dir) {
	std::filesystem::path path(dir);

	try {
		if (std::filesystem::create_directories(dir)) {
			return true;
		}
		else {
			throw SimpubliDirectoryError("Diretório " + dir + " já existe.");
		}
	}
	catch (const std::filesystem::filesystem_error& e) {
		throw SimpubliDirectoryError(e.what());
	}

	return true;
}

bool SimpubliDirectory::RemoveDirectoryW(std::string& dir) {
	try {
		if (std::filesystem::remove(dir)) {
			return true;
		}
		else {
			throw SimpubliDirectoryError("Diretorio não pode ser removido, verifique se ele está vazio.");
		}
	}
	catch (const std::filesystem::filesystem_error& e) {
		throw SimpubliDirectoryError(e.what());
	}

	return 0;
}

std::vector<std::string> SimpubliDirectory::GetListFiles(std::string& dir, std::string ext) {
	std::filesystem::path path(dir);
	std::vector<std::string> files;

	if (!DirectoryExistsW(dir)) {
		throw SimpubliDirectoryError("Diretorio não existe.");
		return files;
	}

	try {
		for (const auto& f : std::filesystem::directory_iterator(path)) {
			if (f.path().filename().extension() == ext) {
				files.push_back(f.path().string());
			}
		}
	}
	catch (const std::filesystem::filesystem_error& e) {
		throw SimpubliDirectoryError(e.what());
	}

	return files;
}

std::vector<std::string> SimpubliDirectory::GetAllFiles(std::string& dir) {
	std::filesystem::path path(dir);
	std::vector<std::string> files;

	if (!DirectoryExistsW(dir)) {
		throw SimpubliDirectoryError("Diretorio não existe.");
		return files;
	}

	try {
		for (const auto& f : std::filesystem::directory_iterator(path)) {
			files.push_back(f.path().filename().string());
		}
	}
	catch (const std::filesystem::filesystem_error& e) {
		throw SimpubliDirectoryError(e.what());
	}

	return files;
}

std::vector<std::string> SimpubliDirectory::GetAllFilesFullPath(std::string& dir)
{
	std::filesystem::path path(dir);
	std::vector<std::string> files;

	if (!DirectoryExistsW(dir)) {
		throw SimpubliDirectoryError("Diretorio não existe.");
		return files;
	}

	try {
		for (const auto& f : std::filesystem::directory_iterator(path)) {
			files.push_back(f.path().string());
		}
	}
	catch (const std::filesystem::filesystem_error& e) {
		throw SimpubliDirectoryError(e.what());
	}

	return files;
}

void SimpubliDirectory::RemoveAllFiles(std::string& dir)
{
	std::vector<std::string> files = GetAllFilesFullPath(dir);
	for (std::string f : files) {
		try {
			std::filesystem::remove(f);
		}
		catch (std::filesystem::filesystem_error& e) {
			std::cerr << e.what() << std::endl;
		}
	}
}
