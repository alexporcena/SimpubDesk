#include "simpubli_base_audesp.h"

SimpubliBaseAudesp::SimpubliBaseAudesp() {
    municipios = listarMunicipios();
    audesp_type = "";
}

SimpubliBaseAudesp::~SimpubliBaseAudesp() {
}

size_t SimpubliBaseAudesp::WriteCallback(void* ptr, size_t size, size_t nmemb, void* stream) {
	return fwrite(ptr, size, nmemb, (FILE*)stream);
}

std::vector<SimpubliBaseAudesp::DownloadInfo> SimpubliBaseAudesp::PrepareRequest() {
    std::vector<DownloadInfo> download_info;

    for (Municipio municipio : municipios) {
        if (municipio.nome_audesp != "sao-paulo") {
            std::string url = "https://transparencia.tce.sp.gov.br/sites/default/files/csv/" + audesp_type + "-" + municipio.nome_audesp + "-2025.zip";
            std::string file_name = SimpubliDirectory::GetHomeUserDirectory() + "/SimPubli/zip/" + audesp_type + "/" + municipio.nome_audesp + ".zip";
            DownloadInfo d_i;
            d_i.url = url;
            d_i.file_name = file_name;
            download_info.push_back(d_i);
        }
    }

    return download_info;
}

int SimpubliBaseAudesp::PerformRequest(const DownloadInfo& info) {
    const char* url_c = info.url.c_str();
    const char* output_filename = info.file_name.c_str();

    CURL* curl;
    CURLcode res;
    FILE* file_pointer = nullptr;
    curl_off_t download_size = 0;

    curl = curl_easy_init();


    if (curl) {
        errno_t err = fopen_s(&file_pointer, output_filename, "wb");

        if (err != 0 || file_pointer == nullptr) {
            std::cerr << "Erro ao abrir/criar o arquivo de saída: " << output_filename << std::endl;
            curl_easy_cleanup(curl);
            curl_global_cleanup();
            return -1;
        }
        curl_easy_setopt(curl, CURLOPT_URL, url_c);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)file_pointer);
        //curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);

        try {
            res = curl_easy_perform(curl);
        }
        catch (...) {
            std::cerr << "Problemas aqui no curl" << std::endl;
            return 0;
        }

        if (res == CURLE_OK) {
        }
        else {
            std::cerr << "curl_easy_perform() falhou: " << curl_easy_strerror(res) << std::endl;
        }

        if (file_pointer != nullptr) {
            fclose(file_pointer);
        }

        curl_easy_cleanup(curl);
    }

    return 0;
}

void SimpubliBaseAudesp::ExecuteCurlRequestThread(std::vector<DownloadInfo>& info) {
    std::vector<std::thread> threads;

    curl_global_init(CURL_GLOBAL_ALL);


    for (const auto& d_info : info) {
        threads.emplace_back(SimpubliBaseAudesp::PerformRequest, d_info);
    }
    
    for (auto& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }

    curl_global_cleanup();
}

void SimpubliBaseAudesp::UnzipFiles()
{
    edit_municipios.clear();

    for (Municipio municipio : municipios) {
        std::string file_name_zip = SimpubliDirectory::GetHomeUserDirectory() + "/SimPubli/zip/" + audesp_type +"/" + municipio.nome_audesp + ".zip";
        std::string file_name_csv = SimpubliDirectory::GetHomeUserDirectory() + "/SimPubli/csv/" + audesp_type + "/" + municipio.nome_audesp + ".csv";

        if (SimpubFile::FileExists(file_name_zip)) {

            int a = SimpubFile::GetFileSize(file_name_zip);
            int last_file_size = 0;

            if (audesp_type != "") {

                if (audesp_type == "receitas") {
                    last_file_size = municipio.last_file_size_receitas;
                }
                else {
                    last_file_size = municipio.last_file_size_despesas;
                }

                if (last_file_size != a) {
                    SimpubliZlib::UnzipFile(file_name_zip, file_name_csv);
                    editaFileSize(municipio.id_ibge, a, audesp_type);
                    edit_municipios.push_back(municipio);
                }
            }
        }
    }
}

bool SimpubliBaseAudesp::ExecutAudesp()
{
    std::cout << "Iniciando o Download dos arquivos, por favor aguarde..." << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<DownloadInfo> download_info = PrepareRequest();
    ExecuteCurlRequestThread(download_info);
    std::cout << "Descompactando arquivos, por favor aguarde..." << std::endl;
    UnzipFiles();
    std::cout << "Salvando no banco de dados, por favor aguarde..." << std::endl;
    InsertDatabase();
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    std::cout << "Concluido em: " << duration.count() << " segundos" << std::endl;
    return true;
}
