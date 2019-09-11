# BlinKit: Crawler & WebUI Framework Based on Blink

## How to Build

### Required

1. python2
2. pyyaml

### Windows

1. Run `helpers\make_gen.py`.
2. Run `helpers\config_duk.bat`.
3. Open `projects\win\BlinKit.sln` or Run `projects\win\build_release.bat`. Set environment variable `PreferredToolArchitecture`  to `x64` if you want to build `Release` configuration.

### Linux

1. Install packages: `clang`, `libc++-dev`, `libc++abi-dev`, `libcurl4-openssl-dev`.
