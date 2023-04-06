#! Function: max_n
#! Description: Select maximum n elements from a vector.
#! Author: Wuqiong Zhao
#! Date: 2023-04-05
#! Version: 0.2.1

# Max n
#
# Input:
#   - v: Input vector
#   - n: Number of elements to be selected
# Output:
#   - indices: The n indices
indices::u1 = FUNCTION max_n v::f1 n::u0
  IF n == 1
    \set_size(indices, 1)
    indices_{0} = \index_max(v)
  ELSE
    indices = \range(0, n-1)
    ranks = INIT n dtype=u
    n_sort_index::u1 = NEW \sort_index(v_{0:n-1})
    COMMENT 0 indicates the smallest one while (n-1) indicates the largest
    ranks_{n_sort_index} = \range(0, n-1)
    COMMENT the index of 0 element in ranks
    j_0::u0 = NEW n_sort_index_{0}
    value_0::d = NEW v_{j_0}
    i::u0 = LOOP n \length(v)
      value::d = NEW v_{i}
      IF value > value_0
        COMMENT the index of the smallest element larger then value
        limit::u = NEW n
        new_j_0 = NEW j_0
        j::u0 = LOOP 0 n
          IF j != j_0
            index::u0 = NEW indices_{j}
            rank::u0r = NEW ranks_{j}
            this_value::d = NEW v_{index}
            IF this_value < value
              rank = rank - 1
              IF rank == 0
                new_j_0 = j
                value_0 = v_{index}
              END
            ELIF rank < limit
              limit = rank
            END
          END
        END
        ranks_{j_0} = limit - 1
        indices_{j_0} = i
        j_0 = new_j_0
      END
    END
  END
END
