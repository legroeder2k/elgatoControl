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

The default Install prefix CMAKE_INSTALL_PREFIX will be ~/.local<br/>
And start the compilation:

`ninja`

After compilation install with:

`ninja install`

And enable and start the daemon:

`systemctl --user enable elgatoDaemon`<br/>
`systemctl --user start elgatoDaemon`

And enjoy :)

Keep the install_manifest.txt

## Uninstall

Disable the daemon

`systemctl --user stop elgatoDaemon`<br />
`systemctl --user disable elgatoDaemon`

Then you can remove all installed files:

`xargs rm < install_manifest.txt`

# Attributions

<a href="https://www.flaticon.com/free-icons/idea" title="idea icons">Idea icons created by Freepik - Flaticon</a>