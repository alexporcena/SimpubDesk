#include "simpubli_audesp_receitas.h"
#include "mariadb_dados_receitas.h"

SimpubliAudespReceitas::SimpubliAudespReceitas()
{
	audesp_type = "receitas";
}

void SimpubliAudespReceitas::InsertDatabase() {
	inserirNReceitasAudesp(edit_municipios);
}