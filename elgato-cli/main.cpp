/*
 * Copyright (c) 2022, Sascha Huck <sascha@wirrewelt.de>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <getopt.h>
#include <fmt/core.h>

#include "../Config.h"
#include "ElgatoClient.h"

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
    static const struct option long_options[] = {
            { "list",       optional_argument,nullptr,'l' },
            { "refresh",    optional_argument,nullptr,'r' },
            { "name",       optional_argument,nullptr,'n' },
            { "powerOn",    optional_argument,nullptr,'o' },
            { "powerOff",   optional_argument,nullptr,'O' },
            { "brightness", optional_argument,nullptr,'b' },
            { "temperature",optional_argument,nullptr,'t' },
            { "help",       optional_argument,nullptr,'h' },
            {"listen",      optional_argument,nullptr,'L' },
    };

    bool listMode = false;
    bool refresh = false;
    std::string nameOfLight = "";
    bool powerOn = false;
    bool powerOff = false;
    bool setBrightness = false;
    long brightness = 0;
    bool setTemperature = false;
    long temperature = 0;
    bool showLongHelp = false;
    bool showShortHelp = false;
    bool listen = false;

    while(1) {
        int index = -1;
        auto result = getopt_long(argc, argv, "LlnoOh", long_options, &index);
        if (result == -1) break;

        switch(result) {
            case 'L':
                listen = true;
                break;
            case 'l':
                listMode = true;
                break;
            case 'r':
                refresh = true;
                break;
            case 'n':
                if (optarg)
                    nameOfLight = std::string(optarg);
                break;
            case 'o':
                powerOn = true;
                powerOff = false;
                break;
            case 'O':
                powerOn = false;
                powerOff = true;
                break;
            case 'h':
                showLongHelp = true;
                break;
            case 'b':
                if (optarg) {
                    setBrightness = true;
                    brightness = strtol(optarg, nullptr, 10);

                    if(brightness == 0)
                        setBrightness = false;
                }
                break;
            case 't':
                if (optarg) {
                    setTemperature = true;
                    temperature = strtol(optarg, nullptr, 10);

                    if(temperature == 0)
                        setTemperature = false;
                }
                break;
            default:
                break;
        }
    }

    if (!listMode && !refresh && !powerOn && !powerOff && !showLongHelp && !setBrightness && !setTemperature && !listen)
        showShortHelp = true;

    if (!listMode && !refresh && !listen && !showShortHelp && !showLongHelp && nameOfLight.empty())
        showShortHelp = true;

    if (!nameOfLight.empty() && !powerOn && !powerOff && !showLongHelp && !setBrightness && !setTemperature)
        showShortHelp = true;

    if (showShortHelp) {
        fmt::print("Usage: {} --name=\"<name>\" <OPTIONEN>\n", argv[0]);
        fmt::print("Try '{} --help' for more informations.\n", argv[0]);

        return 0;
    }

    if (showLongHelp) {
        fmt::print("Usage: {} <OPTIONEN>\n", argv[0]);
        fmt::print("Send a command to the elgatoDaemon.\n");
        fmt::print("Example: {} --name=\"*\" --powerOn\n\n", argv[0]);

        fmt::print("Generic functions:\n");
        fmt::print(" -l, --list\t\tLists all discoverd lights with some basic informations\n");
        fmt::print(" -r, --refresh\t\tAsks the daemon to refresh the list of lights discoverd\n");
        fmt::print(" -h, --help\t\tPrints out this help\n\n");

        fmt::print("Fixture functions: (These all need a specified fixture using --name)\n");
        fmt::print("  --name=NAME\t\tSpecifies the name of the fixture (not display name) the following command acts on.\n\t\t\tCan be a regex to match multiple fixtures and a single asterisk will be replaced to match all.\n");
        fmt::print("  -o, --powerOn\t\tTurns the selected fixture(s) on at the last power setting\n");
        fmt::print("  -O, --powerOff\tTurns the selected fixture(s) off\n");
        fmt::print("  --brightness=VALUE\tSet the brightness to a value between 0 - 100\n");
        fmt::print("  --temperature=VALUE\tSet the color temperature to a value between 2900K and 7000K\n");

        return 0;
    }

    // At this point we should know what to do :)
    auto channel = ElgatoClient::createChannel(SOCKET_FILE);
    ElgatoClient client(channel);

    if (listen) {
        client.listenForChanges();

        std::string line;
        std::cout << "Console interface:\n  q -> quit" << std::endl;
        std::getline(std::cin, line);

        while (line != "q") {
            std::getline(std::cin, line);
        }

        return 0;
    }

    if (listMode) {
        fmt::print("Discovered fixtures:\n");
        client.listFixtures();

        return 0;
    }

    if (refresh) {
        client.refreshBrowser();
        return 0;
    }

    if (powerOn) {
        client.powerOn(nameOfLight);
    }

    if (powerOff) {
        client.powerOff(nameOfLight);
    }

    if (setBrightness) {
        client.setBrightness(nameOfLight, brightness);
    }

    if (setTemperature) {
        client.setTemperature(nameOfLight, temperature);
    }

    return 0;
}