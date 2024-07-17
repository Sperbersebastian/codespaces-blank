#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>

// Function declarations
void printHelp();
bool isValidFilename(const std::string& filename);
bool isValidOption(const std::string& option);
bool isValidNamespace(const std::string& ns);
std::vector<std::vector<std::string>> considerTable(const std::string& filename, const std::string& ns);

int main(int argc, char* argv[]) {
    // Command line arguments
    std::string option;
    std::string filename;
    std::string ns;

    // Check if the number of arguments is correct
    if (argc < 3 || argc > 4) {
        printHelp();
        return 1;
    }

    // Parse command line arguments
    option = argv[1];
    filename = argv[2];
    if (argc == 4) {
        ns = argv[3];
    }

    // Validate option
    if (!isValidOption(option)) {
        std::cerr << "Error: Invalid option '" << option << "'.\n";
        return 1;
    }

    // Validate filename
    if (!isValidFilename(filename)) {
        std::cerr << "Error: Invalid filename '" << filename << "'.\n";
        return 1;
    }

    // Validate namespace if provided
    if (argc == 4 && !isValidNamespace(ns)) {
        std::cerr << "Error: Invalid namespace '" << ns << "'.\n";
        return 1;
    }

    // Application logic here
    if (option == "--consider-table") {
        auto result = considerTable(filename, ns);
        for (const auto& row : result) {
            std::cout << row[0] << " " << row[1] << "\n";
        }
    } else if (option == "--obsolete-stats") {
        std::cout << "Option for --obsolete-stats not yet implemented.\n";
    }

    return 0;
}

void printHelp() {
    std::cout << "Usage: go_parser <option> <filename> [namespace]\n";
    std::cout << "Options:\n";
    std::cout << "  --consider-table    : Generate a table of consider terms.\n";
    std::cout << "  --obsolete-stats    : Generate statistics on obsolete terms.\n";
    std::cout << "Filename:\n";
    std::cout << "  Path to the GO file.\n";
    std::cout << "Namespace (optional):\n";
    std::cout << "  molecular_function\n";
    std::cout << "  cellular_component\n";
    std::cout << "  biological_process\n";
}

bool isValidFilename(const std::string& filename) {
    std::ifstream file(filename);
    return file.good();
}

bool isValidOption(const std::string& option) {
    return option == "--consider-table" || option == "--obsolete-stats";
}

bool isValidNamespace(const std::string& ns) {
    return ns == "molecular_function" || ns == "cellular_component" || ns == "biological_process";
}

std::vector<std::vector<std::string>> considerTable(const std::string& filename, const std::string& ns) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file '" << filename << "'\n";
        return {};
    }

    std::string line;
    std::unordered_map<std::string, std::vector<std::string>> obsolete_terms;
    std::unordered_map<std::string, std::string> term_namespace;
    std::string current_id;
    std::string current_namespace;
    bool is_obsolete = true;

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
        const std::string& term_ns = pair.second;
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
