#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include <minizip/unzip.h>

namespace fs = std::filesystem;

void show_help() {
    std::cout << "Usage: appname [--help, --get-metacyc [MetaCyc-ID]...] OBOFILE\n"
              << "Options:\n"
              << "    --help             Show this help message and exit\n"
              << "    --get-metacyc      Retrieve information about specific MetaCyc IDs (requires one or more MetaCyc-IDs)\n"
              << "    --tab-metacyc      Retrieve MetaCyc information in tabular format\n"
              << "    OBOFILE            Path to the GO-obo input file\n"
              << "Examples:\n"
              << "    appname --help\n"
              << "    appname --get-metacyc RXN12345 go-basic.obo\n"
              << "    appname --get-metacyc RXN12345 PWY56789 go-basic.obo\n"
              << "    appname --tab-metacyc go-basic.obo\n";
}

bool is_valid_metacyc_id(const std::string& id) {
    return id.find("RXN") != std::string::npos || id.find("PWY") != std::string::npos;
}

bool is_valid_obo_file(const std::string& filename) {
    return filename.find(".obo") != std::string::npos;
}

bool is_zip_file(const std::string& filename) {
    return filename.size() >= 4 && filename.substr(filename.size() - 4) == ".zip";
}

bool unzip_file(const std::string& zip_filename, const std::string& dest_directory) {
    unzFile zipfile = unzOpen(zip_filename.c_str());
    if (!zipfile) {
        std::cerr << "Error: Cannot open ZIP file '" << zip_filename << "'.\n";
        return false;
    }

    if (unzGoToFirstFile(zipfile) != UNZ_OK) {
        std::cerr << "Error: Cannot go to the first file in ZIP archive.\n";
        unzClose(zipfile);
        return false;
    }

    do {
        char filename_in_zip[256];
        unz_file_info file_info;
        if (unzGetCurrentFileInfo(zipfile, &file_info, filename_in_zip, sizeof(filename_in_zip), nullptr, 0, nullptr, 0) != UNZ_OK) {
            std::cerr << "Error: Cannot get current file info.\n";
            unzClose(zipfile);
            return false;
        }

        std::string full_filename = dest_directory + "/" + std::string(filename_in_zip);
        std::ofstream output_file(full_filename, std::ios::binary);
        if (!output_file.is_open()) {
            std::cerr << "Error: Cannot open output file '" << full_filename << "'.\n";
            unzClose(zipfile);
            return false;
        }

        if (unzOpenCurrentFile(zipfile) != UNZ_OK) {
            std::cerr << "Error: Cannot open current file in ZIP archive.\n";
            unzClose(zipfile);
            return false;
        }

        char buffer[8192];
        int bytes_read;
        while ((bytes_read = unzReadCurrentFile(zipfile, buffer, sizeof(buffer))) > 0) {
            output_file.write(buffer, bytes_read);
        }

        unzCloseCurrentFile(zipfile);
        output_file.close();

        if (bytes_read < 0) {
            std::cerr << "Error: Failed to read file in ZIP archive.\n";
            unzClose(zipfile);
            return false;
        }
    } while (unzGoToNextFile(zipfile) == UNZ_OK);

    unzClose(zipfile);
    return true;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        show_help();
        return 1;
    }

    std::string arg1 = argv[1];
    std::string obofile;
    std::vector<std::string> metacyc_ids;

    if (arg1 == "--help") {
        show_help();
        return 0;
    } else if (arg1 == "--get-metacyc" || arg1 == "--tab-metacyc") {
        if (argc < 3) {
            std::cerr << "Error: OBOFILE is required.\n";
            return 1;
        }

        obofile = argv[argc - 1];

        if (!fs::exists(obofile)) {
            std::cerr << "Error: The file '" << obofile << "' does not exist.\n";
            return 1;
        }

        if (is_zip_file(obofile)) {
            std::string dest_directory = fs::path(obofile).parent_path();
            if (!unzip_file(obofile, dest_directory)) {
                std::cerr << "Error: Failed to unzip file '" << obofile << "'.\n";
                return 1;
            }
            // Assuming the unzipped file has the same name but without .zip
            obofile = dest_directory + "/" + fs::path(obofile).stem().string() + ".obo";
        }

        if (!is_valid_obo_file(obofile)) {
            std::cerr << "Error: The file '" << obofile << "' is not a valid OBO file. It must have a .obo extension.\n";
            return 1;
        }

        if (arg1 == "--get-metacyc") {
            for (int i = 2; i < argc - 1; ++i) {
                std::string metacyc_id = argv[i];
                if (!is_valid_metacyc_id(metacyc_id)) {
                    std::cerr << "Error: The MetaCyc ID '" << metacyc_id << "' must contain either 'RXN' or 'PWY'.\n";
                    return 1;
                }
                metacyc_ids.push_back(metacyc_id);
            }

            // Here you would process the obofile and metacyc_ids
            // For example, you might have:
            // MetaCycParser parser(obofile);
            // auto results = parser.get_metacyc_entries(metacyc_ids);
            // for (const auto& [id, name, ns, entry] : results) {
            //     std::cout << id << " " << name << " " << ns << " " << entry << "\n";
            // }

            std::cout << "Processing get-metacyc with the following IDs:\n";
            for (const auto& id : metacyc_ids) {
                std::cout << id << "\n";
            }
            std::cout << "Using OBO file: " << obofile << "\n";
        } else if (arg1 == "--tab-metacyc") {
            // Here you would process the obofile to get the tabular data
            // For example, you might have:
            // MetaCycParser parser(obofile);
            // auto results = parser.get_tab_metacyc();
            // for (const auto& [id, ec, rxn] : results) {
            //     std::cout << id << " EC:" << ec << " " << rxn << "\n";
            // }

            std::cout << "Processing tab-metacyc using OBO file: " << obofile << "\n";
        }
    } else {
        std::cerr << "Error: Invalid argument.\n";
        show_help();
        return 1;
    }

    return 0;
}
