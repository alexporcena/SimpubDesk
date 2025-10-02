#ifndef __SIMPUBLI_JSON_CREATE_H__
#define __SIMPUBLI_JSON_CREATE_H__

#include <string>
#include <vector>
#include <iostream>
#include <mariadb/conncpp.hpp>
#include "mariadb_api_municipios.h"

//Dados dos municipios
std::vector<Municipio> populacao();

//Arrecadação por mês
bool arrecadacaoMensal(std::vector<Municipio>& municipios, int next_mun, int last_positioon);

//Arrecadação por fonte de recurso
bool arrecadacaoFonteRecurso(std::vector<Municipio>& municipios, int next_mun, int last_positioon);

//Arrecadação por tipo
bool arrecadacaoTipoRecurso(std::vector<Municipio>& municipios, int next_mun, int last_positioon);

//Arrecadação por fonte contábil
bool arrecadacaoFonteContabil(std::vector<Municipio>& municipios, int next_mun, int last_positioon);


#endif __SIMPUBLI_JSON_CREATE_H__
