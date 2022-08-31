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

#include "ElgatoLight.h"
#include "HTTPRequest.hpp"
#include "Log.h"
#include "../Config.h"

#include <arpa/inet.h>
#include <iostream>
#include <nlohmann/json.hpp>
#include <fmt/core.h>
#include <future>

using json = nlohmann::json;

ElgatoLight::ElgatoLight(std::string name, char* address, uint16_t port) : _name(std::move(name)), _port(port) {
    inet_pton(AF_INET, address, &_address.s_addr);

    queryAccessory();
    if (_accessoryInfo != nullptr) queryState();
}

std::string ElgatoLight::portString() const {
    char address[20];

    inet_ntop(AF_INET, &_address.s_addr, address, sizeof(address));
    return std::string(address) + ":" + std::to_string(port());
}

void ElgatoLight::queryAccessory() {
    try {
        auto requestString = "http://" + portString() + "/elgato/accessory-info";

        http::Request request{requestString};
        const auto response = request.send("GET");
        const auto resString = std::string{response.body.begin(), response.body.end()};

        _accessoryInfo = std::make_shared<ElgatoAccessoryInfo>( json::parse(resString).get<ElgatoAccessoryInfo>() );
    } catch (const std::exception& e) {
        std::clog << kLogWarning << "Request failed, error: " << e.what() << std::endl;
    }
}

void ElgatoLight::queryState() {
    try {
        auto requestString = "http://" + portString() + "/elgato/lights";

        http::Request request{requestString};
        const auto response = request.send("GET");
        const auto resString = std::string{response.body.begin(), response.body.end()};

        _stateInfo = std::make_shared<ElgatoStateInfo>( json::parse(resString).get<ElgatoStateInfo>() );
    } catch(const std::exception& e) {
        std::clog << kLogWarning << "Request failed, error: " << e.what() << std::endl;
    }
}

bool ElgatoLight::powerOn() {
    const std::string body = R"({"lights": [{"on": 1}]})";
    return sendRequest(body);
}

bool ElgatoLight::powerOff() {
    const std::string body = R"({"lights": [{"on": 0}]})";
    return sendRequest(body);
}

bool ElgatoLight::setBrightness(uint8_t level) {
    if (level > 100) level = 100;

    const std::string body = fmt::format(R"({{"lights": [{{"brightness": {}}}]}})", level);
    return sendRequest(body);
}

bool ElgatoLight::setTemperature(uint16_t temperature) {
    if (temperature <= 2900) temperature = 2900;
    if (temperature >= 7000) temperature = 7000;

    const std::string body = fmt::format(R"({{"lights": [{{"temperature": {}}}]}})", colorToElgato(temperature));
    return sendRequest(body);
}

bool ElgatoLight::sendRequest(const std::string& requestBody) {
    try {
        auto requestString = "http://" + portString() + "/elgato/lights";
        http::Request request{requestString};
        const auto response = request.send("PUT", requestBody, {{"Content-Type", "application/json"}});
#if DEBUG_BUILD
        std::clog << kLogDebug << "(ElgatoLight) " << requestBody << " to " << requestString << " -> " << std::to_string(response.status.code) << std::endl;
#endif

        if (response.status.code != 200)
            return false;

        // FIXME: This should work different (modifying the state of the Light, and we can also fire a notification here for our clients)
        const auto resString = std::string{response.body.begin(), response.body.end()};
        _stateInfo = std::make_shared<ElgatoStateInfo>(json::parse(resString).get<ElgatoStateInfo>() );

#if DEBUG_BUILD
        std::clog << kLogDebug << "(ElgatoLight) response: " << resString << std::endl;
#endif
        notifyObservers(ElgatoStateChangedEventArgs{name()});

        return true;
    } catch (const std::exception& e) {
        std::clog << kLogWarning << "Request " << requestBody << " failed, error: " << e.what() << std::endl;
        return false;
    }
}

uint16_t ElgatoLight::colorFromElgato(int elgatoValue) {
    auto converted = 1000000 * pow(elgatoValue, -1);
    return (uint16_t)std::round(converted);
}

uint16_t ElgatoLight::colorToElgato(int colorValue) {
    auto converted = (uint16_t) std::round( 987007 * pow(colorValue, -0.999));

    if (converted < 143) converted = 143;
    if (converted > 344) converted = 344;

    return converted;
}

void ElgatoLight::registerCallback(std::function<void(const ElgatoStateChangedEventArgs&)>& observer) {
    _callbacks.push_back(observer);
}

void ElgatoLight::notifyObservers(const ElgatoStateChangedEventArgs& args) {
    if (_callbacks.empty()) return;

    std::thread caller([args, this] {
        for(const auto& callback : _callbacks){
            callback(args);
        }
    });
    caller.detach();
}

void from_json(const json& js, ElgatoAccessoryInfo& info) {
    js.at("productName").get_to(info.productName);
    js.at("hardwareBoardType").get_to(info.hardwareBoardType);
    js.at("firmwareBuildNumber").get_to(info.firmwareBuildNumber);
    js.at("firmwareVersion").get_to(info.firmwareVersion);
    js.at("serialNumber").get_to(info.serialNumber);
    js.at("displayName").get_to(info.displayName);
}

void from_json(const json& js, ElgatoStateInfo& info) {
    js.at("lights").at(0).at("on").get_to(info.on);
    js.at("lights").at(0).at("brightness").get_to(info.brightness);
    js.at("lights").at(0).at("temperature").get_to(info.temperature);
}
