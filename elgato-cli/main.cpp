/*
 * Copyright (c) 2021, Sascha Huck <sascha@wirrewelt.de>
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
            { "help",       optional_argument,nullptr,'h' }
    };

    bool listMode = false;
    bool refresh = false;
    std::string nameOfLight = "";
    bool powerOn = false;
    bool powerOff = false;
    bool showLongHelp = false;
    bool showShortHelp = false;

    while(1) {
        int index = -1;
        auto result = getopt_long(argc, argv, "lnoOh", long_options, &index);
        if (result == -1) break;

        switch(result) {
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
            default:
                break;
        }
    }

    if (!listMode && !refresh && !powerOn && !powerOff && !showLongHelp)
        showShortHelp = true;

    if (!listMode && !refresh && !showShortHelp && !showLongHelp && nameOfLight.empty())
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

        return 0;
    }

    // At this point we should know what to do :)
    auto channel = ElgatoClient::createChannel(SOCKET_FILE);
    ElgatoClient client(channel);

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
        return 0;
    }

    if (powerOff) {
        client.powerOff(nameOfLight);
        return 0;
    }

    return 0;
}