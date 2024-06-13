#include "PexBan.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <sys/types.h>
#include <pwd.h>
#include <unistd.h>

namespace pex {
    std::string get_home_directory() {
        const char *homedir;
        if ((homedir = getenv("HOME")) == nullptr) {
            homedir = getpwuid(getuid())->pw_dir;
        }
        return std::string(homedir);
    }
}

PexBan::PexBan() {
    itemsfile = pex::get_home_directory() + "/.config/pexban/items.txt";
    itemLoad();
}

PexBan::~PexBan() {
    itemSave();
}

void PexBan::itemSave() {
    std::ofstream outfile(itemsfile);
    for (const auto& item : todo) {
        outfile << "t " << item << std::endl;
    }
    for (const auto& item : progress) {
        outfile << "p " << item << std::endl;
    }
    for (const auto& item : done) {
        outfile << "d " << item << std::endl;
    }
    outfile.close();
}

void PexBan::itemLoad() {
    std::ifstream infile(itemsfile);
    std::string line;
    while (std::getline(infile, line)) {
        if (line[0] == 't') {
            todo.push_back(line.substr(2));
        } else if (line[0] == 'p') {
            progress.push_back(line.substr(2));
        } else if (line[0] == 'd') {
            done.push_back(line.substr(2));
        }
    }
    infile.close();
}
