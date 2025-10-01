#ifndef __MARIADB_API_MUNICIPIOS_H__
#define __MARIADB_API_MUNICIPIOS_H__

#include <string>
#include <vector>
#include <iostream>
#include <mariadb/conncpp.hpp>

struct Municipio {
	int id_ibge;
	std::string nome;
	std::string nome_audesp;
	std::string uf;
	int populacao;
	int last_file_size_receitas;
	int last_file_size_despesas;
};

std::vector<Municipio> listarMunicipios();
int inserirNMunicipios(std::vector<Municipio> municipios);
int editarNMunicipios(std::vector<Municipio> municipios);
int editaFileSize(int id_ibge, int file_size, std::string& audesp_type);
std::vector<int> conferirMunicipio(std::vector<Municipio>& municipios_api, std::vector<Municipio>& populacao_db);
int localizarAlteracoes(Municipio& municipios_api, std::vector<Municipio>& municipios_db);

#endif
