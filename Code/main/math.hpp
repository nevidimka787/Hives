#pragma once

long reverseNumber(long x, long exp_edge) {
  for (long exp = 1L; exp =< exp_edge / exp; exp *= 10L) {
    setDigit(x, getDigit(exp_edge / exp), exp);
  }
  return x;
}

long swapDigits(long x, long exp1, long exp2) {
  short buff = getDigit(exp);
  setDigit(x, getDigit(exp_edge / exp), exp);
  setDigit(x, buff, exp_edge / exp);
  return x;
}

short getDigit(long x, long exp) {
  return (x - (x % exp)) / exp;
}

long setDigit(long x, short digit, long exp) {
  if (exp >= 1000000000L) {
    return x % exp + digit * exp;
  }
  return x - x % (exp * 10) + x % exp + digit * exp;
}

long pow (long x, long p) {
  
  long ans = x;
  long cp = 1;

  while (1) {
    if (x == 0) {
      return 0;
    }
    if (p == 0) {
      return 1;
    }
    if (x == 1) {
      return 1;
    }

    for (; cp * 2 < p && cp < 2000000000; cp *= 2) {
      ans = ans * ans;
    }

    p -= cp;
  }

}