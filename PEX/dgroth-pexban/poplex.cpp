#include <iostream>
#include <iomanip>
#include <regex>
#include "include/popl.hpp"
using namespace std;
using namespace popl;
int main (int argc, const char * argv[]) {
    OptionParser app("poplex application\nUsage: poplex [options] number\nOptions");
    auto help   = app.add<Switch>("h", "help",
        "produce help message");
    auto verbose   = app.add<Switch>("v", "verbose",
        "set verbose on");
    auto round = app.add<Value<int>>("r", "round", "rounding digits",2);
    app.parse(argc, argv);

    // print auto-generated help message
    if (help->is_set()) {
        cout << app << "\n";
        return(0);
    } else if (verbose->is_set()) {
        cout << "verbose is on\n";
    }
    // show unknown options (undefined ones, like "-u" or "--undefined")
    for (const auto& unknown_option: app.unknown_options()) {
        cout << "Error: unknown option: " << unknown_option << "\n";
        return(0);
    }
    // positionals
    float number = 0;
    for (const auto& arg: app.non_option_args()) {
        if (std::regex_match(arg,std::regex("[-+.0-9]+"))) {
              number = std::stof(arg);
              std::cout << "square of: " <<  number << " is " <<
              std::fixed <<
              std::setprecision(round->value()) <<
              number*number <<  std::endl;
        } else {
            std::cout << "Error: " << arg << " is not a number!\n";
        }
    }
    if (number == 0) {
        std::cout << app.help();
    }
}
