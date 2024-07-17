#include <iostream>
#include <fstream>
#include <string>
#include <vector>

// Function declarations
void printHelp();
bool isValidFilename(const std::string& filename);
bool isValidOption(const std::string& option);
bool isValidNamespace(const std::string& ns);

int main(int argc, char* argv[]) {
    // Command line arguments
    std::string option;
    std::string filename;
    std::string ns;

    // List of valid options and namespaces
    std::vector<std::string> validOptions = {"--consider-table", "--obsolete-stats"};
    std::vector<std::string> validNamespaces = {"molecular_function", "cellular_component", "biological_process"};

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
    std::cout << "Option: " << option << "\n";
    std::cout << "Filename: " << filename << "\n";
    if (argc == 4) {
        std::cout << "Namespace: " << ns << "\n";
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
