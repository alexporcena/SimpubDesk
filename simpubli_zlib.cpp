#include "simpubli_zlib.h"
#include "simpubli_files.h"

bool SimpubliZlib::UnzipFile(std::string& file_path_zip, std::string& file_path_csv)
{
    if (SimpubFile::FileExists(file_path_csv)) {
        std::filesystem::remove(file_path_csv);
    }

    if (!SimpubFile::FileExists(file_path_zip)) {
        std::cout << "O arquivo: " << file_path_zip << " não pode ser encontrado" << std::endl;
        return false;
    }

    unzFile zipFile = unzOpen(file_path_zip.c_str());
    if (!zipFile) {
        std::cout << "O arquivo: " << file_path_zip << " não pode ser aberto" << std::endl;
        return false;
        //throw SimpubliZlibError("Erro ao abrir o arquivo.");
    }

    
    unz_global_info globalInfo;
    
    if (unzGetGlobalInfo(zipFile, &globalInfo) != UNZ_OK) {
        unzClose(zipFile);
        throw SimpubliZlibError("Impossível receber as informações do arquivo zipado.");
    }

    for (int i = 0; i < globalInfo.number_entry; ++i) {
        unz_file_info fileInfo;
        char filename[256];
        if (unzGetCurrentFileInfo(zipFile, &fileInfo, filename, sizeof(filename), nullptr, 0, nullptr, 0) != UNZ_OK) {
            unzClose(zipFile);
            throw SimpubliZlibError("Impossivel receber as informações dos arquivos");
        }

        if (filename[strlen(filename) - 1] != '/') {
        
            // Open the current file in the zip
            if (unzOpenCurrentFile(zipFile) != UNZ_OK) {
                unzClose(zipFile);
                throw SimpubliZlibError("Impossivel abrir o arquivo");
            }

            FILE* outFile = nullptr;
            errno_t err = fopen_s(&outFile, file_path_csv.c_str(), "w+");

            if (err != 0 || outFile == nullptr) {
                unzCloseCurrentFile(zipFile);
                unzClose(zipFile);
                std::cerr << "Error: Could not create output file " << filename << std::endl;
                return false;
            }

            // Read and write data in chunks
            std::vector<char> buffer(READ_SIZE);
            int bytesRead = 0;
            while ((bytesRead = unzReadCurrentFile(zipFile, buffer.data(), READ_SIZE)) > 0) {
                fwrite(buffer.data(), 1, bytesRead, outFile);
            }

            fclose(outFile);
            unzCloseCurrentFile(zipFile);
        }

        if ((i + 1) < globalInfo.number_entry) {
            if (unzGoToNextFile(zipFile) != UNZ_OK) {
                std::cerr << "Error: Could not go to next file." << std::endl;
                unzClose(zipFile);
                return 1;
            }
        }
    }

    unzClose(zipFile);
    return true;
}
