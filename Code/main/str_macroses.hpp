#pragma once

// @param c - input symbol
// @return true if the symbol is space symbol (\n \t \r etc.), else false
bool isSpaceSymbol(char c);

// @param c - input symbol
// @return true if the symbol is digit in decemical number system
bool isDigitSymbol(char c);

// str1 > str2 if str1 above str2 in alphabetical order
// @param str1 - first string
// @param str2 - second string
// @return 1 if first string grater second string, 0 if strings are same, 1 if first string less second
int8_t compareStrs(const char* str1, const char* str2);

// @param str - changed string
// @param length - length of the string (will be increment in function)
void addToEndOfStr(char* str, int length, char symbol);

bool isSpaceSymbol(char c) {
  return (c == ' ' || c == '\t' || c == '\r' || c == '\n');
}

bool isDigitSymbol(char c) {
  return (c >= '0' && c <= '9');
}

int8_t compareStrs(const char* str1, const char* str2) {
  int p = 0;
  while (str1[p] != '\0') {
    if (str1[p] > str2[p]) {
      return 1;
    }
    if (str1[p] < str2[p]) {
      return -1;
    }
    ++p;
  }
  if (str1[p] < str2[p]) {
    return -1;
  }
  return 0;
}

void addToEndOfStr(char* str, int& length, char symbol) {
  for (int symbol_id = 0; symbol_id < length; ++symbol_id) {
    str[symbol_id] = str[symbol_id + 1];
  }
  str[length] = symbol;
  ++length;
  return;
}
