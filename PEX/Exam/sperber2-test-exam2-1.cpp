#include <iostream>
#include <fstream>
#include <string>
#include <vector>

void show_help() {
    std::cout << "Detlef’s FastaParser1 searches FASTA files\n";
    std::cout << "Author: Detlef Groth, University of Potsdam, 2024\n";
    std::cout << "Usage: FastaParser1 --search|--summary|--help ?PATTERN? file.fasta\n";
}

bool file_exists(const std::string &filename) {
    std::ifstream file(filename);
    return file.good();
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        show_help();
        return 1;
    }

    std::string option = argv[1];
    std::string filename;
    std::string pattern;

    if (option == "--help") {
        show_help();
        return 0;
    } else if (option == "--search" || option == "--summary") {
        if (argc < 3) {
            show_help();
            return 1;
        }
        filename = argv[2];
        if (option == "--search" && argc == 4) {
            pattern = argv[3];
        }
    } else {
        show_help();
        return 1;
    }

    if (!file_exists(filename)) {
        std::cerr << "Error: File " << filename << " does not exist.\n";
        return 1;
    }

    // Placeholder for the rest of the implementation
    std::cout << "Option: " << option << "\n";
    std::cout << "Filename: " << filename << "\n";
    if (!pattern.empty()) {
        std::cout << "Pattern: " << pattern << "\n";
    }

    return 0;
}
