#include "main_resource.h"
#include "simpubli_directories.h"
#include "simpubli_files.h"
#include "simpubli_database.h"
#include "simpubli_strings.h"
#include "simpubli_audesp_receitas.h"
#include "municipios.h"
#include "receitas.h"

bool executePathConfiguration() {
    std::vector<std::string> data_dir;
    data_dir.push_back(SimpubliDirectory::GetHomeUserDirectory() + "/SimPubli/zip/receitas");
    data_dir.push_back(SimpubliDirectory::GetHomeUserDirectory() + "/SimPubli/csv/receitas");
    data_dir.push_back(SimpubliDirectory::GetHomeUserDirectory() + "/SimPubli/zip/despesas");
    data_dir.push_back(SimpubliDirectory::GetHomeUserDirectory() + "/SimPubli/csv/despesas");
    data_dir.push_back(SimpubliDirectory::GetHomeUserDirectory() + "/SimPubli/data");
    
    for (std::string dir : data_dir) {
        try {
            if (!SimpubliDirectory::DirectoryExistsW(dir)) {
                SimpubliDirectory::CreateDirectory(dir);
            }
            else {
                if (dir != data_dir[4]) {
                    SimpubliDirectory::RemoveAllFiles(dir);
                }
            }
        }
        catch (SimpubliDirectoryError& e) {
            std::cerr << e.what();
            return false;
        }
    }
    return true;
}

std::string verifyConfFile(std::string& file_path) {

    std::string conf_data;

    if (SimpubFile::FileExists(file_path)) {
        conf_data = SimpubFile::ReadTextFile(file_path);
    }

    return conf_data;
}

bool executeDBConfiguration() {

    std::string file_path = SimpubliDirectory::GetHomeUserDirectory() + "/SimPubli/data/data.conf";
    std::string conf_data = verifyConfFile(file_path);
    
    if (conf_data.empty()) {
        try {
            
            bool next = false;
            std::string db_local_url;
            std::string db_local_user;
            std::string db_local_password;

            std::string db_remote_url;
            std::string db_remote_user;
            std::string db_remote_password;

            while (!next) {
                std::cout << "Digite a url do Servidor local: " << std::endl;
                std::cin >> db_local_url;
                next = true;
            }

            next = false;

            while (!next) {
                std::cout << "Digite o usuário do Servidor local: " << std::endl;
                std::cin >> db_local_user;
                next = true;
            }

            next = false;

            while (!next) {
                std::cout << "Digite a senha para o usuário do Servidor local: " << std::endl;
                std::cin >> db_local_password;
                next = true;
            }

            SimpubliDatabase* db = new SimpubliDatabase();
            std::unique_ptr<sql::Connection> connection = db->Connect(db_local_url, db_local_user, db_local_password);
            if (connection == nullptr) {
                std::cout << "Não foi possível conectar ao Banco de dados: " << std::endl;
                return false;
            }
            else {
                std::string csv = db_local_url + ";" + db_local_user + ";" + db_local_password;
                std::string enc = SimpubliString::Encrypt(csv, 7);
                SimpubFile::CreateTextFile(file_path, enc);
            }

            delete db;
            db = nullptr;
        }
        catch (SimpubliFileError& e) {
            std::cerr << e.what() << std::endl;
        }
    }

    return true;
}

void display_menu() {
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << "--- Selecione a opção ---" << std::endl;
    std::cout << "1. Configurar banco de dados." << std::endl;
    std::cout << "2. Somento os dados do IBGE." << std::endl;
    std::cout << "3. Somente os dados de Receitas." << std::endl;
    std::cout << "4. Somente os dados de Despesas." << std::endl;
    std::cout << "5. Gerar JSON." << std::endl;
    std::cout << "6. Executar todos." << std::endl;
    std::cout << "7. Ajuda." << std::endl;
    std::cout << "8. Sair" << std::endl;
    std::cout << "-----------------" << std::endl;
    std::cout << "Escolha a opção: ";
}

int inputVerify(std::string vl) {
    if (vl == "--database") return 1;
    if (vl == "--ibge") return 2;
    if (vl == "--receitas") return 3;
    if (vl == "--despesas") return 4;
    if (vl == "--json") return 5;
    if (vl == "--all") return 6;
    if (vl == "--help") return 7;

    return 8;
}

void displayHelp() {
    std::cout << "-----------------" << std::endl;
    std::cout << "--database = Abre as configurações do sistema." << std::endl;
    std::cout << "--ibge = Executa apenas a coleta de dados do IBGE." << std::endl;
    std::cout << "--receitas = Executa apenas a coleta de dados de receitas." << std::endl;
    std::cout << "--despesas = Executa apenas a coleta de dados de despesas." << std::endl;
    std::cout << "--json = Executa apenas o envio de dados para o servidor remoto." << std::endl;
    std::cout << "--all = Executa todas as coletas." << std::endl;
    std::cout << "--help = Abre esta ajuda." << std::endl;
    std::cout << "-----------------" << std::endl;
}

int readOption(int& option, bool& cont, int& arg) {
    cont = true;
    SimpubliAudespReceitas* sim_rec = new SimpubliAudespReceitas();
    
    if (arg == 0) {
        display_menu();
        std::cin >> option;
    }
    
    switch (option) {
        case 1:
            
            break;
        case 2:
            executar();
            break;
        case 3:
            sim_rec->ExecutAudesp();
            break;
        case 4:
            break;
        case 5:
            break;
        case 6:
            executar();
            executarReceitas();
            break;
        case 7:
            displayHelp();
            break;
        case 8:
            cont = false;
            return 2;
        default:
            std::cout << "\nOpcao invalida. Por favor, tente novamente." << std::endl;
            cont = false;
            return 2;
    }

    std::cin.clear();
    delete sim_rec;
    return arg;
}
