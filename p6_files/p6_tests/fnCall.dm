a : (x:int) int {
 return 5; 
}

b : (x:int) int { 
  a: int = 5; 
  return a;
}


d : (x:int) int {
 return x; 
}

x : (y:int, f:bool) int {
 return d(5);
}

var1 : int;

main : () void {
  var1 = b(1);
  var1 = x(var1, true);
}