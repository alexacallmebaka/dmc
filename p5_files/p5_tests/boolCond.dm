a:int;
b:int = 1;
c:bool = true;
f : () void {
	d:int;
}
C : class {
};
main : () void {
  a = 1;
  if (a == b) {
    if (c) {}
    else {
      while (C) {}
    }
  }
  if (false) {}
  while (false) {}
  while (a) {}
  while (f) {}
}