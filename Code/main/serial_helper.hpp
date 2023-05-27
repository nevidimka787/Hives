#pragma once

#include "math.hpp"
#include "print_debug.hpp"
#include "str_macroses.hpp"

using namespace return_codes;

// timeout = times * 10 ns
return_code_t waitAvailable(Stream& serial, int times = 100) {
  if (serial.available()) {
    return SUCCESS;
  }
  for (int i = 0; i < times; ++i) {
    delay(10);
    if (serial.available()) {
      return SUCCESS;
    }
  }
  return ERROR;
}

return_code_t getIntFromSerial(Stream& serial, int& result, char& last_char) {
  if (waitAvailable(serial) != SUCCESS) {
    return ERROR;
  }

  int answer = -1;
  int exp = 1;

  while (waitAvailable(serial) == SUCCESS) {
    last_char = serial.read();
    if (!isDigitSymbol(last_char)) {
      break;
    }
    if (answer == -1) {
      answer = 0;
    } else if (answer < 0) {
      return ERROR;
    }
    char digit = last_char - '0';
    answer += (int)digit * exp;
    exp *= 10;
  }

  if (answer == -1) {
    return ERROR;
  }
  
  result = reverseNumber(answer);
  return SUCCESS;
}

return_code_t getByteFromSerial(Stream& serial, char& result, char& last_char) {
  if (waitAvailable(serial) != SUCCESS) {
    printError(F("getByteFromSerial: no input\n"));
    return ERROR;
  }

  int answer = -1;
  int exp = 1;

  while (waitAvailable(serial) == SUCCESS) {
    last_char = serial.read();
    if (!isDigitSymbol(last_char)) {
      break;
    }
    if (answer == -1) {
      answer = 0;
    } else if (answer < 0) {
      printError(F("getByteFromSerial: overflow\n"));
      return ERROR;
    }
    char digit = last_char - '0';
    answer += (int)digit * exp;
    exp *= 10;
  }

  if (answer == -1) {
    printError(F("getByteFromSerial: no number\n"));
    return ERROR;
  }
  
  result = reverseNumber(answer);
  return SUCCESS;
}

return_code_t getByteFromSerial(Stream& serial, char& result) {
  if (waitAvailable(serial) != SUCCESS) {
    printError(F("getByteFromSerial: no input\n"));
    return ERROR;
  }

  int answer = -1;
  int exp = 1;

  while (waitAvailable(serial) == SUCCESS) {
    char digit = serial.read();
    if (!isDigitSymbol(digit)) {
      break;
    }
    if (answer == -1) {
      answer = 0;
    } else if (answer < 0) {
      printError(F("getByteFromSerial: overflow\n"));
      return ERROR;
    }
    digit -= '0';
    answer += (int)digit * exp;
    exp *= 10;
  }

  if (answer == -1) {
    printError(F("getByteFromSerial: no number\n"));
    return ERROR;
  }
  
  result = reverseNumber(answer);
  return SUCCESS;
}

return_code_t scanPhoneNumber(Stream& serial, char* phone_number) {
  int p = 1; // zero symbol set in the first while cycle

  while (waitAvailable(serial) == SUCCESS) {
    phone_number[0] = serial.read();    // first symbol set here
    if (!isSpaceSymbol(phone_number[0])) {
      break;
    }
  }

  while (waitAvailable(serial) == SUCCESS) {
    if (p == 16) {
      printError(F("Phone number is too long.\n"));
      return ERROR;
    }

    phone_number[p] = serial.read();

    if (isSpaceSymbol(phone_number[p])) {
      phone_number[p] = '\0';
      break;
    }
    if (!isDigitSymbol(phone_number[p])) {
      printError(F("Phone number is incorect.\n"));
      return ERROR;
    }
    ++p;
  }

  return SUCCESS;
}





