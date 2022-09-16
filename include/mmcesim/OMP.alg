#! Function: OMP
#! Description: Orthogonal matching pursuit compressed sensing.
#! Author: Wuqiong Zhao
#! Date: 2022-09-16
#! Version: 0.1.0

# Input:
#   - Q: Sensing matrix
#   - y: Received signal
#   - L: Sparsity
# Output:
#   - h: The estimated sparse signal
h::v = FUNCTION OMP Q::m y::v L::u
  COMMENT Start of OMP algorithm!
  h = \zeros(\size(Q, 1)) # initialize as zeros
  Q_H = NEW Q^H # the conjugate transpose of Q
  r = NEW y # residual
  r_last = NEW r * 2 # the residual in last iteration
  support = INIT \length(y) dtype=u # over-length support array
  term = INIT $\size(Q_H, 0)$ dtype=f # float number array
  j::u0 = LOOP 0 \length(y)
    term = \abs(Q_H @ r)
    index::u0 = NEW \index_max(term)
    IF \ismember(index, support)
      BREAK # end of the LOOP
    END
    support_{j} = index
    columns::m = NEW Q_{:, support_{0:j}}
    a::v = NEW \pinv(columns) @ y
    r = y - columns @ a
    IF \sum(\abs(r - r_last)) / \sum(\abs(r_last)) < 0.0001 || j >= L
      j = j + 1
      BREAK # accurate enough to end iteration
    ELSE
      r_last = r
    END
  END
  # prepare for the final return
  h_{support_{0:j-1}} = a
END
