# Build Instructions

## Required packages (on Ubuntu 22.04)

- libfmt (8.1.1+) and libfmt-dev
- libavahi-client-dev (0.8.5+)
- nlohmann-json3-dev (3.10.5+)
- gRPC built from https://grpc.io/docs/languages/cpp/quickstart/ <br /> it will not work with OS packages :(

## Building

To build the daemon and the CLI utility create a new build directory inside the source tree:

`mkdir build && cd build`

Configure the build with cmake

`cmake -DCMAKE_BUILD_TYPE=Release -GNinja ..`

And compile

`ninja`