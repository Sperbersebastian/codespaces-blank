#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>

void show_help() {
    std::cout << "Detlef’s FastaParser1 searches FASTA files\n";
    std::cout << "Author: Detlef Groth, University of Potsdam, 2024\n";
    std::cout << "Usage: FastaParser1 --search|--summary|--help ?PATTERN? file.fasta\n";
}

bool file_exists(const std::string &filename) {
    std::ifstream file(filename);
    return file.good();
}

// Function to search sequences in a FASTA file
std::vector<std::tuple<std::string, std::string, bool>> search_sequence(const std::string &filename, const std::string &pattern) {
    std::vector<std::tuple<std::string, std::string, bool>> results;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << "\n";
        return results;
    }

    std::string line, seq_id, sequence;
    std::regex re(pattern);
    while (std::getline(file, line)) {
        if (line[0] == '>') {
            if (!seq_id.empty() && !sequence.empty()) {
                bool match = std::regex_search(sequence, re);
                results.push_back(std::make_tuple(seq_id, pattern, match));
                sequence.clear();
            }
            seq_id = line.substr(1, line.find(' ') - 1);
        } else {
            sequence += line;
        }
    }

    if (!seq_id.empty() && !sequence.empty()) {
        bool match = std::regex_search(sequence, re);
        results.push_back(std::make_tuple(seq_id, pattern, match));
    }

    file.close();
    return results;
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
        filename = argv[argc - 1];
        if (option == "--search" && argc == 4) {
            pattern = argv[2];
        }
    } else {
        show_help();
        return 1;
    }

    if (!file_exists(filename)) {
        std::cerr << "Error: File " << filename << " does not exist.\n";
        return 1;
    }

    if (option == "--search") {
        auto results = search_sequence(filename, pattern);
        for (const auto &result : results) {
            std::cout << std::get<0>(result) << " " << std::get<1>(result) << " " << (std::get<2>(result) ? "true" : "false") << "\n";
        }
    }

    return 0;
}
