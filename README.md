# SteamDeckGyroDSU
**DSU** (*cemuhook protocol*) server for motion data for **Steam Deck** and **Lenovo Legion Go S** (both running SteamOS).

The server auto-detects the device at startup: Steam Deck (`28de:1205`) or Legion Go S (`28de:12ff`).

## Install/Update

Open this page in the browser in the device's desktop mode.

Download [Installation File](https://github.com/tangzero/SteamDeckGyroDSU/releases/latest/download/update-sdgyrodsu.desktop), save it to Desktop and run it by touching or double-clicking *Update GyroDSU*.

This Desktop shortcut may be used also to update the *SteamDeckGyroDSU* to the most recent version.

To uninstall, run *Uninstall GyroDSU* from Desktop by touching or double-clicking.
    
## Usage

Server is running as a service. It provides motion data for cemuhook at the device's IP address and UDP port *26760*.

Optionally, another UDP server port may be specified in an environment variable **SDGYRO_SERVER_PORT**.

**Remark:** The server provides only motion data. Remaining controls (buttons/axes) are not provided.

**Remark (Legion Go S):** the gyro is exposed by Steam as a virtual HID device (`28de:12ff`), so Steam must be running for the server to detect it. In gaming mode Steam is always up.

### Client (emulator) Configuration

See [Client Configuration](https://github.com/kmicki/SteamDeckGyroDSU/wiki/Client-Configuration) wiki page for instructions on how to configure client applications (emulators).

## Reporting problems

Before reporting problems make sure you are running the most recent version of **SteamDeckGyroDSU** (see *Install/Update* section above).

When reporting a problem or an issue with the server, please generate a log file with following command:

    $HOME/sdgyrodsu/logcurrentrun.sh > sdgyrodsu.log
    
File `sdgyrodsu.log` will be generated in current directory. Attach it to the issue describing the problem.

## Alternative installation

To install the server using a binary package provided in a release, see [wiki page](https://github.com/kmicki/SteamDeckGyroDSU/wiki/Alternative-installation-instructions).

To build the server from source on Deck and install it, see [wiki page](https://github.com/kmicki/SteamDeckGyroDSU/wiki/Build-and-install-from-source).

## Building in a container

From a machine with Docker (the repo cannot be built on macOS directly; hidraw/systemd libs are Linux-only):

    scripts/build-container.sh

The binary is produced at `bin/release/sdgyrodsu`. The single binary auto-detects
Steam Deck (`28de:1205`) and Legion Go S (`28de:12ff`) at startup; run the usual
install/update scripts on the device to deploy.
