#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>
#include <regex>
#include <filesystem>
#include <unordered_map>

namespace fs = std::filesystem;

class MetaCycParser {
public:
    MetaCycParser(const std::string& filename) : filename(filename) {
        if (!fs::exists(filename)) {
            throw std::runtime_error("Error: The file '" + filename + "' does not exist.");
        }
    }

    std::vector<std::tuple<std::string, std::string, std::string, std::string>> get_metacyc_entries(const std::vector<std::string>& metacyc_ids) {
        std::vector<std::tuple<std::string, std::string, std::string, std::string>> results;
        std::ifstream file(filename);
        if (!file) {
            throw std::runtime_error("Error: Cannot open file '" + filename + "'");
        }

        std::string line;
        std::string id, name, ns, entry;
        bool in_entry = false;

        while (std::getline(file, line)) {
            if (line == "[Term]") {
                if (in_entry) {
                    for (const auto& metacyc_id : metacyc_ids) {
                        if (entry.find("MetaCyc:" + metacyc_id) != std::string::npos) {
                            results.emplace_back(id, name, ns, entry);
                        }
                    }
                }
                id.clear();
                name.clear();
                ns.clear();
                entry.clear();
                in_entry = true;
            } else if (line.find("id:") == 0) {
                id = line.substr(4);
            } else if (line.find("name:") == 0) {
                name = line.substr(6);
            } else if (line.find("namespace:") == 0) {
                ns = line.substr(11);
            } else if (line.find("xref: MetaCyc:") != std::string::npos) {
                entry = line.substr(6);
            }

            if (in_entry) {
                entry += line + "\n";
            }
        }

        if (in_entry) {
            for (const auto& metacyc_id : metacyc_ids) {
                if (entry.find("MetaCyc:" + metacyc_id) != std::string::npos) {
                    results.emplace_back(id, name, ns, entry);
                }
            }
        }

        return results;
    }

    std::vector<std::tuple<std::string, std::string, std::string>> get_tab_metacyc() {
        std::vector<std::tuple<std::string, std::string, std::string>> results;
        std::ifstream file(filename);
        if (!file) {
            throw std::runtime_error("Error: Cannot open file '" + filename + "'");
        }

        std::string line;
        std::string id, ec, rxn;
        std::unordered_map<std::string, std::pair<std::string, std::string>> go_mapping;

        while (std::getline(file, line)) {
            if (line == "[Term]") {
                if (!id.empty() && !ec.empty() && !rxn.empty()) {
                    go_mapping[id] = std::make_pair(ec, rxn);
                }
                id.clear();
                ec.clear();
                rxn.clear();
            } else if (line.find("id:") == 0) {
                id = line.substr(4);
            } else if (line.find("xref: EC:") == 0) {
                ec = line.substr(6);
            } else if (line.find("xref: MetaCyc:RXN") == 0) {
                rxn = line.substr(6);
            }
        }

        file.clear();
        file.seekg(0, std::ios::beg);

        while (std::getline(file, line)) {
            if (line == "[Term]") {
                if (!id.empty() && go_mapping.count(id) > 0) {
                    const auto& [mapped_ec, mapped_rxn] = go_mapping[id];
                    results.emplace_back(id, mapped_ec.empty() ? "NA" : mapped_ec, mapped_rxn);
                }
                id.clear();
            } else if (line.find("id:") == 0) {
                id = line.substr(4);
            }
        }

        return results;
    }

private:
    std::string filename;
};

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
