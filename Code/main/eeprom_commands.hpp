#pragma once

#include <EEPROM.h>

#include "print_debug.hpp"
#include "return_codes.hpp"
#include "serial_helper.hpp"
#include "str_macroses.hpp"

using namespace return_codes;

enum ERROM_IDS {
  MAX_TEMPERATURE_ID      = 0,  // size = 4   type = float
  MIN_TEMPERATURE_ID      = 4,  // size = 4   type = float
  MAX_HUMIDITY_ID         = 8,  // size = 4   type = float
  MIN_HUMIDITY_ID         = 12, // size = 4   type = float
  PHONE_NUMBER_ID         = 16, // size = 20  type = char[]
  SEND_TIME_ID            = 36  // size = 3   type = char[]
};

struct StoredData {
  // public
  float min_temperature;
  float max_temperature;
  float min_humidity;
  float max_humidity;
  char send_time[3]; // hh:mm:ss

  // private
  char phone_number[20];
  char system_update_period;
};

return_code_t EEPROM_putStr(int idx, const char* str) {
  int c = 0;
  for (; str[c] != '\0'; ++c) {
    EEPROM.put(idx + c, str[c]);
  }
  EEPROM.put(idx + c, str[c]);
  
  return SUCCESS;
}

return_code_t EEPROM_putStr(int idx, char* str, unsigned size) {
  int c = 0;
  for (unsigned c = 0; c < size; ++c) {
    EEPROM.put(idx + c, str[c]);
  }
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

return_code_t EEPROM_getStr(int idx, char* str, unsigned size) {
  int c = 0;
  for (unsigned c = 0; c < size; ++c) {
    EEPROM.get(idx + c, str[c]);
  }
  return SUCCESS;
}

void returnToDefaults() {
  EEPROM_putStr(PHONE_NUMBER_ID, "00000000000");
  EEPROM_putStr(SEND_TIME_ID, (const char[]){(char)255, (char)255, (char)255}, 3);
  EEPROM.put(MAX_TEMPERATURE_ID, 100.0f);
  EEPROM.put(MIN_TEMPERATURE_ID, 0.0f);
  EEPROM.put(MAX_HUMIDITY_ID, 100.0f);
  EEPROM.put(MIN_HUMIDITY_ID, 0.0f);

  return;
}

struct StoredData getStoredData() {
  struct StoredData data;
  EEPROM_getStr(PHONE_NUMBER_ID, data.phone_number);
  EEPROM.get(MAX_TEMPERATURE_ID, data.max_temperature);
  EEPROM.get(MIN_TEMPERATURE_ID, data.min_temperature);
  EEPROM.get(MAX_HUMIDITY_ID, data.max_humidity);
  EEPROM.get(MIN_HUMIDITY_ID, data.min_humidity);
  EEPROM_getStr(SEND_TIME_ID, data.send_time, 3);

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

return_code_t setSendTime(struct date_time& date_time) {
  EEPROM.put(SEND_TIME_ID, date_time.hour);
  EEPROM.put(SEND_TIME_ID + 1, date_time.minute);
  EEPROM.put(SEND_TIME_ID + 2, date_time.second);
  return SUCCESS;
}

return_code_t getSendTime(struct date_time& date_time) {
  EEPROM.get(SEND_TIME_ID, date_time.hour);
  EEPROM.get(SEND_TIME_ID + 1, date_time.minute);
  EEPROM.get(SEND_TIME_ID + 2, date_time.second);
  return SUCCESS;
}

return_code_t printStoredDataTo(Stream& serial) {
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
  serial.print(F("printStoredDataTo: send_time: "));
  if (data.send_time[0] >= 24) {
    serial.print(F("not send enviromental data\n"));
  } else {
    if (data.send_time[0] < 10) {
      serial.print('0');
    }
    serial.print((int)data.send_time[0]);
    serial.print(':');
    if (data.send_time[1] < 10) {
      serial.print('0');
    }
    serial.print((int)data.send_time[1]);
    serial.print(':');
    if (data.send_time[2] < 10) {
      serial.print('0');
    }
    serial.println((int)data.send_time[2]);
  }
  
  return SUCCESS;
}

return_code_t shortPrintStoredDataTo(Stream& serial) {
  struct StoredData data;
  data = getStoredData();

  serial.print(F("Stored data\nMAX_TEMPERATURE: "));
  serial.print(data.max_temperature);
  serial.print(F(" C\nMIN_TEMPERATURE: "));
  serial.print(data.min_temperature);
  serial.print(F(" C\nMAX_HUMIDITY: "));
  serial.print(data.max_humidity);
  serial.print(F(" %\nMIN_HUMIDITY: "));
  serial.print(data.min_humidity);
  serial.print(F(" %\nSEND_TIME: "));
  if (data.send_time[0] >= 24) {
    serial.print(F("not send enviromental data\n"));
  } else {
    if (data.send_time[0] < 10) {
      serial.print('0');
    }
    serial.print((int)data.send_time[0]);
    serial.print(':');
    if (data.send_time[1] < 10) {
      serial.print('0');
    }
    serial.print((int)data.send_time[1]);
    serial.print(':');
    if (data.send_time[2] < 10) {
      serial.print('0');
    }
    serial.println((int)data.send_time[2]);
  }
  
  return SUCCESS;
}

























