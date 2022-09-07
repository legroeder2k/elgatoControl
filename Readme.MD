# ElgatoControl

Control your Elgato fixtures from Linux.

This is a small and simple project to control your Elgato Fixtures from Linux and one of my first C++ projects.

# Build Instructions

## Required libraries (on Ubuntu 22.04)

- libfmt (8.1.1+) and libfmt-dev
- libavahi-client-dev (0.8.5+)
- nlohmann-json3-dev (3.10.5+)
- gRPC built from https://grpc.io/docs/languages/cpp/quickstart/ <br /> it will not work with OS packages :(

## Building

To build the daemon and the CLI utility create a new build directory inside the source tree:

    mkdir build && cd build

Configure the build with cmake

    cmake -DCMAKE_BUILD_TYPE=Release -GNinja ..

The default Install prefix CMAKE_INSTALL_PREFIX will be ~/.local<br/>
And start the compilation:

    ninja

After compilation install with:

    ninja install

And enable and start the daemon:

    systemctl --user enable elgatoDaemon
    systemctl --user start elgatoDaemon

And enjoy :)

Keep the install_manifest.txt

## Usage

### GUI

Either start elgato-ui which will present you with a StatusIcon where you can control your KeyLights either from the
context menu or by left-clicking and using the settings window.

### CLI

Using elgato-cli you can control the brightness, color temperature and power directly from the command line or from 
scripts / other programs. See elgato-cli --help for further informations.

```
elgato-cli --help

Usage: ./elgato-cli <OPTIONS>
Send a command to the elgatoDaemon.
Example: ./elgato-cli --name="*" --powerOn

Generic functions:
 -l, --list		Lists all discoverd lights with some basic informations
 -r, --refresh		Asks the daemon to refresh the list of lights discoverd
 -h, --help		Prints out this help

Fixture functions: (These all need a specified fixture using --name)
  --name=NAME		Specifies the name of the fixture (not display name) the following command acts on.
			Can be a regex to match multiple fixtures and a single asterisk will be replaced to match all.
  -o, --powerOn		Turns the selected fixture(s) on at the last power setting
  -O, --powerOff	Turns the selected fixture(s) off
  --brightness=VALUE	Set the brightness to a value between 0 - 100
  --temperature=VALUE	Set the color temperature to a value between 2900K and 7000K
```

## Uninstall

Disable the daemon

`systemctl --user stop elgatoDaemon`<br />
`systemctl --user disable elgatoDaemon`

Then you can remove all installed files:

`xargs rm < install_manifest.txt`

# Attributions

<a href="https://www.flaticon.com/free-icons/idea" title="idea icons">Idea icons created by Freepik - Flaticon</a>