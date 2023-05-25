#pragma once

#include <EEPROM.h>

#include "print_debug.hpp"
#include "return_codes.hpp"
#include "serial_helper.hpp"
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

void getPhoneNumber(char* phone_number) {
  EEPROM_getStr(PHONE_NUMBER_ID, phone_number);
}

return_code_t setPhoneNumber(const char phone_number[20]) {
  EEPROM_putStr(PHONE_NUMBER_ID, phone_number);

  return SUCCESS;
}

return_code_t setMaxTemperature(float temperature) {
  EEPROM.put(MAX_TEMPERATURE_ID, temperature);
  return SUCCESS;
}

return_code_t setMaxTemperature(Stream& serial) {
  return setMaxTemperature(serial.parseFloat());
}

return_code_t setMinTemperature(float temperature) {
  EEPROM.put(MIN_TEMPERATURE_ID, temperature);
  return SUCCESS;
}

return_code_t setMinTemperature(Stream& serial) {
  return setMinTemperature(serial.parseFloat());
}

return_code_t setMaxHumidity(float humidity) {
  EEPROM.put(MAX_HUMIDITY_ID, humidity);
  return SUCCESS;
}

return_code_t setMaxHumidity(Stream& serial) {
  return setMaxHumidity(serial.parseFloat());
}

return_code_t setMinHumidity(float humidity) {
  EEPROM.put(MIN_HUMIDITY_ID, humidity);
  return SUCCESS;
}

return_code_t setMinHumidity(Stream& serial) {
  return setMinHumidity(serial.parseFloat());
}

printStoredDataTo(Stream& serial) {
  struct StoredData data;
  data = getStoredData();

  serial.print(F("printStoredDataTo: phone number: +"));
  serial.print(data.phone_number);
  serial.print(F("\nprintStoredDataTo: temperature max: "));
  serial.println(data.max_temperature);
  serial.print(F("printStoredDataTo: temperature min: "));
  serial.println(data.min_temperature);
  serial.print(F("printStoredDataTo: humidity max: "));
  serial.println(data.max_humidity);
  serial.print(F("printStoredDataTo: humidity min: "));
  serial.println(data.min_humidity);
  
  return SUCCESS;
}


























