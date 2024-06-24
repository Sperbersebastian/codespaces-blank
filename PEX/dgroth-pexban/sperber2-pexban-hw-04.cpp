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
    bool verbose;  // Add the verbose member variable

public:
    PexBan() : verbose(false) {  // Initialize verbose to false
        mymap["t"] = "TODO";
        mymap["p"] = "PROGRESS";
        mymap["d"] = "DONE";
        std::string home = pex::get_home_directory();
        fs::path homeDir(home);
        fs::path pexbanPath = homeDir / ".config" / "pexban";
        fs::path itemFile =  pexbanPath / "items.txt";
        std::cout << itemFile << std::endl;
        if (!fs::is_directory(pexbanPath)) {
            fs::create_directory(pexbanPath);
        }
        this->itemFile = itemFile;
    }

    void setVerbose(bool v) {  // Method to set the verbose flag
        verbose = v;
    }

    void itemAdd(std::string list, std::string item) {
        if (mymap.count(list) == 1) {
            items[list].push_back(item);
        }
        if (verbose) {
            std::cout << "list: '" << list << "' add item: '" << item << "'" << std::endl;
        }
        box("all");
    }

    void itemDelete(std::string list, int itemIndex) {
        if (mymap.count(list) == 1) {
            if (items[list].size() >= itemIndex) {
                items[list].erase(items[list].begin() + itemIndex - 1);
            }
        }
        if (verbose) {
            std::cout << "list: '" << list << "' deleted item: '" << itemIndex << "'" << std::endl;
        }
        box("all");
    }

    void itemMove(std::string from, std::string to, int itemIndex) {
        std::string selItem = items[from].at(itemIndex-1);
        itemDelete(from, itemIndex);
        itemAdd(to, selItem);
    }

    void itemSave () {
        std::ofstream fout;
        fout.open(itemFile);
        for (const auto& [key, its] : items) {
            for (auto item : its) {
                fout << key << " " << item << "\n";
            }
        }
        fout.close();
    }

    void itemLoad () {
        std::ifstream fin;
        std::string line;
        if (fs::exists(itemFile)) {
            fin.open(itemFile);
            while (std::getline(fin, line)) {
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

    void hilight(std::string choice, int i) {
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
                    hilight("t",i);
                    printf("  ");
                    hilight("p",i);
                    printf("  ");
                    hilight("d",i);
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
                std::cin.ignore();
                std::getline(std::cin, in);
                itemAdd(in.substr(0, 1), in.substr(2));
            } else if (choice == "de") {
                std::cout << "Enter item (list-abbreviation item-index): ";
                std::string in;
                std::cin.ignore();
                std::getline(std::cin, in);
                itemDelete(in.substr(0, 1), std::stoi(in.substr(2)));
            } else if (choice == "m") {
                std::cout << "Enter item (from-list to-list item-index): ";
                std::string in;
                std::cin.ignore();
                std::getline(std::cin, in);
                itemMove(in.substr(0, 1), in.substr(2, 1), std::stoi(in.substr(4)));
            } else {
                box(choice, size);
            }
        }
    }
};

int main(int argc, char **argv) {
    popl::OptionParser pexapp("pexban application\nUsage: pexban [options] ?itemsfile?\nOptions");
    auto help = pexapp.add<popl::Switch>("h", "help", "produce help message");
    auto interactive = pexapp.add<popl::Switch>("i", "interactive", "start interactive menu"); 
    auto add = pexapp.add<popl::Value<std::string>>("a", "add", "add items to list 't|p|d itemname'", "");      
    auto del = pexapp.add<popl::Value<std::string>>("d", "del", "delete items from list 't|p|d item-index'", "");      
    auto move = pexapp.add<popl::Value<std::string>>("m", "mv", "move items 'from-list to-list item-index'", "");      
    auto verboseFlag = pexapp.add<popl::Switch>("v", "verbose", "enable verbose output");

    pexapp.parse(argc, argv);

    if (help->is_set()) {
        std::cout << pexapp << "\n";
        return 0;
    } 

    PexBan pex;
    pex.itemLoad();

    if (verboseFlag->is_set()) {
        pex.setVerbose(true);
    }

    if (interactive->is_set()) {
        pex.menu();
        pex.itemSave();
    } else if (add->is_set()) {
        std::string value = add->value();
        auto pos = value.find(' ');
        if (pos != std::string::npos) {
            pex.itemAdd(value.substr(0, pos), value.substr(pos + 1));
        } else {
            std::cout << "Invalid format for add command!\n";
        }
    } else if (del->is_set()) {
        std::string value = del->value();
        auto pos = value.find(' ');
        if (pos != std::string::npos) {
            pex.itemDelete(value.substr(0, pos), std::stoi(value.substr(pos + 1)));
        } else {
            std::cout << "Invalid format for del command!\n";
        }
    } else if (move->is_set()) {
        std::string value = move->value();
        auto pos1 = value.find(' ');
        auto pos2 = value.find(' ', pos1 + 1);
        if (pos1 != std::string::npos && pos2 != std::string::npos) {
            pex.itemMove(value.substr(0, pos1), value.substr(pos1 + 1, pos2 - pos1 - 1), std::stoi(value.substr(pos2 + 1)));
        } else {
            std::cout << "Invalid format for move command!\n";
        }
    } else {
        pex.box("all");
        std::cout << "Run: " << argv[0] << " --help to see help!\n"; 
    }

    return 0;
}
