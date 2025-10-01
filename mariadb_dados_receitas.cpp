#include "mariadb_dados_receitas.h"
#include "simpubli_strings.h"
#include "simpubli_files.h"
#include "simpubli_directories.h"

std::unique_ptr<sql::Connection> conectarRec()
{
	std::string file_path = SimpubliDirectory::GetHomeUserDirectory() + "/SimPubli/data/data.conf";
	std::string data_str = SimpubFile::ReadTextFile(file_path);
	data_str = SimpubliString::Decrypt(data_str, 7);
	std::vector<std::string> data_v = SimpubliString::Split(data_str, ';');

	try {
		sql::Driver* driver = sql::mariadb::get_driver_instance();
		std::unique_ptr<sql::Connection> conn(driver->connect(data_v[0], data_v[1], data_v[2]));
		conn->setSchema("simpubli");
		return conn;
	}
	catch (sql::SQLException& e) {
		return nullptr;
	}
}

int quantidadeReceitaAudesp(const int id_ibge)
{
	std::unique_ptr<sql::Connection> conn = conectarRec();
	int r = 0;
	try {
		std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("SELECT count(ID) AS REGISTROS FROM receitas_audesp WHERE id_ibge = ?"));
		pstmt->setInt(1, id_ibge);
		std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

		while (res->next()) {
			r = res->getInt("REGISTROS");
		}
	}
	catch (sql::SQLException& e) {
		std::cerr << "Erro ao pesquisar: " << e.what() << std::endl;
	}

	return r;
}

int inserirReceitaAudesp(const ReceitasAudesp & receita)
{
	std::unique_ptr<sql::Connection> conn = conectarRec();
	int linhasAfetadas;
	try {
		std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("INSERT INTO receitas_audesp VALUES (NULL, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)"));

		pstmt->setInt(1, receita.idMunicipio);
		pstmt->setInt(2, receita.id_rec_arrec_detalhe);
		pstmt->setInt(3, receita.ano_exercicio);
		pstmt->setString(4, receita.ds_municipio);
		pstmt->setString(5, receita.ds_orgao);
		pstmt->setInt(6, receita.mes_referencia);
		pstmt->setString(7, receita.mes_ref_extenso);
		pstmt->setString(8, receita.ds_poder);
		pstmt->setString(9, receita.ds_fonte_recurso);
		pstmt->setString(10, receita.ds_cd_aplicacao_fixo);
		pstmt->setString(12, receita.ds_categoria);
		pstmt->setString(13, receita.ds_subcategoria);
		pstmt->setString(14, receita.ds_fonte);
		pstmt->setString(18, receita.ds_tipo);
		pstmt->setDouble(19, receita.vl_arrecadacao);
		linhasAfetadas = pstmt->executeUpdate();

	}
	catch (sql::SQLException& e) {
		//std::cerr << "Erro ao Inserir: " << e.what() << std::endl;
		return -1;
	}

	return linhasAfetadas;
}

int inserirNReceitasAudesp(std::vector<Municipio>& municipios)
{
	std::unique_ptr<sql::Connection> conn = conectarRec();
	int linhasAfetadas = 0;
	std::string directory = SimpubliDirectory::GetHomeUserDirectory() + "/SimPubli/csv/receitas";
	std::vector<std::string> csvs = SimpubliDirectory::GetListFiles(directory, ".csv");
	conn->setAutoCommit(false);

	std::vector<ConferenciaReceitasAudesp> confere_receitas = selecionar_conferencia_receitas();
	
	for (std::string csv : csvs)
	{
		std::vector<std::string> csv_file = SimpubFile::ReadTextFileLines(csv);
		std::string municipio_selecionado = SimpubFile::FileNameFromPath(csv);
		SimpubliString::ReplaceAll(municipio_selecionado, "receitas-", "");
		SimpubliString::ReplaceAll(municipio_selecionado, "-2025", "");

		int max_id_receitas = 0;

		for (ConferenciaReceitasAudesp confere_receita : confere_receitas)
		{
			if (confere_receita.nome_audesp == municipio_selecionado)
			{
				max_id_receitas = confere_receita.max_id_arrecadacao;
			}
		}

		int id_ibge = 0;

		//Pegar o id do municipio e id_ibge
		for (Municipio municipio : municipios)
		{
			if (municipio.nome_audesp == municipio_selecionado)
			{
				
				id_ibge = municipio.id_ibge;
			}
		}

		std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("INSERT INTO receitas_audesp VALUES (NULL, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)"));
		
		int get_line = 0;

		for (std::string csv_line : csv_file)
		{
			SimpubliString::ReplaceAll(csv_line, "\\x", "");
			if (get_line > 0)
			{
				std::vector<std::string> valores = SimpubliString::Split(csv_line, ';');
				
				if (std::stoi(valores[0]) > max_id_receitas)
				{
					//Verifico se é maior que o ultimo ID_ARREC
					try {
						//wxMessageBox(id_ibge);
						pstmt->setInt(1, id_ibge);
						pstmt->setString(2, municipio_selecionado);
						pstmt->setInt(3, std::stoi(valores[0]));
						pstmt->setInt(4, std::stoi(valores[1]));
						pstmt->setString(5, SimpubliString::AnsiToUtf8(std::string(valores[2])));
						pstmt->setString(6, SimpubliString::AnsiToUtf8(std::string(valores[3])));
						pstmt->setInt(7, std::stoi(valores[4]));
						pstmt->setString(8, SimpubliString::AnsiToUtf8(std::string(valores[5])));
						pstmt->setString(9, SimpubliString::AnsiToUtf8(std::string(valores[6])));
						pstmt->setString(10, SimpubliString::AnsiToUtf8(std::string(valores[7])));
						pstmt->setString(11, SimpubliString::AnsiToUtf8(std::string(valores[8])));
						pstmt->setString(12, SimpubliString::AnsiToUtf8(std::string(valores[10])));
						pstmt->setString(13, SimpubliString::AnsiToUtf8(std::string(valores[11])));
						pstmt->setString(14, SimpubliString::AnsiToUtf8(std::string(valores[12])));
						pstmt->setString(15, SimpubliString::AnsiToUtf8(std::string(valores[16])));
						std::string arrec = valores[17];
						SimpubliString::ReplaceAll(arrec, ".", "");
						SimpubliString::ReplaceAll(arrec, ",", ".");
						pstmt->setDouble(16, std::stod(arrec));
						linhasAfetadas += pstmt->executeUpdate();
					}
					catch (sql::SQLException& e) {
						std::cerr << e.what() << std::endl;
						return -1;
					}
				}
			}
			get_line++;
		}

		conn->commit();
	}

	// Todos os ultimos ids de cada municipio (id_municipio, max(id_arrecadacao), nome_municipio_audesp)

	return linhasAfetadas;
}

std::vector<ConferenciaReceitasAudesp> selecionar_conferencia_receitas()
{
	std::unique_ptr<sql::Connection> conn = conectarRec();
	std::vector<ConferenciaReceitasAudesp> ret;

	try {
		std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("SELECT MAX(id_rec_arrec_detalhe) AS max_id, nome_audesp FROM receitas_audesp GROUP BY nome_audesp;"));
		std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

		while (res->next()) {
			ConferenciaReceitasAudesp r;
			r.max_id_arrecadacao = res->getInt("max_id");
			r.nome_audesp = res->getString("nome_audesp");
			ret.push_back(r);
		}
	}
	catch (sql::SQLException& e) {
		std::cerr << "Erro ao pesquisar: " << e.what() << std::endl;
	}

	return ret;
}

