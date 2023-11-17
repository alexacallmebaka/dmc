main : () int  {
  a : int = 10;
  b : bool = true;

  if (b){
    a = 0;
    b = false;
  }
  a = 2;

  if (b) {
    a = 0;
    b = false;
  } else {
    a = 2;
    b = true;
  }

  return a;
}
