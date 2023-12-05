main : () void { }

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

c : (x:int) int {
 return true; 
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