#include <iostream>
#include <string>

void drawBox(const std::string& title, int lines = 9, int width = 20) {
    std::cout << "         " << title << std::endl;
    std::cout << " +" << std::string(width, '-') << "+" << std::endl;
    for (int i = 1; i <= lines; ++i) {
        std::cout << " | " << i << std::string(width - std::to_string(i).length() - 1, ' ') << "|" << std::endl;
    }
    std::cout << " +" << std::string(width, '-') << "+" << std::endl;
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
            case 'q':
                std::cout << "Exiting...\n";
                break;
            default:
                std::cout << "Invalid choice\n";
                break;
        }
    } while (choice != 'q');
}


void drawFullKanban(int lines) {
    std::cout << "         TODO                    PROGRESS                   DONE" << std::endl;
    std::cout << " +" << std::string(20, '-') << "+    +" << std::string(20, '-') << "+   +" << std::string(20, '-') << "+" << std::endl;
    for (int i = 1; i <= lines; ++i) {
        std::cout << " | " << i << std::string(18 - std::to_string(i).length(), ' ') << "|    ";
        std::cout << "| " << i << std::string(18 - std::to_string(i).length(), ' ') << "|   ";
        std::cout << "| " << i << std::string(18 - std::to_string(i).length(), ' ') << "|" << std::endl;
    }
    std::cout << " +" << std::string(20, '-') << "+    +" << std::string(20, '-') << "+   +" << std::string(20, '-') << "+" << std::endl;
}


int main() {
    menu();
    return 0;
}