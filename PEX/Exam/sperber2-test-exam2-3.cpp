#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <map>

// Function to show the help message
void show_help() {
    std::cout << "Detlef’s FastaParser1 searches FASTA files\n";
    std::cout << "Author: Detlef Groth, University of Potsdam, 2024\n";
    std::cout << "Usage: FastaParser1 --search|--summary|--help ?PATTERN? file.fasta [...fileN.fasta]\n";
}

// Function to check if a file exists
bool file_exists(const std::string &filename) {
    std::ifstream file(filename);
    return file.good();
}

// Function to check if a string is a valid amino acid sequence pattern
bool is_valid_amino_acid_pattern(const std::string &pattern) {
    std::regex amino_acid_re("^[ACDEFGHIKLMNPQRSTVWYX]+$");
    return std::regex_match(pattern, amino_acid_re);
}

// Function to search sequences in a FASTA file
std::vector<std::tuple<std::string, std::string, bool>> search_sequence(const std::string &filename, const std::string &pattern) {
    std::vector<std::tuple<std::string, std::string, bool>> results;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << "\n";
        return results;
    }

    std::string line, seq_id;
    std::regex re(pattern);
    while (std::getline(file, line)) {
        if (line[0] == '>') {
            seq_id = line.substr(1);
        } else {
            bool match = std::regex_search(line, re);
            results.push_back(std::make_tuple(seq_id, pattern, match));
        }
    }
    file.close();
    return results;
}

// Function to summarize sequence lengths in multiple FASTA files
std::map<std::string, int> summarize_sequences(const std::vector<std::string> &filenames) {
    std::map<std::string, int> summary;
    for (const auto &filename : filenames) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error opening file: " << filename << "\n";
            continue;
        }

        std::string line, seq_id;
        while (std::getline(file, line)) {
            if (line[0] == '>') {
                seq_id = line.substr(1);
                summary[seq_id] = 0;
            } else {
                summary[seq_id] += line.size();
            }
        }
        file.close();
    }
    return summary;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        show_help();
        return 1;
    }

    std::string option = argv[1];
    std::vector<std::string> filenames;
    std::string pattern;

    if (option == "--help") {
        show_help();
        return 0;
    } else if (option == "--search" || option == "--summary") {
        if (argc < 3) {
            show_help();
            return 1;
        }
        for (int i = 2; i < argc; ++i) {
            std::string arg = argv[i];
            if (option == "--search" && i == argc - 1) {
                if (is_valid_amino_acid_pattern(arg)) {
                    pattern = arg;
                } else if (arg.substr(arg.find_last_of(".") + 1) == "fasta") {
                    filenames.push_back(arg);
                } else {
                    std::cerr << "Error: Invalid pattern or filename: " << arg << "\n";
                    return 1;
                }
            } else if (arg.substr(arg.find_last_of(".") + 1) == "fasta") {
                filenames.push_back(arg);
            } else {
                std::cerr << "Error: Invalid filename: " << arg << "\n";
                return 1;
            }
        }
    } else {
        show_help();
        return 1;
    }

    for (const auto &filename : filenames) {
        if (!file_exists(filename)) {
            std::cerr << "Error: File " << filename << " does not exist.\n";
            return 1;
        }
    }

    if (option == "--search") {
        for (const auto &filename : filenames) {
            auto results = search_sequence(filename, pattern);
            for (const auto &result : results) {
                std::cout << std::get<0>(result) << " " << std::get<1>(result) << " " << (std::get<2>(result) ? "true" : "false") << "\n";
            }
        }
    } else if (option == "--summary") {
        auto summary = summarize_sequences(filenames);
        for (const auto &entry : summary) {
            std::cout << entry.first << " " << entry.second << "\n";
        }
    }

    return 0;
}
