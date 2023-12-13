bar : ( f1: int, f2: int, f3: int, f4: int, f5: int, f6: int, f7: int, f8: int, f9: int) void {
  b: int;
  b = f8;
  give b;
  give f1;
}

bar2 : ( f1: int, f2: int, f3: int, f4: int) void {
  b: int;
  b = f1;
  give f1;
  give b;
  give f4;
}
bar3 : (f : int) int {
  give "bar3";
  return f;
}

main : () void { 

bar2(1,2,3,4);
bar(1,2,3,4,5,6,7,8,9);
a : int =bar3(1);
give a;

}
