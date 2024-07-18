// Filename: obo_parser.cpp
// Author: Sebastian Sperber

#include <iostream>
#include <string>
#include <algorithm>

// Function prototypes
void printHelp();
bool isValidSlimName(const std::string& slimName);
bool fileExists(const std::string& filename);

int main(int argc, char* argv[]) {
    // Check if the correct number of arguments is provided
    if (argc < 2 || argc > 4) {
        printHelp();
        return 1;
    }

    // Process command line arguments
    std::string task;
    std::string slimName;
    std::string oboFile;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "--help") {
            printHelp();
            return 0;
        } else if (arg == "--get-slims") {
            task = "--get-slims";
        } else if (arg == "--tab-slim") {
            if (i + 1 < argc) {
                slimName = argv[++i];
                if (!isValidSlimName(slimName)) {
                    std::cerr << "Slim identifiers should consist only of lowercase letters!\n";
                    return 1;
                }
            } else {
                printHelp();
                return 1;
            }
        } else {
            oboFile = arg;
        }
    }

    // Check if a valid OBO file is provided
    if (!fileExists(oboFile)) {
        std::cerr << "'" << oboFile << "' does not exist!\n";
        return 1;
    }

    // Placeholder for functionality
    if (task == "--get-slims") {
        std::cout << "No functionality yet!\n";
    } else if (task == "--tab-slim") {
        std::cout << "No functionality yet!\n";
    } else {
        printHelp();
        return 1;
    }

    return 0;
}

void printHelp() {
    std::cout << "Usage: appname [--help,--get-slims,--tab-slim SLIMNAME] OBOFILE\n";
    std::cout << "Ed's Obo-Parser - extract information from GO-Obo files\n\n";
    std::cout << "--help                   - display this help page\n";
    std::cout << "--get-slims              - get all slim names\n";
    std::cout << "--tab-slim SLIMNAME      - get GO-ids, names and namespaces for given slim\n";
    std::cout << "OBOFILE                  - an uncompressed Gene Ontology Obo file\n";
}

bool isValidSlimName(const std::string& slimName) {
    return std::all_of(slimName.begin(), slimName.end(), [](unsigned char c) {
        return std::islower(c);
    });
}

bool fileExists(const std::string& filename) {
    std::ifstream file(filename);
    return file.good();
}
