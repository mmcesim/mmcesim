#/bin/sh

# Note:
# You may need to remove CMakeFiles and CMakeCache.txt to avoid problems.

rm -r build
mkdir build && cd build
cmake -D MMCESIM_CODE_COVERAGE=ON -D CMAKE_BUILD_TYPE=Debug ..
make
ctest -j $(sysctl -n hw.logicalcpu) --output-on-failure
