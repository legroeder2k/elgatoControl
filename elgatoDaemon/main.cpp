#include <iostream>
#include <string>
#include "Log.h"
#include "AvahiBrowser.h"

int main(int argc, char*argv[]) {
    std::clog.rdbuf(new Log("elgatoDaemon", LOG_LOCAL0));
    std::clog << kLogNotice << "Elgato daemon starting." << std::endl;

    AvahiBrowser::getInstance().start();

    std::string line;
    std::cout << "Press RETURN to exit." << std::endl;
    std::getline(std::cin, line);

    while (line != "q") {

        if (line == "p") {
            for(auto& it : AvahiBrowser::getInstance().getLights()) {
                std::cout << "Found: " << it->name << std::endl;
            }
        }

        std::getline(std::cin, line);
    }

    return 0;
}