#pragma once

#include <EEPROM.h>

#include "print_debug.hpp"
#include "return_codes.hpp"
#include "str_macroses.hpp"

enum ERROM_IDS {
  MAX_TEMPERATURE_ID  = 0,  // size = 4   type = float
  MIN_TEMPERATURE_ID  = 4,  // size = 4   type = float
  MAX_HUMIDITY_ID     = 8,  // size = 4   type = float
  MIN_HUMIDITY_ID     = 12  // size = 4   type = float
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

struct StoredData getStoredData() {
  struct StoredData data;
  EEPROM.get(PHONE_NUMBER_ID, data.phone_number);
  EEPROM.get(MAX_TEMPERATURE_ID, data.max_temperature);
  EEPROM.get(MIN_TEMPERATURE_ID, data.min_temperature);
  EEPROM.get(MAX_HUMIDITY_ID, data.max_humidity);
  EEPROM.get(MIN_HUMIDITY_ID, data.min_humidity);

  data.data_valid = SUCCESS;

  return data;  
}

return_code_t setPhoneNumber(const char& phone_number[20]) {
  EEPROM.put(PHONE_NUMBER_ID, phone_number);

  return SUCCESS;
}

return_code_t setPhoneNumberFromSerial() {
  int skip_spaces = 1;

  char phone_number[20] = "";
  int p = 0;

  while (Serial.available()) {
    char c = Serial.read();
    if (skip_spaces == 1 && !isSpaceSymbol(c)) {
      skip_spaces = 0;
    }

    if (skip_spaces == 0) {
      if (isSpaceSymbol(c)) {
        phone_number[p] = '\0';
        break;
      }
      if (!isDigitSymbol(c)) {
        printError("Phone number is incorect.\n");
        return ERROR;
      }
      if (p == 16) {
        printError("Phone number is too long.\n");
        return ERROR;
      }
      phone_number[p] = c;
      ++p;
    }
  }

  return setPhoneNumber(phone_number);
}

return_code_t setMaxTemperature(temperature) {
  EEPROM.put(MAX_TEMPERATURE_ID, temperature);
  return SUCCESS;
}

return_code_t setMaxTemperatureFromSerial() {
  return setMaxTemperature(Serial.parseFloat());
}

return_code_t setMinTemperature(temperature) {
  EEPROM.put(MIN_TEMPERATURE_ID, temperature);
  return SUCCESS;
}

return_code_t setMaxTemperatureFromSerial() {
  return setMinTemperature(Serial.parseFloat());
}

return_code_t setMaxHumidity(humidity) {
  EEPROM.put(MAX_HUMIDITY_ID, humidity);
  return SUCCESS;
}

return_code_t setMaxHumidityFromSerial() {
  return setMaxHumidity(Serial.parseFloat());
}

return_code_t setMinHumidity(humidity) {
  EEPROM.put(MIN_HUMIDITY_ID, humidity);
  return SUCCESS;
}

return_code_t setMaxHumidityFromSerial() {
  return setMinHumidity(Serial.parseFloat());
}

printStoredDataToSerial() {
  struct StoredData data;
  data = getStoredData();

  Serial.print("printStoredDataToSerial: phone number: +");
  Serial.print(data.phone_number.number_14_9);
  Serial.println(data.phone_number.number_8_0);
  Serial.print("printStoredDataToSerial: temperature max: ");
  Serial.println(data.max_temperature);
  Serial.print("printStoredDataToSerial: temperature min: ");
  Serial.println(data.min_temperature);
  Serial.print("printStoredDataToSerial: humidity max: ");
  Serial.println(data.max_humidity);
  Serial.print("printStoredDataToSerial: humidity min: ");
  Serial.println(data.min_humidity);
  
  return SUCCESS;
}


























