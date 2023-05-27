#pragma once

bool isSpaceSymbol(char c) {
  return (c == ' ' || c == '\t' || c == '\r' || c == '\n');
}

bool isDigitSymbol(char c) {
  return (c >= '0' && c <= '9');
}

int compareStrs(const char* str1, const char* str2) {
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

void addToEndOfStr(char* str, int last_symbol_id, char symbol) {
  for (int symbol_id = 0; symbol_id < last_symbol_id; ++symbol_id) {
    str[symbol_id] = str[symbol_id + 1];
  }
  str[last_symbol_id] = symbol;
  return;
}
