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

#include "ElgatoClient.h"

#include <grpcpp/create_channel.h>
#include <fmt/core.h>

using ::grpc::ClientContext;
using ::grpc::CreateChannel;
using ::grpc::Status;

std::shared_ptr<Channel> ElgatoClient::createChannel(const std::string& address) {
    auto realPath = expand_with_environment(address);
    auto channelAddress = "unix://" + realPath;

    return grpc::CreateChannel(channelAddress, grpc::InsecureChannelCredentials());
}

std::string ElgatoClient::expand_with_environment(const std::string &s) {
    if ( s.find("$[") == std::string::npos) return s;

    std::string pre     = s.substr( 0, s.find("$["));
    std::string post    = s.substr( s.find("$[")+ 2);

    if (post.find(']') == std::string::npos) return s;
    std::string variable = post.substr( 0, post.find( ']' ) );
    std::string value   = "";

    post = post.substr(post.find(']') + 1);
    const char *v = getenv( variable.c_str() );
    if ( v != NULL ) value = std::string( v );

    return expand_with_environment(pre + value + post);
}

void ElgatoClient::listFixtures() {
    Empty empty;
    FixtureList fixtureList;
    ClientContext context;

    _stub->ListFixtures(&context, empty, &fixtureList);

    for(auto& fixture : fixtureList.fixtures()) {
        fmt::print("  {} ({}) is {} with s/n {} (Power {} @ {}%, {}K)\n", fixture.name(), fixture.displayname(), fixture.productname(), fixture.serialnumber(), fixture.powerstate() ? "on" : "off", fixture.brightness(), fixture.temperature());
    }
}

void ElgatoClient::refreshBrowser() {
    ClientContext context;
    Empty empty;
    SimpleCliResponse response;

    fmt::print("Refreshing fixture list...");

    auto status = _stub->Refresh(&context, empty, &response);

    if (status.ok() && response.successful())
        fmt::print(" OK\n");
    else
        fmt::print(" Error!\n");
}

void ElgatoClient::powerOn(const std::string& fixtureFilter) {
    ClientContext context;
    SimpleCliRequest request;
    SimpleCliResponse response;

    request.set_fixturefilter(fixtureFilter);

    fmt::print("Sending Power on request: ");

    auto status = _stub->PowerOn(&context, request, &response);

    if (status.ok() && response.successful())
        fmt::print(" OK\n");
    else
        fmt::print(" Error!\n");
}

void ElgatoClient::powerOff(const std::string& fixtureFilter) {
    ClientContext context;
    SimpleCliRequest request;
    SimpleCliResponse response;

    request.set_fixturefilter(fixtureFilter);

    fmt::print("Sending Power off request: ");

    auto status = _stub->PowerOff(&context, request, &response);

    if (status.ok() && response.successful())
        fmt::print(" OK\n");
    else
        fmt::print(" Error!\n");
}

void ElgatoClient::setBrightness(const std::string& fixtureFilter, long newValue) {
    ClientContext context;
    Int32CliRequest request;
    SimpleCliResponse response;

    request.set_fixturefilter(fixtureFilter);
    request.set_newvalue(newValue);

    fmt::print("Setting fixtures to {} brightness: ", newValue);

    auto status = _stub->SetBrightness(&context, request, &response);

    if (status.ok() && response.successful())
        fmt::print(" OK\n");
    else
        fmt::print(" Error!\n");
}

void ElgatoClient::setTemperature(const std::string& fixtureFilter, long newValue) {
    ClientContext context;
    Int32CliRequest request;
    SimpleCliResponse response;

    request.set_fixturefilter(fixtureFilter);
    request.set_newvalue(newValue);

    fmt::print("Setting fixtures to a color temp of {}K: ", newValue);

    auto status = _stub->SetTemperature(&context, request, &response);

    if (status.ok() && response.successful())
        fmt::print(" OK\n");
    else
        fmt::print(" Error!\n");
}