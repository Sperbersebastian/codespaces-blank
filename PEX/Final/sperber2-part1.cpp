#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

void show_help() {
    std::cout << "Usage: appname [--help, --get-metacyc MetaCyc-ID, --tab-metacyc] OBOFILE\n";
    std::cout << "Options:\n";
    std::cout << "  --help               Show this help message\n";
    std::cout << "  --get-metacyc        Get information for the specified MetaCyc ID\n";
    std::cout << "  --tab-metacyc        Generate a tab-delimited report from the OBO file\n";
    std::cout << "Arguments:\n";
    std::cout << "  OBOFILE              Path to the GO-obo input file\n";
    std::cout << "  MetaCyc-ID           Optional MetaCyc identifier (required for --get-metacyc)\n";
}

bool is_valid_file(const std::string& filename) {
    std::ifstream infile(filename);
    return infile.good();
}

bool is_valid_metacyc_id(const std::string& id) {
    return id.find("RXN") != std::string::npos || id.find("PWY") != std::string::npos;
}

int main(int argc, char* argv[]) {
    if (argc < 2 || argc > 4) {
        show_help();
        return 1;
    }

    std::string obofile;
    std::string task;
    std::string metacyc_id;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--help") {
            show_help();
            return 0;
        } else if (arg == "--get-metacyc") {
            if (i + 1 < argc) {
                task = "--get-metacyc";
                metacyc_id = argv[++i];
                if (!is_valid_metacyc_id(metacyc_id)) {
                    std::cerr << "Invalid MetaCyc ID. It must contain 'RXN' or 'PWY'.\n";
                    return 1;
                }
            } else {
                std::cerr << "--get-metacyc requires a MetaCyc ID.\n";
                return 1;
            }
        } else if (arg == "--tab-metacyc") {
            task = "--tab-metacyc";
        } else {
            obofile = arg;
        }
    }

    if (obofile.empty()) {
        std::cerr << "OBOFILE is required.\n";
        show_help();
        return 1;
    }

    if (!is_valid_file(obofile)) {
        std::cerr << "The file " << obofile << " does not exist.\n";
        return 1;
    }

    // Process the file and execute the task here
    // ...

    std::cout << "Task: " << task << "\n";
    std::cout << "OBO File: " << obofile << "\n";
    if (task == "--get-metacyc") {
        std::cout << "MetaCyc ID: " << metacyc_id << "\n";
    }

    return 0;
}
