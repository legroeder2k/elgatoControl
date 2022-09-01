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


#pragma once

#include <memory>
#include <utility>
#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>

#include "elgato.grpc.pb.h"

using grpc::Channel;

struct RemoteFixture {
    RemoteFixture(std::string name, bool isReady, std::string displayName, std::string productName,
                  std::string serialNumber, bool powerState, int32_t brightness, int32_t temperature) :
        _name(std::move(name)),
        _isReady(isReady),
        _displayName(std::move(displayName)),
        _productName(std::move(productName)),
        _serialNumber(std::move(serialNumber)),
        _powerState(powerState),
        _brightness(brightness),
        _temperature(temperature) { }

    std::string _name;
    bool _isReady;
    std::string _displayName;
    std::string _productName;
    std::string _serialNumber;
    bool _powerState;
    int32_t _brightness;
    int32_t _temperature;
};

class ElgatoClient final {
public:
    explicit ElgatoClient(const std::shared_ptr<Channel> &channel) : _stub(Elgato::NewStub(channel)) { }
    static std::shared_ptr<Channel> createChannel(const std::string&);

    std::vector<std::shared_ptr<RemoteFixture>> listFixtures();

    bool powerOn(std::string);
    bool powerOff(std::string);
    bool setBrightness(std::string, uint32_t);
    bool setColorTemp(std::string, uint32_t);
private:
    static std::string expand_with_environment(const std::string& );
    std::unique_ptr<Elgato::Stub> _stub;
};
