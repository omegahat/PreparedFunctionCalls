#dyn.load("eval.so")
library(PreparedFunctionCalls)

x = 1:100
y = rep(1, length(x))
z = rep(TRUE, length(x))

f =
function(x, y, z)
{
#  print(c(x = x, y = y, z = z))
  x + y + z
}


f1 =
function(x, y, z)
{
  1.0
}


system.time(invisible(.Call("R_evalFunBody", x, y, z, f)))
   user  system elapsed 
 15.458   0.234  16.301


system.time(invisible(.Call("R_inlineFunCall", x, y, z, f)))
   user  system elapsed 
 28.091   0.419  31.194


With f1
system.time(invisible(.Call("R_evalFunBody", x, y, z, f1)))
   user  system elapsed 
  2.362   0.101   2.554 
system.time(invisible(.Call("R_inlineFunCall", x, y, z, f1)))
   user  system elapsed 
 12.185   0.206  12.901   



Linux (winnie)
system.time(invisible(.Call("R_evalFunBody", x, y, z, body(f1))))
   user  system elapsed 
  2.087   0.050   2.138 

 system.time(invisible(.Call("R_inlineFunCall", x, y, z, f1)))
   user  system elapsed 
 10.882   0.054  10.937 

# 5.115529


f1.clos =
(function(n = 4)  {
  function(x, y, z)
      x + n
})()

