#! FILE: demo.alg
#! DESCRIPTION: This is a simple .alg file example.
#!              Some basic functions will be shown here.
#! AUTHOR: Wuqiong Zhao (wqzhao@seu.edu.cn)
#! DATE: 2022-07-22
#! LICENSE: MIT License

INCLUDE xxx # include a file
a = INIT 5 4 type=c128 fill=zeros # initialize a complex matrix of size 5x4
b = CALC $a * 2$
b = a * 2 # this is equivalent to the above (simplifies the syntax)
c = norm(b)
d = ELEM a 2 1
d = a@2@1 # this is equivalent to the above (note that @ indicates it is index)
CONFIG initial_index 0 # this is the default behavior
i = LOOP from=0 to=3 step=1
  PRINT i
  IF i==1
    # do something
    e = TRAN a # transpose
    e = t(a)
  ELSE
    # do something else
    e = HERM a
  END
END

p1 p2 = CALL foo param1 param2 key=val

r1:f64 r2:f64 = FUNC p1 p2
  # This is a function body
  # with parameter p1 and p2
  # and return r1 and r2.
  r1 = exp(p1+p2)
  r2 = abs(r1)
  PRINT $r1 + r2$
END
