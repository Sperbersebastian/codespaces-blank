#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>

class PexBan {
private:
    std::map<std::string, std::vector<std::string>> items;

public:
    PexBan() {
        items["todo"] = {};
        items["progress"] = {};
        items["done"] = {};
    }

    void box(const std::string& choice = "t", size_t size = 9) {
        if (items.find(choice) != items.end()) {
            std::cout << choice << " items:\n";
            for (size_t i = 0; i < std::min(items[choice].size(), size); ++i) {
                std::cout << "- " << items[choice][i] << "\n";
            }
        } else {
            std::cout << "Invalid choice!\n";
        }
    }

    void itemAdd(const std::string& list, const std::string& item) {
        if (items.find(list) != items.end()) {
            items[list].push_back(item);
            std::cout << "Item added to " << list << ": " << item << "\n";
        } else {
            std::cout << "Invalid list!\n";
        }
    }

    void itemDelete(const std::string& list, size_t index) {
        if (items.find(list) != items.end() && index < items[list].size()) {
            std::cout << "Item deleted from " << list << ": " << items[list][index] << "\n";
            items[list].erase(items[list].begin() + index);
        } else {
            std::cout << "Invalid list or index!\n";
        }
    }

    void menu() {
        std::string choice;
        size_t size = 9;
        std::string in;
        std::string delInput;
        while (true) {
            std::cout << "\n\033[32mPlease select \033[0m"
                      << "\033[32mq\033[0m(uit), "
                      << "\033[32mt\033[0m(odo), "
                      << "\033[32mp\033[0m(rogress), "
                      << "\033[32md\033[0m(one), "
                      << "\033[32ma\033[0m(ll), "
                      << "\033[32mad\033[0m(d item), "
                      << "\033[32mdel\033[0m(ete item), "
                      << "\033[32ms\033[0m(et size): ";
            std::cin >> choice;

            if (choice == "q") {
                break;
            } else if (choice == "s") {
                std::cout << " \033[32mEnter board size:\033[0m ";
                std::cin >> size;
            } else if (choice == "ad") {
                std::cout << "Enter item (list-abbreviation item name): ";
                std::cin.ignore();
                std::getline(std::cin, in);
                std::string list = in.substr(0, 1);
                std::string item = in.substr(2);
                itemAdd(list, item);
            } else if (choice == "del") {
                std::cout << "Enter list abbreviation and item index to delete (e.g., t 0): ";
                std::string list;
                size_t index;
                std::cin >> list >> index;
                itemDelete(list, index);
            } else if (choice == "a") {
                box("todo", size);
                box("progress", size);
                box("done", size);
            } else {
                box(choice, size);
            }
        }
    }
};

int main(int argc, char **argv) {
    PexBan kanban;
    kanban.menu();
    return 0;
}