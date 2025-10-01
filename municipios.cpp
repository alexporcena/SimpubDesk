#include <string>
#include <vector>
#include <iostream>
#include <chrono>
#include <mariadb/conncpp.hpp>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include "simpubli_strings.h"
#include "municipios.h"

using json = nlohmann::json;


static size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t realsize = size * nmemb;
    ((std::string*)userp)->append((char*)contents, realsize);
    return realsize;
}

std::string performApiRequest() {
	std::cout << "Iniciando requisição com a API do IBGE..." << std::endl;
	std::string ret;

    CURL* curl;
    CURLcode res;

    std::string readBuffer;

    curl_global_init(CURL_GLOBAL_DEFAULT);

    curl = curl_easy_init();

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "https://servicodados.ibge.gov.br/api/v3/agregados/6579/periodos/2025/variaveis/9324?localidades=N6[N3[35]]");

		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() falhou: " << curl_easy_strerror(res) << std::endl;
        } else {
            //std::cout << "Conteúdo JSON recebido:\n" << readBuffer << std::endl;
			ret = readBuffer;
        }
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();

    return ret;
}


std::vector<Municipio> parseJsonApi(std::string& jsn) {
	std::vector<Municipio> municipios;
	try
	{
		std::cout << "Parse JSON..." << std::endl;
		json dados_json = json::parse(jsn);

		for (const auto& item : dados_json)
		{
			if (item.contains("resultados") && item["resultados"].is_array())
			{
				for (const auto& resultado : item["resultados"])
				{
					if (resultado.contains("series") && resultado["series"].is_array())
					{
						for (const auto& serie : resultado["series"])
						{
							// Acessa o nome e ID da localidade
							std::string nome_localidade = serie["localidade"]["nome"].get<std::string>();
							SimpubliString::ReplaceAll(nome_localidade, " - SP", "");
							std::string id_localidade = serie["localidade"]["id"].get<std::string>();
							std::string populacao = serie["serie"]["2025"].get<std::string>();
							std::string nova_localidade = nome_localidade;
							
							nova_localidade = SimpubliString::RemoveDiacritcs(nome_localidade);
							SimpubliString::ReplaceAll(nova_localidade, " ", "-");
							SimpubliString::ReplaceAll(nova_localidade, "'", "-");
							nova_localidade = SimpubliString::ToLower(nova_localidade);
							std::cout << nova_localidade << std::endl;
							
							Municipio municipio;
							municipio.id_ibge = std::stoi(id_localidade);
							municipio.nome = nome_localidade;
							municipio.nome_audesp = nova_localidade;
							municipio.uf = "SP";
							municipio.populacao = std::stoi(populacao);
							municipios.push_back(municipio);
							
						}
					}
				}
			}
		}
	}
	catch (const json::parse_error& e)
	{
		std::cerr << "Erro ao analisar JSON: " << e.what() << std::endl;
		return municipios;
	}

	return municipios;
}

void Save(std::vector<Municipio>& municipios, std::vector<Municipio>& municipios_api)
{
	std::vector<Municipio> edit, insert;

	bool found = false;

	for (Municipio municipio_api : municipios_api) {
		for (Municipio municipio : municipios) {
			if (!found) {
				if (municipio_api.id_ibge == municipio.id_ibge) {
					found = true;
					if (municipio_api.populacao != municipio.populacao) {
						edit.push_back(municipio_api);
					}
				}
			}
		}
		if (!found) {
			insert.push_back(municipio_api);
			found = false;
		}
	}

	int i = inserirNMunicipios(insert);
	std::cout << i << " registros inseridos no Banco de Dados" << std::endl;
	int e = editarNMunicipios(edit);
	std::cout << e << " registros editados no Banco de Dados" << std::endl;
}

void  executar() {
	std::vector<Municipio> municipios = listarMunicipios();
	std::cout << "Iniciando o processo, por favor aguarde..." << std::endl;
	auto start = std::chrono::high_resolution_clock::now();
    std::string js = performApiRequest();
    std::vector<Municipio> municipios_api = parseJsonApi(js);
	Save(municipios, municipios_api);
	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	double a = duration.count() / 1000.00;
	std::cout << "Concluído em " << a << " segundos" << std::endl;
}
