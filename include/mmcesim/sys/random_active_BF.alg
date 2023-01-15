#! Description: Random Active Beamforming
#! Author: Wuqiong Zhao
#! Date: 2023-01-15
#! Version: 0.1.1

COMMENT Random Active Beamforming
COMMENT Generated from 'sys/random_active_BF.alg'

# `VAR` is an internal temprary macro
# SIZE, BEAM, TIMES are local variables
`VAR` = \randn(SIZE, BEAM, TIMES)

# Normalize the beamforming matrix.
CPP `VAR`.each_slice([](cx_mat& X){return normalise(X,2,0);});
MATLAB % normalize the beamforming matrix
PYTHON # normalize the beamforming matrix
