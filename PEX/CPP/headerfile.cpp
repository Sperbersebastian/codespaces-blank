#include <iostream>
#include <string>

// Forward declaration of pex namespace
namespace pex {
    std::string get_home_directory();
}

class PexBan {
public:
    void itemSave() {
        std::cout << "itemSave method called." << std::endl;
        std::cout << "Home Directory: " << pex::get_home_directory() << std::endl;
    }
    
    void interactiveMenu() {
        char choice;
        while (true) {
            std::cout << "(s)ave: Save item" << std::endl;
            std::cout << "(q)uit: Quit" << std::endl;
            std::cout << "Choose an option: ";
            std::cin >> choice;
            if (choice == 's') {
                itemSave();
            } else if (choice == 'q') {
                break;
            } else {
                std::cout << "Invalid option." << std::endl;
            }
        }
    }
};

int main() {
    PexBan pb;
    pb.interactiveMenu();
    return 0;
}
