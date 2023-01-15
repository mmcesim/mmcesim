#! Description: Random RIS Beamforming
#! Author: Wuqiong Zhao
#! Date: 2023-01-15
#! Version: 0.1.1

COMMENT Random RIS Beamforming
COMMENT Generated from 'sys/random_active_BF.alg'

`VAR` = \randn(SIZE, TIMES) # generate random
`VAR` = `VAR` / \abs(Psi)   # normalize
