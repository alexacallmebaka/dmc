a:int;
b:int = 1;
c:bool = true;
main : () void {
  a = 1 + 2;
  a = 1 - 2;
  a = 1 * 2;
  a = b / 2;
  a = -1;
  a++;
  a--;

  c = true;
  c = (a < 1);
  c = (a > 1);
  c = (a >= 1);
  c = (a <= b);
}