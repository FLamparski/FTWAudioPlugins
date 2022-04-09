# FTW Audio Plugins

This is a monorepo for my audio plugins. It is a monorepo because the whole way that iPlug2 and
VST3 development works is backwards and having one repo per plugin would be a pain in the ass
and quickly fill up my disk with identical copies of prebuilt libraries.

## Getting started

1. **Use `git clone --recursive`** - iPlug2 is included as a submodule
1. Download dependencies:
    1. [Prebuild Windows libraries][PREBUILT-WIN] - extract the `win` and `src` directories to
    `iPlug2/Dependencies/Build`
    1. [Prebuild Windows Faust library][PREBUILT-WIN-FAUST] - extract the `Faust` directory to
    `iPlug2/Dependencies/Build/win`

You should now be able to *compile* plugins like WeirdOrgan without errors. However, they may
still not run, or crash your host.

[PREBUILT-WIN]: https://github.com/iPlug2/iPlug2/releases/download/setup/IPLUG2_DEPS_WIN.zip
[PREBUILT-WIN-FAUST]: https://github.com/iPlug2/iPlug2/releases/download/setup/IPLUG2_DEPS_WIN_FAUST.zip