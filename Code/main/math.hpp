#pragma once

short getDigit(long x, long exp) {
  return (x - (x % (exp * 10))) / exp;
}

long setDigit(long x, short digit, long exp) {
  if (exp >= 1000000000L) {
    return x % exp + digit * exp;
  }
  return x - x % (exp * 10) + x % exp + digit * exp;
}

long swapDigits(long x, long exp1, long exp2) {
  short buff = getDigit(x, exp1);
  setDigit(x, getDigit(x, exp2), exp1);
  setDigit(x, buff, exp2);
  return x;
}

long reverseNumber(long n) {
  long reverse = 0;
  long remainder;

  while (n != 0) {
    remainder = n % 10;
    reverse = reverse * 10 + remainder;
    n /= 10;
  }

  return reverse;
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