#ifndef PEXBAN_H
#define PEXBAN_H

#include <string>
#include <vector>

class PexBan {
public:
    void itemSave();
    void itemLoad();
    PexBan();
    ~PexBan();

private:
    std::vector<std::string> todo;
    std::vector<std::string> progress;
    std::vector<std::string> done;
    std::string itemsfile;
};

#endif // PEXBAN_H
