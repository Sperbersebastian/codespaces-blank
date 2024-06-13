#include "pex.h"
#include <cstdlib>
#include <string>
namespace pex {
    std::string get_home_directory() {
        // Get home directory Unix
        const char* homeDir = getenv("HOME");
        // Get home directory on Windows (untested)
        const char* userProfile = getenv("USERPROFILE");
        std::string result = "";
        if (homeDir != nullptr) {
            result = homeDir;
        } else if (userProfile != nullptr) {
            result = userProfile;
        }
        return result;
    }
}

#ifndef PEX_H
    int main() {
    std::cout << "PEX Home: " << pex::get_home_directory()
        << std::endl;
    return 0;
}
#endif // PEX_H
