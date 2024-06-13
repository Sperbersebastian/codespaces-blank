#include "pex.h"
#include <iostream>
#include <cstdlib>

namespace pex {
    std::string get_home_directory () {
    // get home directory Unix
    const char* homeDir = getenv("HOME");
    // Get home directory on Windows (untested)
    const char* userProfile = getenv("USERPROFILE");
    std::string result = "";
    if (homeDir != nullptr) {
        result = homeDir;
    } else if (userProfile != nullptr) {
        result = userProfile;
    }
    return(result);
}
}

#ifdef PEX_MAIN
int main() {
    std::cout << "PEX Home: " << pex::get_home_directory()
        << std::endl;
    return(0);
}
#endif
