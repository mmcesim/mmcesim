#! Function: OMPL
#! Description: Orthogonal matching pursuit list compressed sensing.
#! Author: Wuqiong Zhao
#! Date: 2023-04-05
#! Version: 0.2.1

# OMP List
#
# Input:
#   - Q: Sensing matrix
#   - y: Received signal
#   - L: Sparsity
# Output:
#   - h: The estimated sparse signal
h::v = FUNCTION OMPL Q::m y::v L::u0 n::u0
  COMMENT Start of OMP algorithm!
  G::u0c = NEW \size(Q, 1) # GNt * GNr
  h = \zeros(G) # initialize as zeros
  Q_H::m = NEW Q^H # the conjugate transpose of Q
  r = NEW y # residual
  rs::m = NEW \repmat(r, 1, n) # residual for n lists
  supports = INIT L n dtype=u  fill=ones scale=G # over-length support matrix
  inits = INIT G n
  candidate_inits = INIT G n*n
  candidate_init = INIT G
  term = INIT G dtype=f # float number array
  i::u0 = LOOP 0 L
    COMMENT OMPL "Branch"
    candidate_supports = INIT L n*n dtype=u fill=ones scale=G
    candidate_rs::m = INIT \length{y} n*n
    j::u0 = LOOP 0 n
      IF i == 0 || supports_{i-1,j} != G
        support::u1 = NEW supports_{:,j}
        r::v = NEW rs_{:,j}
        term = \abs(Q_H @ r)
        indices::u1 = CALL max_n term n init=true
        k::u0 = LOOP 0 n
          support_{i} = indices_{k}
          columns::m = NEW Q_{:,support_{0:i}}
          # a::v = NEW \pinv(columns) @ y
          a::v = NEW \solve(columns, y)
          candidate_rs_{:,k+j*n} = y - columns @ a
          candidate_supports_{:,k+j*n} = support
          IF i + 1 == L
            candidate_init_{support_{0:i}} = a
            candidate_inits_{:,k+j*n}=candidate_init
          END
        END
      END
    END
    COMMENT OMPL "MERGE"
    ranking::u1 = NEW \sort_index(\sum(\pow(\abs(candidate_rs),2), 0))
    selected_cnt::u0 = NEW 0
    j::u0 = LOOP 0 n*n
      index::u0 = NEW ranking_{j}
      IF selected_cnt == n
        BREAK
      ELIF !(candidate_supports_{0,index} == G || (j && !\accu(candidate_supports_{:,index} != candidate_supports_{:,ranking_{j-1}})))
        supports_{:,selected_cnt} = candidate_supports_{:,index}
        rs_{:,selected_cnt} = candidate_rs_{:,index}
        IF i + 1 == L
          inits_{:,selected_cnt} = candidate_inits_{:,index}
        END
        selected_cnt = selected_cnt + 1
      END
    END
  END
  # prepare for the final return
  h = inits_{:,0}
END
