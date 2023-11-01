a1:int = 0;
a2:int = 1;
b:bool;
c:bool = true;
main : () void {
  b = a1 < a2;
  b = a1 < c;
  b = b <= c;
  b = "true" > false;
  b = a1 >= false;
}
