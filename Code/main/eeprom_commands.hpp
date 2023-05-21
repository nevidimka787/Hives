#pragma once

#include <EEPROM.h>

#include "print_debug.hpp"
#include "return_codes.hpp"
#include "str_macroses.hpp"

using namespace return_codes;

enum ERROM_IDS {
  MAX_TEMPERATURE_ID  = 0,  // size = 4   type = float
  MIN_TEMPERATURE_ID  = 4,  // size = 4   type = float
  MAX_HUMIDITY_ID     = 8,  // size = 4   type = float
  MIN_HUMIDITY_ID     = 12,  // size = 4   type = float
  PHONE_NUMBER_ID     = 16  // size = 20  type = char[]
};

struct StoredData {
  char phone_number[20];
  float min_temperature;
  float max_temperature;
  float min_humidity;
  float max_humidity;

  return_code_t data_valid;
};

return_code_t EEPROM_putStr(int idx, const char* str) {
  int c = 0;
  for (; str[c] != '\0'; ++c) {
    EEPROM.put(idx + c, str[c]);
  }
  EEPROM.put(idx + c, str[c]);
  
  printDebug("EEPROM_putStr: ");
  printDebugInLine(c + 1);
  printDebugInLine("\n");
  return SUCCESS;
}

return_code_t EEPROM_getStr(int idx, char* str) {
  int c = 0;
  for (; ; ++c) {
    EEPROM.get(idx + c, str[c]);
    if (str[c] == '\0') {
      break;
    }
  }
  printDebug("EEPROM_getStr: ");
  printDebugInLine(c);
  printDebugInLine("\n");
  return SUCCESS;
}

struct StoredData getStoredData() {
  struct StoredData data;
  EEPROM_getStr(PHONE_NUMBER_ID, data.phone_number);
  EEPROM.get(MAX_TEMPERATURE_ID, data.max_temperature);
  EEPROM.get(MIN_TEMPERATURE_ID, data.min_temperature);
  EEPROM.get(MAX_HUMIDITY_ID, data.max_humidity);
  EEPROM.get(MIN_HUMIDITY_ID, data.min_humidity);

  data.data_valid = SUCCESS;

  return data;  
}

return_code_t setPhoneNumber(const char phone_number[20]) {
  printDebug("setPhoneNumber: ");
  printDebugInLine(phone_number);
  printDebugInLine("\n");
  EEPROM_putStr(PHONE_NUMBER_ID, phone_number);

  return SUCCESS;
}

return_code_t setPhoneNumberFromSerial() {
  int skip_spaces = 1;

  char phone_number[20] = "";
  int p = 1; // zero symbol set in the first while cycle

SKIP_SPACES:
  while (Serial.available()) {
    phone_number[0] = Serial.read();    // first symbol set here
    if (!isSpaceSymbol(phone_number[0])) {
      goto GET_PHONE_NUMBER;
    }
  }

  for (int i = 0; i < 1000; ++i) {
    delay(10);
    if (Serial.available()) {
      goto SKIP_SPACES;
    }
  }

GET_PHONE_NUMBER:
  while (Serial.available()) {
    if (p == 16) {
      printError("Phone number is too long.\n");
      return ERROR;
    }

    phone_number[p] = Serial.read();

    if (isSpaceSymbol(phone_number[p])) {
      phone_number[p] = '\0';
      break;
    }
    if (!isDigitSymbol(phone_number[p])) {
      printError("Phone number is incorect.\n");
      return ERROR;
    }
    ++p;
  }
  
  for (int i = 0; i < 1000; ++i) {
    delay(10);
    if (Serial.available()) {
      goto GET_PHONE_NUMBER;
    }
  }

  printDebug("setPhoneNumberFromSerial: ");
  printDebugInLine(phone_number);
  printDebugInLine("\n");

  return setPhoneNumber(phone_number);
}

return_code_t setMaxTemperature(float temperature) {
  EEPROM.put(MAX_TEMPERATURE_ID, temperature);
  return SUCCESS;
}

return_code_t setMaxTemperatureFromSerial() {
  return setMaxTemperature(Serial.parseFloat());
}

return_code_t setMinTemperature(float temperature) {
  EEPROM.put(MIN_TEMPERATURE_ID, temperature);
  return SUCCESS;
}

return_code_t setMinTemperatureFromSerial() {
  return setMinTemperature(Serial.parseFloat());
}

return_code_t setMaxHumidity(float humidity) {
  EEPROM.put(MAX_HUMIDITY_ID, humidity);
  return SUCCESS;
}

return_code_t setMaxHumidityFromSerial() {
  return setMaxHumidity(Serial.parseFloat());
}

return_code_t setMinHumidity(float humidity) {
  EEPROM.put(MIN_HUMIDITY_ID, humidity);
  return SUCCESS;
}

return_code_t setMinHumidityFromSerial() {
  return setMinHumidity(Serial.parseFloat());
}

printStoredDataToSerial() {
  struct StoredData data;
  data = getStoredData();

  Serial.print("printStoredDataToSerial: phone number: +");
  Serial.print(data.phone_number);
  Serial.print("\nprintStoredDataToSerial: temperature max: ");
  Serial.println(data.max_temperature);
  Serial.print("printStoredDataToSerial: temperature min: ");
  Serial.println(data.min_temperature);
  Serial.print("printStoredDataToSerial: humidity max: ");
  Serial.println(data.max_humidity);
  Serial.print("printStoredDataToSerial: humidity min: ");
  Serial.println(data.min_humidity);
  
  return SUCCESS;
}


























