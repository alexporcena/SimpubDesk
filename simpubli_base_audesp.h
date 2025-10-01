#ifndef __SIMPUBLI_BASE_AUDESP_H__
#define __SIMPUBLI_BASE_AUDESP_H__

#include <string>
#include <vector>
#include <iostream>
#include <chrono>
#include <thread>
#include <stdexcept>

#include "simpubli_strings.h"
#include "simpubli_directories.h"
#include "simpubli_files.h"
#include "simpubli_zlib.h"
#include "mariadb_api_municipios.h"

#include <curl/curl.h>

class SimpubliBaseAudespError : public std::runtime_error {

public:
	SimpubliBaseAudespError(std::string& err) : std::runtime_error(err) {};
	~SimpubliBaseAudespError() {}
};

class SimpubliBaseAudesp {
public:
	SimpubliBaseAudesp();
	~SimpubliBaseAudesp();

	bool ExecutAudesp();

protected:

	struct DownloadInfo {
		std::string url;
		std::string file_name;
	};

	static size_t WriteCallback(void* ptr, size_t size, size_t nmemb, void* stream);
	std::vector<DownloadInfo> PrepareRequest();
	static int PerformRequest(const DownloadInfo& info);
	void ExecuteCurlRequestThread(std::vector<DownloadInfo>& info);
	void UnzipFiles();
	virtual void InsertDatabase() {};

	std::vector<Municipio> municipios;
	std::vector<Municipio> edit_municipios;
	std::string audesp_type; //in receitas | despesas 
};

#endif