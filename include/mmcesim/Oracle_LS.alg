#! Function: Oracle_LS
#! Description: Oracle LS compressed sensing.
#! Author: Wuqiong Zhao
#! Date: 2022-09-18
#! Version: 0.1.0

h::v = FUNCTION Oracle_LS Q::m y::v indices::u1
  h = \zeros(\size(Q,1))
  h_{indices} = \pinv(Q_{:, indices}) @ y
END
