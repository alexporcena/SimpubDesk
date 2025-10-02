#include "mariadb_dados_despesas.h"
#include "simpubli_strings.h"
#include "simpubli_files.h"
#include "simpubli_directories.h"

std::unique_ptr<sql::Connection> conectarDesp()
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

int quantidadeDespesaAudesp(const int id_ibge)
{
	std::unique_ptr<sql::Connection> conn = conectarDesp();
	int r = 0;
	try {
		std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("SELECT count(ID) AS REGISTROS FROM despesas_audesp WHERE id_ibge = ?"));
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

int inserirNDespesaAudesp(std::vector<Municipio>& municipios)
{
	std::unique_ptr<sql::Connection> conn = conectarDesp();
	int linhasAfetadas = 0;
	std::string directory = SimpubliDirectory::GetHomeUserDirectory() + "/SimPubli/csv/despesas";
	std::vector<std::string> csvs = SimpubliDirectory::GetListFiles(directory, ".csv");
	conn->setAutoCommit(false);

	std::vector<ConferenciaDespesasAudesp> confere_despesas = selecionar_conferencia_despesas();

	for (std::string csv : csvs)
	{
		std::vector<std::string> csv_file = SimpubFile::ReadTextFileLines(csv);
		std::string municipio_selecionado = SimpubFile::FileNameFromPath(csv);
		SimpubliString::ReplaceAll(municipio_selecionado, "despesas-", "");
		SimpubliString::ReplaceAll(municipio_selecionado, "-2025", "");

		int max_id_receitas = 0;

		for (ConferenciaDespesasAudesp confere_despesa : confere_despesas)
		{
			if (confere_despesa.nome_audesp == municipio_selecionado)
			{
				max_id_receitas = confere_despesa.max_id_despesa;
			}
		}

		int id_ibge = 0;

		for (Municipio municipio : municipios)
		{
			if (municipio.nome_audesp == municipio_selecionado)
			{

				id_ibge = municipio.id_ibge;
			}
		}

		std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("INSERT INTO despesas_audesp VALUES (NULL, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)"));

		int get_line = 0;

		for (std::string csv_line : csv_file)
		{
			SimpubliString::ReplaceAll(csv_line, "\"", "");
			SimpubliString::ReplaceAll(csv_line, "\'", "");
			
			if (get_line > 0)
			{
				std::vector<std::string> valores = SimpubliString::Split(csv_line, ';');

				if (std::stoi(valores[0]) > max_id_receitas)
				{
					try {
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
						pstmt->setString(12, SimpubliString::AnsiToUtf8(std::string(valores[9])));
						pstmt->setString(13, SimpubliString::AnsiToUtf8(std::string(valores[10])));

						std::string arrec = valores[11];
						SimpubliString::ReplaceAll(arrec, ".", "");
						SimpubliString::ReplaceAll(arrec, ",", ".");
						pstmt->setDouble(14, std::stod(arrec));

						pstmt->setString(15, SimpubliString::AnsiToUtf8(std::string(valores[12])));
						pstmt->setString(16, SimpubliString::AnsiToUtf8(std::string(valores[13])));
						pstmt->setString(17, SimpubliString::AnsiToUtf8(std::string(valores[14])));
						pstmt->setString(18, SimpubliString::AnsiToUtf8(std::string(valores[15])));
						pstmt->setString(19, SimpubliString::AnsiToUtf8(std::string(valores[16])));
						pstmt->setString(20, SimpubliString::AnsiToUtf8(std::string(valores[17])));
						pstmt->setString(21, SimpubliString::AnsiToUtf8(std::string(valores[18])));
						pstmt->setString(22, SimpubliString::AnsiToUtf8(std::string(valores[19])));
						pstmt->setString(23, SimpubliString::AnsiToUtf8(std::string(valores[20])));
						pstmt->setString(24, SimpubliString::AnsiToUtf8(std::string(valores[21])));
						
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


	return linhasAfetadas;
}

std::vector<ConferenciaDespesasAudesp> selecionar_conferencia_despesas()
{
	std::unique_ptr<sql::Connection> conn = conectarDesp();
	std::vector<ConferenciaDespesasAudesp> ret;

	try {
		std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("SELECT MAX(vl_despesa) AS max_vl, nome_audesp FROM despesas_audesp GROUP BY nome_audesp;"));
		std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

		while (res->next()) {
			ConferenciaDespesasAudesp r;
			r.max_id_despesa = res->getInt("max_vl");
			r.nome_audesp = res->getString("nome_audesp");
			ret.push_back(r);
		}
	}
	catch (sql::SQLException& e) {
		std::cerr << "Erro ao pesquisar: " << e.what() << std::endl;
	}

	return ret;
}
