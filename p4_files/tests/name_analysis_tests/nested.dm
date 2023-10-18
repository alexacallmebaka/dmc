f : (arg1 : int,arg2 : bool) int {
}

g : () void {
	a : int;
  if (true) {
    b : int;
  } else {
    b : int = 1;
    give b;
  }
	f(a, true);
	g();
}
