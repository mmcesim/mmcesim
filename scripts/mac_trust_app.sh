#!/bin/sh

## This script allows you to run pre-built mmCEsim CLI
## without being asked about Apple developer information.

# allow as exectuable
chmod a+x ./bin/astyle
chmod a+x ./bin/mmcesim

# disable macOS developer sign check
xattr -r -d com.apple.quarantine ./bin/astyle
xattr -r -d com.apple.quarantine ./bin/mmcesim
