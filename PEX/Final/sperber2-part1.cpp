#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <filesystem>

void show_help() {
    std::cout << "Usage: appname [--help, --get-metacyc MetaCyc-ID, --tab-metacyc] OBOFILE\n";
    std::cout << "Options:\n";
    std::cout << "  --help               Show this help message\n";
    std::cout << "  --get-metacyc        Get information for the specified MetaCyc ID\n";
    std::cout << "  --tab-metacyc        Generate a tab-delimited report from the OBO file\n";
    std::cout << "Arguments:\n";
    std::cout << "  OBOFILE              Path to the GO-obo input file\n";
    std::cout << "  MetaCyc-ID           Optional MetaCyc identifier (required for --get-metacyc)\n";
}

bool is_valid_file(const std::string& filename) {
    std::ifstream infile(filename);
    return infile.good();
}

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
            show_help();
            return 1;
        }

        obofile = argv[argc - 1];

        if (!fs::exists(obofile)) {
            std::cerr << "Error: The file '" << obofile << "' does not exist.\n";
            show_help();
            return 1;
        }

        if (arg1 == "--get-metacyc") {
            for (int i = 2; i < argc - 1; ++i) {
                std::string metacyc_id = argv[i];
                if (!is_valid_metacyc_id(metacyc_id)) {
                    std::cerr << "Error: The MetaCyc ID '" << metacyc_id << "' must contain either 'RXN' or 'PWY'.\n";
                    show_help();
                    return 1;
                }
                metacyc_ids.push_back(metacyc_id);
            }

            try {
                MetaCycParser parser(obofile);
                auto results = parser.get_metacyc_entries(metacyc_ids);
                for (const auto& [id, name, ns, entry] : results) {
                    std::cout << id << " " << name << " " << ns << " " << entry << "\n";
                }
            } catch (const std::runtime_error& e) {
                std::cerr << e.what() << "\n";
                return 1;
            }
        } else if (arg1 == "--tab-metacyc") {
            try {
                MetaCycParser parser(obofile);
                auto results = parser.get_tab_metacyc();
                for (const auto& [id, ec, rxn] : results) {
                    std::cout << id << " EC:" << ec << " " << rxn << "\n";
                }
            } catch (const std::runtime_error& e) {
                std::cerr << e.what() << "\n";
                return 1;
            }
        }
    } else {
        std::cerr << "Error: Invalid argument.\n";
        show_help();
        return 1;
    }

    return 0;
}