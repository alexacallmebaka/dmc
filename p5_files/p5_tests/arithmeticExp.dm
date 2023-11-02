a:int;
b:int = 1;
c:bool = true;
main : () void {
  a = 1 + true;
  a = true + false;
  a = "EECS" + 665;
  a = 1 + 2;

  a = true - 1;
  a = "EECS" - 665;
  a = 1 - 2;

  a = true * 1;
  a = "EECS" * 665;
  a = 1 * 2;

  a = true / 1;
  a = "EECS" / 665;
  a = 1 / 2;

  a = -true;

  b++;
  b--;
  c++;
  c--;
  d: perfect int;
  a = a + d;
}
