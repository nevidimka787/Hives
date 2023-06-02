#pragma once

#include "date_time_struct.hpp"
#include "print_debug.hpp"
#include "str_macroses.hpp"

using namespace return_codes;

// wait while serial will be available to read
// @param serial - followed stream
// @param timeout - time in milliseconds that the function will be waiting for available
// @return SUCCES if stream will be available during timeout, else ERROR
return_code_t waitAvailable(Stream& serial, int timeout = 1000);

// @param serial - input stream
// @param result - scaned value
// @param last_char - last scaned char. The char isn't a part of a number
// @return SUCCESS if value obtained, return error if no input or overflow
return_code_t getIntFromSerial(Stream& serial, int& result, char& last_char);

// @param serial - input stream
// @param result - scaned value
// @param last_char - last scaned char. The char isn't a part of a number
// @return SUCCESS if value obtained, return error if no input or overflow
return_code_t getByteFromSerial(Stream& serial, char& result, char& last_char);

// @param serial - input stream
// @param result - scaned value
// @return SUCCESS if value obtained, return error if no input or overflow
return_code_t getByteFromSerial(Stream& serial, char& result);

// Use "nan" to set nan.
// @param serial - input stream
// @param result - scaned value can be nan of +- inf
// @return SUCCESS if value obtained, return error if no input or input is  invalid.
return_code_t scanFloatFromSerial(Stream& serial, float& result);

// @param serial - input stream
// @param phone_number - string with number
// @return SUCCESS if phone number is obtained, ERROR if one is incorrect or too long
return_code_t scanPhoneNumber(Stream& serial, char* phone_number);

// @param date_time_i - output date (time isn't changed)
// @param serial - input stream
// @return SUCCESS if date is obtained, ERROR if one is incorrect
return_code_t scanDate(struct date_time& date_time_i, Stream& serial);

// @param date_time_i - output date (time isn't changed)
// @param serial - input stream
// @param last_char - last scaned char. The char isn't a part of a date
// @return SUCCESS if date is obtained, ERROR if one is incorrect
return_code_t scanDate(struct date_time& date_time_i, Stream& serial, char& last_char);

// @param date_time_i - output date and time
// @param serial - input stream
// @return SUCCESS if date and time is obtained, ERROR if date or time is incorrect
return_code_t scanDateTime(struct date_time& date_time_i, Stream& serial);

// @param date_time_i - output date and time
// @param serial - input stream
// @param last_char - last scaned char. The char isn't a part of a date or a time
// @return SUCCESS if date and time is obtained, ERROR if date or time is incorrect
return_code_t scanDateTime(struct date_time& date_time_i, Stream& serial, char& last_char);

// @param date_time_i - output time (date isn't changed)
// @param serial - input stream
// @return SUCCESS if time is obtained, ERROR if one is incorrect
return_code_t scanTime(struct date_time& date_time_i, Stream& serial);

// @param date_time_i - output time (date isn't changed)
// @param serial - input stream
// @param last_char - last scaned char. The char isn't a part of a time
// @return SUCCESS if time is obtained, ERROR if one is incorrect
return_code_t scanTime(struct date_time& date_time_i, Stream& serial, char& last_char);

// safety read from serial function
// @param serail - input stream
// @param not_available_symbol - return this symbol if the serial does't available
char readAwailable(Stream& serial, char not_available_symbol = '\0');

return_code_t waitAvailable(Stream& serial, int timeout = 1000) {
  if (serial.available()) {
    return SUCCESS;
  }
  for (int i = 0; i < timeout; i += 10) {
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
  
  result = 0;
  bool overflow = false;

  while (waitAvailable(serial) == SUCCESS) {
    last_char = serial.read();
    if (!isDigitSymbol(last_char)) {
      break;
    }
    if (overflow == true) {
      return ERROR;
    }
    
    result *= 10;
    
    result += (last_char - '0');
    if (result * 10 / 10 != result) {
      overflow = true;
    }
  }

  return SUCCESS;
}

return_code_t getByteFromSerial(Stream& serial, char& result, char& last_char) {
  if (waitAvailable(serial) != SUCCESS) {
    return ERROR;
  }
  
  result = 0;
  bool overflow = false;

  while (waitAvailable(serial) == SUCCESS) {
    last_char = serial.read();
    if (!isDigitSymbol(last_char)) {
      break;
    }
    if (overflow == true) {
      return ERROR;
    }
    
    result *= 10;
    
    result += (last_char - '0');
    if (result * 10 / 10 != result) {
      overflow = true;
    }
  }

  return SUCCESS;
}

return_code_t getByteFromSerial(Stream& serial, char& result) {
  if (waitAvailable(serial) != SUCCESS) {
    return ERROR;
  }
  
  result = 0;
  bool overflow = false;

  while (waitAvailable(serial) == SUCCESS) {
    char last_char = serial.read();
    if (!isDigitSymbol(last_char)) {
      break;
    }
    if (overflow == true) {
      return ERROR;
    }
    
    result *= 10;
    
    result += (last_char - '0');
    if (result * 10 / 10 != result) {
      overflow = true;
    }
  }

  return SUCCESS;
}

return_code_t scanFloatFromSerial(Stream& serial, float& result) {
  char c;
  result = 0.0f;
  bool sign = false; // positive
  do {
    if (waitAvailable(serial) != SUCCESS) {
      return ERROR;
    }
    c = serial.read();
  } while (isSpaceSymbol(c));

  if (c == '-') {
    sign = true; // negative
    if (waitAvailable(serial) != SUCCESS) {
      return ERROR;
    }
    c = serial.read();
  } else if ((c == 'n' || c == 'N') && waitAvailable(serial) == SUCCESS) {
    c = serial.read();
    if (c == 'a' || c == 'A' && waitAvailable(serial) == SUCCESS) {
      c = serial.read();
      if (c == 'n' || c == 'N') {
        result = 0.0f / 0.0f;
        return SUCCESS;
      }
    }
    return ERROR;
  }


  if (!isDigitSymbol(c)) {
    return ERROR;
  }

  do {
    result *= 10.f;
    result += (float)(c - '0');
    if (waitAvailable(serial) != SUCCESS) {
      if (sign) {
        result = -result;
      }
      return SUCCESS;
    }
    c = serial.read();
  } while (isDigitSymbol(c));

  if (c != '.') {
    if (sign) {
      result = -result;
    }
    return SUCCESS;
  }

  float power = 0.1f;

  while (waitAvailable(serial) == SUCCESS) {
    c = serial.read();
    if (!isDigitSymbol(c)) {
      if (sign) {
        result = -result;
      }
      return SUCCESS;
    }
    result += (float)(c - '0') * power;
    power /= 10.0f;
  }
  
  if (sign) {
    result = -result;
  }
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

return_code_t scanDate(struct date_time& date_time_i, Stream& serial) {
  if (getByteFromSerial(serial, date_time_i.year) != SUCCESS) {
    return ERROR;
  }
  if (getByteFromSerial(serial, date_time_i.month) != SUCCESS) {
    return ERROR;
  }
  return getByteFromSerial(serial, date_time_i.day);
}

return_code_t scanDate(struct date_time& date_time_i, Stream& serial, char& last_char) {
  if (getByteFromSerial(serial, date_time_i.year) != SUCCESS) {
    return ERROR;
  }
  if (getByteFromSerial(serial, date_time_i.month) != SUCCESS) {
    return ERROR;
  }
  return getByteFromSerial(serial, date_time_i.day, last_char);
}

return_code_t scanDateTime(struct date_time& date_time_i, Stream& serial) {
  if (scanDate(date_time_i, serial) != SUCCESS) {
    return ERROR;
  }
  return scanTime(date_time_i, serial);
}

return_code_t scanDateTime(struct date_time& date_time_i, Stream& serial, char& last_char) {
  if (scanDate(date_time_i, serial) != SUCCESS) {
    return ERROR;
  }
  return scanTime(date_time_i, serial, last_char);
}

return_code_t scanTime(struct date_time& date_time_i, Stream& serial) {
  if (getByteFromSerial(serial, date_time_i.hour) != SUCCESS) {
    return ERROR;
  }
  if (getByteFromSerial(serial, date_time_i.minute) != SUCCESS) {
    return ERROR;
  }
  return getByteFromSerial(serial, date_time_i.second);
}

return_code_t scanTime(struct date_time& date_time_i, Stream& serial, char& last_char) {
  if (getByteFromSerial(serial, date_time_i.hour) != SUCCESS) {
    return ERROR;
  }
  if (getByteFromSerial(serial, date_time_i.minute) != SUCCESS) {
    return ERROR;
  }
  return getByteFromSerial(serial, date_time_i.second, last_char);
}

char readAwailable(Stream& serial, char not_available_symbol = '\0') {
  if (waitAvailable(serial) != SUCCESS) {
    return not_available_symbol;
  }
  return serial.read();
}





