a:int;
b:bool;

f : () void {
	a = b;
}
C : class {
};
main : () int {
  a : perfect int = 5;
  take f;
  take a;
	b : int;
	b = f;
	b = C;
}