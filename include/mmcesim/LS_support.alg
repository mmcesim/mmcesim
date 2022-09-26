#! Function: LS_support
#! Description: Least square for a selected support.
#! Author: Wuqiong Zhao
#! Date: 2022-09-26
#! Version: 0.1.0

# Input:
#   - Q: The matrix
#   - y: Received signal
# Output:
#   - h: The estimated signal
h::v = FUNCTION LS_support Q::m y::v s::u1
  h = \zeros(\size(Q, 1)) # initialize as zeros
  h_{s} = \solve(Q_{:,s}, y)
END
