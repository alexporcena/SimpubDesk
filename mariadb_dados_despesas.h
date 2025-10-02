#ifndef __MARIADB_DADOS_DESPESAS_H__
#define __MARIADB_DADOS_DESPESAS_H__

#include "mariadb_api_municipios.h"
#include <filesystem>

struct ReceitasAudesp {
	int id;
	int id_ibge;
	int nome_audesp;
	int id_despesa_detalhe;
	int ano_exercicio;
	std::string ds_municipio;
	std::string	ds_orgao;
	int mes_referencia;
	std::string mes_ref_extenso;
	std::string tp_despesa;
	std::string nr_empenho;
	std::string identificador_despesa;
	std::string dt_emissao_despesa;
	std::string ds_despesa;
	double vl_despesa;
	std::string ds_funcao_governo;
	std::string ds_subfuncao_governo;
	std::string cd_programa;
	std::string ds_programa;
	std::string cd_acao;
	std::string ds_acao;
	std::string ds_fonte_recurso;
	std::string ds_cd_aplicacao_fixo;
	std::string ds_modalidade_lic;
	std::string ds_elemento;
};

struct ConferenciaDespesasAudesp
{
	int max_id_despesa;
	std::string nome_audesp;
};

int quantidadeDespesaAudesp(const int id_ibge);
int inserirNDespesaAudesp(std::vector<Municipio>& municipios);
std::vector<ConferenciaDespesasAudesp> selecionar_conferencia_despesas();

#endif  //__MARIADB_DADOS_DESPESAS_H__
