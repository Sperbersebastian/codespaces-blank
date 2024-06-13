#include <iostream>
#include <version>

// Check for older compilers
#ifdef __cpp_lib_filesystem
    #include <filesystem>
    namespace fs = std::filesystem;
#elif __cpp_lib_experimental_filesystem
    #include <experimental/filesystem>
    namespace fs = std::experimental::filesystem;
#else
    #error "no filesystem support ='("
#endif

int main() {
    // Print the current working directory
    std::cout << "current_path:\n" << fs::current_path() <<
    "\nExists Makefile?: " << fs::exists("../Makefile") <<
    std::endl;

    return 0;
}
