#include <iostream>
#include <vector>
#include <string>
#include <map>

class PexBan {
private:
    std::map<std::string, std::vector<std::string>> items;

public:
    void drawBox(const std::string& title, int lines = 9, int width = 20) {
        std::cout << "         " << title << std::endl;
        std::cout << " +" << std::string(width, '-') << "+" << std::endl;
        for (int i = 1; i <= lines; ++i) {
            if (i <= items[title].size()) {
                std::cout << " | " << items[title][i-1] << std::string(width - items[title][i-1].length() - 1, ' ') << "|" << std::endl;
            } else {
                std::cout << " | " << i << std::string(width - std::to_string(i).length() - 1, ' ') << "|" << std::endl;
            }
        }
        std::cout << " +" << std::string(width, '-') << "+" << std::endl;
    }

    void drawFullKanban(int lines) {
        std::cout << "         TODO                    PROGRESS                   DONE" << std::endl;
        std::cout << " +" << std::string(20, '-') << "+    +" << std::string(20, '-') << "+   +" << std::string(20, '-') << "+" << std::endl;
        for (int i = 1; i <= lines; ++i) {
            std::string todo = (i <= items["TODO"].size()) ? items["TODO"][i-1] : std::to_string(i);
            std::string progress = (i <= items["PROGRESS"].size()) ? items["PROGRESS"][i-1] : std::to_string(i);
            std::string done = (i <= items["DONE"].size()) ? items["DONE"][i-1] : std::to_string(i);

            std::cout << " | " << todo << std::string(18 - todo.length(), ' ') << "|    "
                      << "| " << progress << std::string(18 - progress.length(), ' ') << "|   "
                      << "| " << done << std::string(18 - done.length(), ' ') << "|" << std::endl;
        }
        std::cout << " +" << std::string(20, '-') << "+    +" << std::string(20, '-') << "+   +" << std::string(20, '-') << "+" << std::endl;
    }

    void itemAdd(const std::string& list, const std::string& item) {
        items[list].push_back(item);
    }

    void itemDelete(const std::string& list, int index) {
        if (items.find(list) != items.end() && index >= 0 && index < items[list].size()) {
            items[list].erase(items[list].begin() + index);
        } else {
            std::cout << "Invalid list or index\n";
        }
    }

    void menu() {
        char choice;
        int lines = 9;
        do {
            std::cout << "\nMenu:\n";
            std::cout << "s: set board size\n";
            std::cout << "t: display TODO\n";
            std::cout << "p: display PROGRESS\n";
            std::cout << "d: display DONE\n";
            std::cout << "a: display full kanban\n";
            std::cout << "ad: add item\n";
            std::cout << "dt: delete item\n";
            std::cout << "q: quit application\n";
            std::cout << "Please enter your choice: ";
            std::cin >> choice;

            switch (choice) {
                case 's':
                    std::cout << "Enter number of lines: ";
                    std::cin >> lines;
                    break;
                case 't':
                    drawBox("TODO", lines);
                    break;
                case 'p':
                    drawBox("PROGRESS", lines);
                    break;
                case 'd':
                    drawBox("DONE", lines);
                    break;
                case 'a':
                    drawFullKanban(lines);
                    break;
                case 'a':
                    std::cout << "Enter item (list item name): ";
                    std::string in;
                    std::cin.ignore(); // clear remaining chars
                    std::getline(std::cin, in); // get line with spaces
                    itemAdd(in.substr(0, 1), in.substr(2));
                    drawFullKanban(lines);
                    break;
                case 'd':
                    std::cout << "Enter item to delete (list index): ";
                    std::string delInput;
                    std::cin.ignore(); // clear remaining chars
                    std::getline(std::cin, delInput); // get line with spaces
                    itemDelete(delInput.substr(0, 1), std::stoi(delInput.substr(2)));
                    drawFullKanban(lines);
                    break;
                case 'q':
                    std::cout << "Exiting...\n";
                    break;
                default:
                    std::cout << "Invalid choice\n";
                    break;
            }
        } while (choice != 'q');
    }
};

int main(int argc, char **argv) {
    PexBan kanban;
    kanban.menu();
    return 0;
}