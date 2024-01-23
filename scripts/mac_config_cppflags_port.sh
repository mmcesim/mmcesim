#!/bin/sh

# You can also use this script if you install Armadillo library with MacPorts
# available at: https://ports.macports.org/port/armadillo/

bin/mmcesim config cpp --value="clang++"
bin/mmcesim config cppflags --value="-I/opt/local/include -L/opt/local/lib"
