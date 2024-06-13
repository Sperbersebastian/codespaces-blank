#include <iostream>
#include <string>

// Forward declaration of pex namespace
namespace pex {
    std::string get_home_directory();
}

class PexBan {
public:
    void itemSave() {
        std::string home = pex::get_home_directory();
        //Create the folder path
        fs::path homeDir(home);
        fs::path pexbanPath = homeDir / ".config" / "pexban";
        fs::path itemFile = pexbanPath / "items.txt";
        std::count << itemFile << std::endl;
        if (!fs::is_directory(pexbanPath)) {
            std::cout << pexbanPath << std::endl;
            fs::create_directories(pexbanPath);
        }

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
