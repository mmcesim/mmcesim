#! Function: LS
#! Description: Least square.
#! Author: Wuqiong Zhao
#! Date: 2022-09-26
#! Version: 0.1.0

# Input:
#   - Q: The matrix
#   - y: Received signal
# Output:
#   - h: The estimated signal
h::v = FUNCTION LS Q::m y::v
  h = \solve(Q, y)
END
