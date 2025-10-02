#ifndef __MUNCIPIO_H__
#define __MUNCIPIO_H__

//Padrão
#include <string>
#include <vector>
#include <iostream>

//Terceiros
#include <mariadb/conncpp.hpp>

//Projeto
struct Data_Municipio {
	int id_ibge;
	std::string nome;
	std::string nome_audesp;
	std::string uf;
	int populacao;
};

class Municipio {
public:
private:
protected:
};

#endif
