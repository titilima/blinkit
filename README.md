# BlinKit: Crawler Framework Based on Blink

## How to Build

### Required

* python2

### Windows

1. Run `helpers\config_duk.bat`.
2. Open `projects\win\BlinKit.sln` or Run `projects\win\build_release.bat`. Set environment variable `PreferredToolArchitecture`  to `x64` if you want to build `Release` configuration.

### Linux

1. Install packages: `clang`, `libc++-dev`, `libc++abi-dev`, `libcurl4-openssl-dev`.
2. Run `helpers/config_duk.sh`.
3. Run `make all` in `projects/posix` to generate `libBlinKit.a`.

### macOS/iOS

1. Run `helpers/config_duk.sh`.
2. Open `projects/macos/BlinKit.xcworkspace` in Xcode.
