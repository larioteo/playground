module;
#include <iostream>

export module Module;

import Cli;

export namespace app {

void log(const char *message) {
    std::cout << message << std::endl;

    app::cli::Test();
}

}
