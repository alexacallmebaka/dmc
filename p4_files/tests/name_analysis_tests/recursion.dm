func : (x : int) int {
  if (x == 0) {
    return x;
  }
  return func(x);
}