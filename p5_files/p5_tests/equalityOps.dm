a1:int = 0;
a2:int = 1;
b:bool;
c:bool = true;

f : () int {
	a:int;
	a = 4;
}

C : class {
};

main : () void {
  b = a1 == a2;
  b = a1 == c;
  b = b != c;
  b = "true" != "true";
  b = a1 != false;
  b = a1 == C;
  b = C != f;
}
