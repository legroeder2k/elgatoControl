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

#include <grpc/grpc.h>
#include <grpcpp/security/server_credentials.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>
#include <thread>

#include "ElgatoServerImpl.h"
#include "Log.h"
#include "AvahiBrowser.h"


using ::grpc::Status;
using ::grpc::Server;
using ::grpc::ServerBuilder;
using ::grpc::ServerContext;
using ::Fixture;
using ::FixtureList;
using ::SimpleCliRequest;
using ::SimpleCliResponse;

void ElgatoServerImpl::RunServer(const std::string& socketPath) {
    auto server_address = "unix://" + expand_with_environment(socketPath);

    std::thread serverThread([this, server_address]{
        ServerBuilder builder;
        builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
        builder.RegisterService(this);
        std::unique_ptr<Server> server(builder.BuildAndStart());
        std::clog << kLogInfo << "RPC Server listening on " << server_address << std::endl;
        server->Wait();
    });

    serverThread.detach();
}

std::string ElgatoServerImpl::expand_with_environment(const std::string &s) {
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

Status ElgatoServerImpl::ListFixtures([[maybe_unused]] ServerContext* _, [[maybe_unused]] const Empty* empty, [[maybe_unused]] FixtureList* fixtureList) {
    for(auto& light : AvahiBrowser::getInstance().getLights()) {
        auto newFix = fixtureList->add_fixtures();
        newFix->set_name(light->name());

        if (light->isReady())
        {
            newFix->set_isready(true);
            newFix->set_displayname(light->deviceInfo()->displayName);
            newFix->set_productname(light->deviceInfo()->productName);
            newFix->set_serialnumber(light->deviceInfo()->serialNumber);
            newFix->set_powerstate(light->deviceState()->on == 1);
            newFix->set_brightness(light->deviceState()->brightness);
            newFix->set_temperature(ElgatoLight::colorFromElgato(light->deviceState()->temperature));
        }
    }

    return Status::OK;
}

Status ElgatoServerImpl::Refresh([[maybe_unused]] ServerContext* _, [[maybe_unused]] const Empty* empty, SimpleCliResponse* response) {
    AvahiBrowser::getInstance().restart();
    response->set_successful(true);
    return Status::OK;
}

Status ElgatoServerImpl::PowerOn([[maybe_unused]] ServerContext* _, [[maybe_unused]] const SimpleCliRequest* request, SimpleCliResponse* response ) {
    for(auto& light : AvahiBrowser::getInstance().allByName(request->fixturefilter())) {
        if (light->isReady())
            light->powerOn();
    }

    response->set_successful(true);
    return Status::OK;
}

Status ElgatoServerImpl::PowerOff([[maybe_unused]] ServerContext* _, [[maybe_unused]] const SimpleCliRequest* request, SimpleCliResponse* response ) {
    for(auto& light : AvahiBrowser::getInstance().allByName(request->fixturefilter())) {
        if (light->isReady())
            light->powerOff();
    }

    response->set_successful(true);
    return Status::OK;
}

Status ElgatoServerImpl::SetBrightness([[maybe_unused]] ServerContext* _, const Int32CliRequest* request, SimpleCliResponse* response) {
    for(auto& light : AvahiBrowser::getInstance().allByName(request->fixturefilter())) {
        if (light->isReady())
            light->setBrightness(request->newvalue());
    }

    response->set_successful(true);
    return Status::OK;
}

Status ElgatoServerImpl::SetTemperature([[maybe_unused]] ServerContext* _, const Int32CliRequest* request, SimpleCliResponse* response) {
    for(auto& light : AvahiBrowser::getInstance().allByName(request->fixturefilter())) {
        if (light->isReady())
            light->setTemperature(request->newvalue());
    }

    response->set_successful(true);
    return Status::OK;
}