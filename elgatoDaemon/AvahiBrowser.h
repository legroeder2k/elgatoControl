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


#pragma once

#include <utility>
#include <vector>
#include <string>
#include <memory>
#include <thread>
#include <avahi-common/simple-watch.h>
#include <avahi-client/lookup.h>

#include "ElgatoLight.h"


class AvahiBrowser {
public:
    static AvahiBrowser& getInstance() {
        static AvahiBrowser instance;
        return instance;
    }

    AvahiBrowser(const AvahiBrowser&) = delete;
    AvahiBrowser& operator=(const AvahiBrowser&) = delete;

    std::vector<std::shared_ptr<ElgatoLight>> getLights() { return _lights; }

    void start();
    void restart();
private:
    AvahiBrowser() = default;

    static void resolveCallback(AvahiServiceResolver*, [[maybe_unused]] AvahiIfIndex, [[maybe_unused]] AvahiProtocol,
                         AvahiResolverEvent, const char*, const char*, const char*, const char*, const AvahiAddress*,
                         uint16_t, AvahiStringList*, AvahiLookupResultFlags, [[maybe_unused]] void*);
    static void browseCallback(AvahiServiceBrowser*, AvahiIfIndex, AvahiProtocol, AvahiBrowserEvent, const char* , const char*,
                        const char*, [[maybe_unused]] AvahiLookupResultFlags, void*);
    static void clientCallback(AvahiClient*, AvahiClientState, [[maybe_unused]] void*);
    static void threadStart();
    void cleanUp();

    void addIfUnknown(std::shared_ptr<ElgatoLight>& light);
    void removeByName(std::string name);

    std::vector<std::shared_ptr<ElgatoLight>> _lights;

    std::thread* _workerThread = nullptr;
    AvahiSimplePoll* _simple_poll = nullptr;
    AvahiClient* _client = NULL;
    AvahiServiceBrowser* _browser = NULL;

public:
};
