#ifndef PEXBAN_H
#define PEXBAN_H

#include <string>
#include <vector>

class PexBan {
public:
    PexBan();
    ~PexBan();
    void itemSave();
    void itemLoad();
    void itemLeft(std::string choice, int itemIndex);
    void itemRight(std::string choice, int itemIndex);

private:
    std::vector<std::string> todo;
    std::vector<std::string> progress;
    std::vector<std::string> done;
    std::string itemsfile;
};

#endif // PEXBAN_H
