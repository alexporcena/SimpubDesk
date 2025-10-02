#include "simpubli_audesp_despesas.h"
#include "mariadb_dados_despesas.h"

SimpubliAudespDespesas::SimpubliAudespDespesas()
{
	audesp_type = "despesas";
}

void SimpubliAudespDespesas::InsertDatabase() {
	inserirNDespesaAudesp(edit_municipios);
}