#include <iostream>
#include <string>
#include "../Config.h"
#include "Log.h"
#include "AvahiBrowser.h"
#include "ElgatoServerImpl.h"

int main([[maybe_unused]]int argc, [[maybe_unused]]char*argv[]) {
    std::clog.rdbuf(new Log("elgatoDaemon", LOG_LOCAL0));
    std::clog << kLogNotice << "Elgato daemon starting." << std::endl;

    AvahiBrowser::getInstance().start();

    ElgatoServerImpl elgatoServer;
    elgatoServer.RunServer(SOCKET_FILE);

    std::string line;
    std::cout << "Console interface:\n  q -> quit, l -> list all devices, s -> status, 1 -> on, 0 -> off, b -> 25% brightness, B -> 100% brightness, t -> 4000K temp, T -> 7000K temp" << std::endl;
    std::getline(std::cin, line);

    while (line != "q") {

        if (line == "r") {
            AvahiBrowser::getInstance().restart();
        }

        if (line == "l") {
            for(auto& it : AvahiBrowser::getInstance().getLights()) {
                std::cout << "Found: " << it->name() << std::endl;
            }
        }

        if (line == "s" && !AvahiBrowser::getInstance().getLights().empty()) {
            auto light = AvahiBrowser::getInstance().getLights().at(0);

            if (light->isReady()) {
                std::cout << light->deviceInfo()->displayName << " is currently " <<
                (light->deviceState()->on ? "on" : "off") << " at a temperature of " <<
                ElgatoLight::colorFromElgato(light->deviceState()->temperature) <<
                " and a brightness of " << std::to_string(light->deviceState()->brightness) << std::endl;
            }

        }

        if (line == "1" && !AvahiBrowser::getInstance().getLights().empty()) {
            auto light = AvahiBrowser::getInstance().getLights().at(0);

            if (light->isReady())
                light->powerOn();
        }

        if (line == "0" && !AvahiBrowser::getInstance().getLights().empty()) {
            auto light = AvahiBrowser::getInstance().getLights().at(0);

            if (light->isReady())
                light->powerOff();
        }

        if (line == "b" && !AvahiBrowser::getInstance().getLights().empty()) {
            auto light = AvahiBrowser::getInstance().getLights().at(0);

            if (light->isReady())
                light->setBrightness(25);
        }

        if (line == "B" && !AvahiBrowser::getInstance().getLights().empty()) {
            auto light = AvahiBrowser::getInstance().getLights().at(0);

            if (light->isReady())
                light->setBrightness(100);
        }

        if (line == "t" && !AvahiBrowser::getInstance().getLights().empty()) {
            auto light = AvahiBrowser::getInstance().getLights().at(0);

            if (light->isReady())
                light->setTemperature(4000);
        }

        if (line == "T" && !AvahiBrowser::getInstance().getLights().empty()) {
            auto light = AvahiBrowser::getInstance().getLights().at(0);

            if (light->isReady())
                light->setTemperature(7000);
        }

        std::getline(std::cin, line);
    }

    return 0;
}