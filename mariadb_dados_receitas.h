#ifndef __MARIADB_DADOS_RECEITAS_H__
#define __MARIADB_DADOS_RECEITAS_H__

#include "mariadb_api_municipios.h"
#include <filesystem>

struct ReceitasAudesp {
	int id;
	int idMunicipio;
	int id_ibge;
	int id_rec_arrec_detalhe;
	int ano_exercicio;
	std::string ds_municipio;
	std::string ds_orgao;
	int mes_referencia;
	std::string mes_ref_extenso;
	std::string ds_poder;
	std::string ds_fonte_recurso;
	std::string ds_cd_aplicacao_fixo;
	std::string ds_categoria;
	std::string ds_subcategoria;
	std::string ds_fonte;
	std::string ds_tipo;
	double vl_arrecadacao;
};

struct ConferenciaReceitasAudesp
{
	int max_id_arrecadacao;
	std::string nome_audesp;
};

int quantidadeReceitaAudesp(const int id_ibge);
int inserirNReceitasAudesp(std::vector<Municipio>& municipio);
std::vector<ConferenciaReceitasAudesp> selecionar_conferencia_receitas();

#endif  //__MARIADB_DADOS_RECEITAS_H__
