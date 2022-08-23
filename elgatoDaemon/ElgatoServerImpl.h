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

#include "elgato.grpc.pb.h"
#include "elgato.pb.h"

class ElgatoServerImpl final : public Elgato::Service {
public:
    void RunServer(const std::string&);

    ::grpc::Status ListFixtures(::grpc::ServerContext*, const Empty*, FixtureList*) override;
    ::grpc::Status Refresh(::grpc::ServerContext*, const Empty*, SimpleCliResponse*) override;

    ::grpc::Status PowerOn(::grpc::ServerContext*, const SimpleCliRequest*, SimpleCliResponse*) override;
    ::grpc::Status PowerOff(::grpc::ServerContext*, const SimpleCliRequest*, SimpleCliResponse*) override;
    ::grpc::Status SetBrightness(::grpc::ServerContext*, const Int32CliRequest*, SimpleCliResponse*) override;
    ::grpc::Status SetTemperature(::grpc::ServerContext*, const Int32CliRequest*, SimpleCliResponse*) override;
private:
    static std::string expand_with_environment( const std::string &s );
};