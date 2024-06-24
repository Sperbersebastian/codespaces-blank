/// usr/bin/g++ filename.cpp -o filename && exec ./filename
/*
 * cpp-app.cpp
 *
 * Copyright 2024 kiosk-user <kiosk-user@node-001-1>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 *
 *
 */

#include <algorithm>
#include <cstdio>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <regex>
#ifdef __cpp_lib_filesystem
#include <filesystem>
namespace fs = std::filesystem;
#elif __cpp_lib_experimental_filesystem
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#else
#include "include/ghc/filesystem.hpp"
namespace fs = ghc::filesystem;
#endif

#include "pex.hpp"
#include "include/popl.hpp"
class PexBan {
private:
    std::map<std::string, std::vector<std::string>> items;
    std::map<std::string, std::string> mymap;
    fs::path itemFile; 

public:
    PexBan() {
        mymap["t"] = "TODO";
        mymap["p"] = "PROGRESS";
        mymap["d"] = "DONE";
        std::string home = pex::get_home_directory();
        // Create the folder path
        fs::path homeDir(home);
        fs::path pexbanPath = homeDir / ".config" / "pexban";
        fs::path itemFile =  pexbanPath / "items.txt";
        std::cout << itemFile << std::endl;
        if (!fs::is_directory(pexbanPath)) {
            //std::cout << pexbanPath << std::endl;
            fs::create_directory(pexbanPath);
        }
        this->itemFile = itemFile;

    }
    void itemAdd(std::string list, std::string item) {
        if (mymap.count(list) == 1) {
            items[list].push_back(item);
        }
        std::cout << "list: '" << list << "' add item: '" << item << "'"
                  << std::endl;
    }
    void itemDelete(std::string list, int itemIndex) {
        if (mymap.count(list) == 1) {
            if (items[list].size() >= itemIndex) {

                items[list].erase(items[list].begin() + itemIndex - 1);
            }
        }
        std::cout << "list: '" << list << "' deleted item: '" << itemIndex << "'"
                  << std::endl;
    }
    void itemSave () {
        std::ofstream fout;
        fout.open(itemFile);
        // hw03
        // TODO: remove duplicates
        
        for (const auto& [key, its] : items) {
            for (auto item : its) {
                fout << key << " " << item << "\n";
            }
        }
        fout.close();
    }
    // hw03
    void itemLoad () {
        std::ifstream fin;
        std::string line;
        if (fs::exists(itemFile)) {
            fin.open(itemFile);
            while (std::getline(fin,line)) {
                std::cout << line << std::endl;
                if (line.find("t") == 0) {
                    items["t"].push_back(line.substr(2));
                } else if (line.find("p") == 0) {
                    items["p"].push_back(line.substr(2));
                } else if (line.find("d") == 0) {
                    items["d"].push_back(line.substr(2));
                }
            }
            fin.close();
        }
    }
    void itemMove (std::string from, std::string to, int itemIndex) {
        std::string selItem = items[from].at(itemIndex-1);
        itemDelete(from, itemIndex);
        itemAdd(to,selItem);

    }
    void hilight (std::string choice, int i) {
        std::string currentItem = "";
        if (items[choice].size() > i) {
            currentItem = items[choice][i];
        }
        if (currentItem.find("+") != 0) {
            printf("  | %2i %-16s |  ", i + 1, currentItem.c_str());
        } else {
            printf("  | %2i \033[31m%-16s\033[0m |  ", i + 1, currentItem.substr(2).c_str());
        }

    }
    void box(std::string choice = "t", int size = 9) {
        std::string border = "  +---------------------+ ";

        if (mymap.count(choice) == 0) {
            if (choice == "all" || choice == "a") {
                std::cout << "       \033[31mo\033[0m TODO                      ";
                std::cout << "  \033[33mo\033[0m PROGRESS                   "
                          "  \033[32mo\033[0m DONE\n";
                std::cout << border << "   " << border << "   " << border << "\n";
                for (int i = 0; i < size; i++) {
                    hilight("t",i); // TODO
                    printf("  ");
                    hilight("p",i); // PROGRESS
                    printf("  ");
                    hilight("d",i); // DONE
                    printf("\n");
                }
                std::cout << border << "   " << border << "   " << border << "\n";
            } else {
                std::cout << "invalid choice '" << choice << "'\n";
            }
        } else {
            std::cout << "        " << mymap[choice] << "\n";
            std::cout << border << "\n";
            for (int i = 0; i < size; i++) {
                hilight(choice,i);
                printf("\n");
            }

            std::cout << border << "\n";
        }
    }
    void menu() {
        std::string choice = "";
        int size = 9;
        while (true) {
            std::cout << "\n\033[32mPlease select \033[0m ";
            std::cout << "\033[32mq\033[0m(uit), ";
            std::cout << "\033[32mt\033[0m(odo), ";
            std::cout << "\033[32mp\033[0m(progress), ";
            std::cout << "\033[32md\033[0m(one), ";
            std::cout << "\033[32ma\033[0m(ll), ";
            std::cout << "\033[32mad\033[0m(d items), ";
            std::cout << "\033[32mde\033[0m(l items), ";
            std::cout << "\033[32mm\033[0m(ove items), ";            

            std::cout << "\033[32ms\033[0m(et size): ";

            std::cin >> choice;
            if (choice == "q") {
                break;
            } else if (choice == "s") {
                std::cout << " \033[32mEnter board size:\033[0m ";
                std::cin >> size;
            } else if (choice == "ad") {
                std::cout << "Enter item (list-abbreviation item-name): ";
                std::string in;
                std::cin.ignore();          // clear remaining chars
                std::getline(std::cin, in); // get line with spaces
                std::cout << in;
                itemAdd(in.substr(0, 1), in.substr(2));
            } else if (choice == "de") {
                std::cout << "Enter item (list-abbreviation item-index): ";
                std::string in;
                std::cin.ignore();          // clear remaining chars
                std::getline(std::cin, in); // get line with spaces
                std::cout << in;
                itemDelete(in.substr(0, 1), std::stoi(in.substr(2)));
            } else if (choice == "m") {
                std::cout << "Enter item (from-list to-list item-index): ";
                std::string in;
                std::cin.ignore();          // clear remaining chars
                std::getline(std::cin, in); // get line with spaces
                std::cout << in;
                std::string item = items[in.substr(0,1)].at(std::stoi(in.substr(4))-1);
                std::cout << "item is: " << item << " idx1 '" << in.substr(0, 1) << "'\n" ;
                itemMove(in.substr(0, 1),in.substr(2, 1), std::stoi(in.substr(4)));
            } else {
                box(choice, size);
            }
        }
    }
};

int main(int argc, char **argv) {
	popl::OptionParser pexapp("pexban application\nUsage: pexban [options] ?itemsfile?\nOptions");
    auto help   = pexapp.add<popl::Switch>("h", "help",
        "produce help message");
	auto interactive   = pexapp.add<popl::Switch>("i", "interactive",
        "start interactive menu"); 
    auto add = pexapp.add<popl::Value<std::string>>("a", "add", 
    "add items to list 't|p|d itemname'","");      
    pexapp.parse(argc, argv);
    if (help->is_set()) {
        std::cout << pexapp << "\n";
        return(0);
    } 
    auto pex = PexBan();
    //pex.itemAdd("t", "PEX Homework");
    //pex.itemAdd("t", "MLB Quiz");
    pex.itemLoad();
    if (interactive->is_set()) {
		pex.menu();
		pex.itemSave();
	} else if (add->is_set()) {
		if (std::regex_match(add->value(),std::regex("[tpd] .+"))) {
			std::cout << "setting: " << add->value() << "\n";
			pex.itemAdd(add->value().substr(0,1),add->value().substr(2));
			pex.box("all");
		} else {
			std::cout << "Some more error!\n";
		}
	} else {
		pex.box("all");
		std::cout << "Run: " << argv[0] << " --help to see help!\n"; 
	}
    return 0;
}
