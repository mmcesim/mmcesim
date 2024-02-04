#!/bin/sh

## This script allows you to run pre-built mmCEsim CLI
## without being asked about Apple developer information.

# allow as exectuable
chmod a+x ./bin/astyle
chmod a+x ./bin/mmcesim
chmod a+x ./bin/mmcesim-log
chmod a+x ./bin/mmcesim-maintain

# disable macOS developer sign check
xattr -r -d com.apple.quarantine ./bin/astyle
xattr -r -d com.apple.quarantine ./bin/mmcesim
xattr -r -d com.apple.quarantine ./bin/mmcesim-log
xattr -r -d com.apple.quarantine ./bin/mmcesim-maintain
