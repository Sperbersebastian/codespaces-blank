#include "PexBan.h"
#include <iostream>

int main() {
    PexBan pexban;

    char choice;
    while (true) {
        std::cout << "Enter choice (s: save, l: load, m: move, q: quit): ";
        std::cin >> choice;
        if (choice == 'q') break;
        switch (choice) {
            case 's':
                pexban.itemSave();
                break;
            case 'l':
                pexban.itemLoad();
                break;
            case 'm': {
                std::string move_choice;
                int index;
                std::cout << "Enter move choice (t p 2 to move 2nd todo to progress): ";
                std::cin >> move_choice >> index;
                if (move_choice == "t" || move_choice == "p") {
                    pexban.itemRight(move_choice, index - 1);
                } else if (move_choice == "p" || move_choice == "d") {
                    pexban.itemLeft(move_choice, index - 1);
                }
                break;
            }
        }
    }
    return 0;
}
