#include "mariadb_api_municipios.h"
#include "simpubli_files.h"
#include "simpubli_directories.h"
#include "simpubli_strings.h"

std::unique_ptr<sql::Connection> conectar()
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

std::vector<Municipio> listarMunicipios() {

	std::unique_ptr<sql::Connection> conn = conectar();

	std::vector<Municipio> municipios;
	try {
		std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("SELECT * FROM municipios_ibge;"));
		std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

		while (res->next()) {
			Municipio m;
			m.id_ibge = res->getInt("id_ibge");
			m.nome = res->getString("nome");
			m.nome_audesp = res->getString("nome_audesp");
			m.populacao = res->getInt("populacao");
			m.last_file_size_receitas = res->getInt("last_file_size_receitas");
			m.last_file_size_despesas = res->getInt("last_file_size_despesas");
			municipios.push_back(m);
		}
	}
	catch (sql::SQLException& e) {
		std::cerr << "Erro ao pesquisar: " << e.what() << std::endl;
		return municipios;
	}

	return municipios;
}

int inserirNMunicipios(std::vector<Municipio> municipios)
{
	int ret = 0;

	std::unique_ptr<sql::Connection> conn = conectar();

	try {
		//Cancelo o autom commit
		conn->setAutoCommit(false);
		std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("INSERT INTO municipios_ibge (ID_IBGE, NOME, NOME_AUDESP, UF, POPULACAO) VALUES (?, ?, ?, ?, ?)"));

		for (Municipio municipio  : municipios)	{
			pstmt->setInt(1, municipio.id_ibge);
			pstmt->setString(2, municipio.nome);
			pstmt->setString(3, municipio.nome_audesp);
			pstmt->setString(4, municipio.uf);
			pstmt->setInt(5, municipio.populacao);
			ret += pstmt->executeUpdate();
		}

		conn->commit();
		conn->setAutoCommit(true);
	}
	catch (sql::SQLException& e) {
		std::cerr << "Erro ao Inserir: " << e.what() << std::endl;
		return -1;
	}

	return ret;
}

int editarNMunicipios(std::vector<Municipio> municipios)
{
	int ret = 0;

	std::unique_ptr<sql::Connection> conn = conectar();

	try {
		conn->setAutoCommit(false);
		std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("UPDATE municipios_ibge SET POPULACAO = ? WHERE ID_IBGE = ? "));
		for (Municipio municipio : municipios) {
			pstmt->setInt(1, municipio.populacao);
			pstmt->setInt(2, municipio.id_ibge);
			ret += pstmt->executeUpdate();
		}

		conn->commit();
		conn->setAutoCommit(true);
	}
	catch (sql::SQLException& e) {
		std::cerr << "Erro ao Editar: " << e.what() << std::endl;
		return -1;
	}

	return ret;
}

int editaFileSize(int id_ibge, int file_size, std::string& audesp_type)
{
	int ret = 0;

	std::unique_ptr<sql::Connection> conn = conectar();
	std::string sql_str = "UPDATE municipios_ibge SET last_file_size_" + audesp_type + " = ? WHERE ID_IBGE = ? ";

	try {

		std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement(sql_str));
		
		pstmt->setInt(1, file_size);
		pstmt->setInt(2, id_ibge);
		ret = pstmt->executeUpdate();
	}
	catch (sql::SQLException& e) {
		std::cerr << "Erro ao Editar: " << e.what() << std::endl;
		return -1;
	}

	return ret;
}

std::vector<int> conferirMunicipio(std::vector<Municipio>& municipios_api, std::vector<Municipio>& municipios_db)
{
	std::vector<int> acoes;
	for (Municipio municipio_api : municipios_api)
	{
		acoes.push_back(localizarAlteracoes(municipio_api, municipios_db));
	}
	return acoes;

}

int localizarAlteracoes(Municipio& municipio_api, std::vector<Municipio>& municipios_db)
{
	for (Municipio municipio_db : municipios_db) {
		if (municipio_db.id_ibge == municipio_api.id_ibge) {
			if (municipio_db.populacao == municipio_api.populacao)
				return 0;
			else
				return 1;
		}
	}
	return 2;
}
