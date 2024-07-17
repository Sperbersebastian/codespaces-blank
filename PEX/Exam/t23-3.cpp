#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>

class GoParser {
public:
    void printHelp();
    bool isValidFilename(const std::string& filename);
    bool isValidOption(const std::string& option);
    bool isValidNamespace(const std::string& ns);
    std::vector<std::vector<std::string>> considerTable(const std::string& filename, const std::string& ns);
    void obsoleteStats(const std::string& filename);
    void writeToFile(const std::vector<std::vector<std::string>>& data, const std::string& filename);

private:
    std::unordered_map<std::string, std::vector<std::string>> obsolete_terms;
    std::unordered_map<std::string, std::string> term_namespace;
};

void GoParser::printHelp() {
    std::cout << "Usage: go_parser <option> <filename> [namespace|outfile]\n";
    std::cout << "Options:\n";
    std::cout << "  --consider-table    : Generate a table of consider terms.\n";
    std::cout << "  --obsolete-stats    : Generate statistics on obsolete terms.\n";
    std::cout << "Filename:\n";
    std::cout << "  Path to the GO file.\n";
    std::cout << "Namespace (optional):\n";
    std::cout << "  molecular_function\n";
    std::cout << "  cellular_component\n";
    std::cout << "  biological_process\n";
    std::cout << "Outfile (optional):\n";
    std::cout << "  Path to the output file with .tab extension.\n";
}

bool GoParser::isValidFilename(const std::string& filename) {
    std::ifstream file(filename);
    return file.good();
}

bool GoParser::isValidOption(const std::string& option) {
    return option == "--consider-table" || option == "--obsolete-stats";
}

bool GoParser::isValidNamespace(const std::string& ns) {
    return ns == "molecular_function" || ns == "cellular_component" || ns == "biological_process";
}

std::vector<std::vector<std::string>> GoParser::considerTable(const std::string& filename, const std::string& ns) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file '" << filename << "'\n";
        return {};
    }

    std::string line;
    std::string current_id;
    std::string current_namespace;
    bool is_obsolete = false;

    obsolete_terms.clear();
    term_namespace.clear();

    while (std::getline(file, line)) {
        if (line == "[Term]") {
            current_id.clear();
            current_namespace.clear();
            is_obsolete = false;
        } else if (line.rfind("id: ", 0) == 0) {
            current_id = line.substr(4);
        } else if (line.rfind("namespace: ", 0) == 0) {
            current_namespace = line.substr(11);
            term_namespace[current_id] = current_namespace;
        } else if (line.rfind("is_obsolete: true", 0) == 0) {
            is_obsolete = true;
        } else if (line.rfind("consider: ", 0) == 0) {
            if (is_obsolete) {
                obsolete_terms[current_id].push_back(line.substr(10));
            }
        }
    }

    // Handle obsolete terms without any consider alternatives
    for (const auto& pair : term_namespace) {
        const std::string& id = pair.first;
        if (is_obsolete && obsolete_terms.find(id) == obsolete_terms.end()) {
            obsolete_terms[id].push_back("NA");
        }
    }

    std::vector<std::vector<std::string>> result;
    for (const auto& pair : obsolete_terms) {
        const std::string& id = pair.first;
        const std::vector<std::string>& alternatives = pair.second;
        if (ns.empty() || term_namespace[id] == ns) {
            if (alternatives.empty()) {
                result.push_back({id, "NA"});
            } else {
                for (const auto& alt_id : alternatives) {
                    result.push_back({id, alt_id});
                }
            }
        }
    }

    return result;
}

void GoParser::obsoleteStats(const std::string& filename) {
    auto data = considerTable(filename, "");
    int consider_count = 0;
    int no_consider_count = 0;

    for (const auto& row : data) {
        if (row[1] == "NA") {
            no_consider_count++;
        } else {
            consider_count++;
        }
    }

    std::cout << "consider " << consider_count << "\n";
    std::cout << "no-consider " << no_consider_count << "\n";
}

void GoParser::writeToFile(const std::vector<std::vector<std::string>>& data, const std::string& filename) {
    std::ofstream outfile(filename);
    if (!outfile.is_open()) {
        std::cerr << "Error: Could not open file '" << filename << "' for writing\n";
        return;
    }

    for (const auto& row : data) {
        outfile << row[0] << "\t" << row[1] << "\n";
    }

    outfile.close();
}
