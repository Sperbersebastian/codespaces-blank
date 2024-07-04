#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

// Function declarations
void showHelp(const std::string &programName);

int main(int argc, char *argv[]) {
    // Check if there are enough command line arguments
    if (argc < 2) {
        showHelp(argv[0]);
        return 1;
    }

    std::string option = argv[1];
    std::string filename;
    std::string pattern;

    // Process command line arguments
    if (option == "--help") {
        showHelp(argv[0]);
        return 0;
    } else if (option == "--summary" || option == "--search") {
        if (argc < 3) {
            std::cerr << "Error: Missing filename.\n";
            showHelp(argv[0]);
            return 1;
        }
        filename = argv[2];
        if (option == "--search" && argc >= 4) {
            pattern = argv[3];
        }
    } else {
        std::cerr << "Error: Invalid option.\n";
        showHelp(argv[0]);
        return 1;
    }

    // Check if file exists
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error: File " << filename << " does not exist.\n";
        return 1;
    }

    // Placeholder for the actual functionality
    if (option == "--summary") {
        std::cout << "Summary of the file: " << filename << "\n";
        // Add code to summarize the FASTA file
    } else if (option == "--search") {
        std::cout << "Searching for pattern " << pattern << " in file: " << filename << "\n";
        // Add code to search the FASTA file for the pattern
    }

    return 0;
}

// Function to show help message
void showHelp(const std::string &programName) {
    std::cout << "Detlef’s FastaParser1 - Search and summarize FASTA files\n"
              << "Author: Detlef Groth, University of Potsdam, 2024\n"
              << "Usage: " << programName << " --search|--summary|--help ?PATTERN? file.fasta\n"
              << "Options:\n"
              << "  --help       Show this help message\n"
              << "  --summary    Summarize the contents of the FASTA file\n"
              << "  --search     Search for a sequence pattern in the FASTA file\n"
              << "Arguments:\n"
              << "  PATTERN      (optional) The sequence pattern to search for (used with --search)\n"
              << "  file.fasta   The FASTA file to process\n";
}
