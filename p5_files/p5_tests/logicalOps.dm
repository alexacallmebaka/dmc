a:int;
b:bool = false;
c:bool = true;

f : () void {
	d:int;
}

main : () void {
  b = b and c;
  b = a or c;
  b = true or false;
  b = "true" or "false";
  b = f and c;
}
