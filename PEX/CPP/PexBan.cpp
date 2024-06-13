#include "PexBan.h"
#include "pex.h"
#include <iostream>
#include <fstream>

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

void PexBan::itemLeft(std::string choice, int itemIndex) {
    if (choice == "p" && itemIndex < progress.size()) {
        todo.push_back(progress[itemIndex]);
        progress.erase(progress.begin() + itemIndex);
    } else if (choice == "d" && itemIndex < done.size()) {
        progress.push_back(done[itemIndex]);
        done.erase(done.begin() + itemIndex);
    }
}

void PexBan::itemRight(std::string choice, int itemIndex) {
    if (choice == "t" && itemIndex < todo.size()) {
        progress.push_back(todo[itemIndex]);
        todo.erase(todo.begin() + itemIndex);
    } else if (choice == "p" && itemIndex < progress.size()) {
        done.push_back(progress[itemIndex]);
        progress.erase(progress.begin() + itemIndex);
    }
}
