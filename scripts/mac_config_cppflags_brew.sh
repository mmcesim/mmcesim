#!/bin/sh

# You can also use this script if you install Armadillo library with Homebrew
# available at: https://formulae.brew.sh/formula/armadillo

bin/mmcesim config cpp --value="clang++"
bin/mmcesim config cppflags --value="-I/opt/homebrew/include -L/opt/homebrew/lib"
