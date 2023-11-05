a : (x:int) int {
 return 5; 
}

b : (x:int) int { 
  a: int = 5; 
  return a;
}

c : (x:int) int {
 return true; 
}

d : (x:int) int {
 return x; 
}

g : (x:int, f:bool) int {
 return f; 
}

x : (y:int, f:bool) int {
 return;
}

y : (y:int, f:bool) void {
 return 5; 
}

z : (y:int, f:bool) int {
}

var1 : int;
var2 : bool;

main : () void {
  y(1, true);
  var1();
  g(true);
  d(true);
  var1 = b(1);
  var2 = b(1);
}