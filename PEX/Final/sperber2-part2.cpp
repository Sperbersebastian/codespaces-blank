#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>
#include <sstream>
#include <filesystem>

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

std::vector<std::tuple<std::string, std::string, std::string, std::string>> get_metacyc_entries(const std::string& filename, const std::vector<std::string>& metacyc_ids) {
    std::vector<std::tuple<std::string, std::string, std::string, std::string>> results;
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Error: Could not open the file '" + filename + "'.");
    }

    std::string line;
    std::string id, name, ns, entry;
    bool in_term = false;

    while (std::getline(file, line)) {
        if (line == "[Term]") {
            in_term = true;
            id.clear();
            name.clear();
            ns.clear();
            entry.clear();
        } else if (line.empty() && in_term) {
            in_term = false;
            for (const auto& metacyc_id : metacyc_ids) {
                if (entry.find("MetaCyc:" + metacyc_id) != std::string::npos) {
                    results.emplace_back(id, name, ns, entry);
                    break;
                }
            }
        } else if (in_term) {
            if (line.find("id: ") == 0) {
                id = line.substr(4);
            } else if (line.find("name: ") == 0) {
                name = line.substr(6);
            } else if (line.find("namespace: ") == 0) {
                ns = line.substr(11);
            } else if (line.find("xref: MetaCyc:") == 0) {
                entry = line.substr(5);
            }
        }
    }

    return results;
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
                auto results = get_metacyc_entries(obofile, metacyc_ids);
                for (const auto& [id, name, ns, entry] : results) {
                    std::cout << id << " " << name << " " << ns << " " << entry << "\n";
                }
            } catch (const std::runtime_error& e) {
                std::cerr << e.what() << "\n";
                return 1;
            }
        } else if (arg1 == "--tab-metacyc") {
            // Placeholder for the --tab-metacyc functionality
            std::cout << "Processing tab-metacyc using OBO file: " << obofile << "\n";
        }
    } else {
        std::cerr << "Error: Invalid argument.\n";
        show_help();
        return 1;
    }

    return 0;
}
