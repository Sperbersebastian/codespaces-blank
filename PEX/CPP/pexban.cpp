#include <algorithm>
#include <cstdio>
#include <iostream>
#include <map>
#include <string>
#include <vector>
class PexBan {
private:
  std::map<std::string, std::vector<std::string>> items;
  std::map<std::string, std::string> mymap;

public:
  PexBan() {
    mymap["t"] = "TODO";
    mymap["p"] = "PROGRESS";
    mymap["d"] = "DONE";
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

      std::cout << "\033[32ms\033[0m(et size): ";

      std::cin >> choice;
      if (choice == "q") {
        break;
      } else if (choice == "s") {
        std::cout << " \033[32mEnter board size:\033[0m ";
        std::cin >> size;
      } else if (choice == "ad") {
        std::cout << "Enter item (list-abbreviation item name): ";
        std::string in;
        std::cin.ignore();          // clear remaining chars
        std::getline(std::cin, in); // get line with spaces
        std::cout << in;
        itemAdd(in.substr(0, 1), in.substr(2));
      } else if (choice == "de") {
        std::cout << "Enter item (list-abbreviation item index): ";
        std::string in;
        std::cin.ignore();          // clear remaining chars
        std::getline(std::cin, in); // get line with spaces
        std::cout << in;
        itemDelete(in.substr(0, 1), std::stoi(in.substr(2)));
      } else {
        box(choice, size);
      }
    }
  }
};
int main(int argc, char **argv) {
  auto pex = PexBan();
  pex.itemAdd("t", "PEX Homework");
  pex.itemAdd("t", "MLB Quiz");
  pex.menu();
  return 0;
}
