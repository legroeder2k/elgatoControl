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

#include <string>
#include <netinet/in.h>
#include <nlohmann/json.hpp>
#include <iostream>

struct ElgatoAccessoryInfo final {
public:
    ElgatoAccessoryInfo() = default;

    std::string productName = {};
    uint16_t hardwareBoardType = 0;
    uint16_t firmwareBuildNumber = 0;
    std::string firmwareVersion = {};
    std::string serialNumber = {};
    std::string displayName = {};
};

struct ElgatoStateInfo final {
public:
    ElgatoStateInfo() = default;

    uint8_t on = 0;
    uint8_t brightness = 0;
    uint8_t temperature = 0;
};

class ElgatoLight final {
public:
    ElgatoLight(std::string name, char* address, uint16_t port);

    // Overload == to make equality check based on name
    bool operator==(const ElgatoLight &other) const;

    [[nodiscard]] std::string name() const { return _name; }
    [[nodiscard]] in_addr address() const { return _address; }
    [[nodiscard]] uint16_t port() const { return _port; }

    [[nodiscard]] bool isReady() const {
        return _accessoryInfo != nullptr && _stateInfo != nullptr;
    }

    [[nodiscard]] std::shared_ptr<ElgatoAccessoryInfo> deviceInfo() const {
        return _accessoryInfo;
    }

    [[nodiscard]] std::shared_ptr<ElgatoStateInfo> deviceState() const {
        return _stateInfo;
    }

    [[nodiscard]] std::string portString() const;

    bool powerOn();
    bool powerOff();

    bool setBrightness(uint8_t level);
    bool setTemperature(uint16_t temperature);

    static uint16_t colorToElgato(int colorValue);
    static uint16_t colorFromElgato(int elgatoValue);

private:
    bool sendRequest(const std::string& requestBody);

    void queryAccessory();
    void queryState();

    std::string _name = {};
    in_addr _address = {};
    uint16_t _port = 0;

    std::shared_ptr<ElgatoAccessoryInfo> _accessoryInfo = nullptr;
    std::shared_ptr<ElgatoStateInfo> _stateInfo = nullptr;
};

void from_json(const nlohmann::json&, ElgatoAccessoryInfo&);
void from_json(const nlohmann::json&, ElgatoStateInfo&);
