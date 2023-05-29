#pragma once

#include <Stream.h>

#include "commands_list.hpp"
#include "date_time_struct.hpp"

struct ParsRequest {
  long commands_list;
  char phone_number[20];
  float max_temperature;
  float max_humidity;
  float min_temperature;
  float min_humidity;
  int sms_number;
  struct date_time date_time;
};

void printCommandsList(long commands_list, Stream& serial);

void printRequest(struct ParsRequest& request, Stream& serial = Serial) {
  serial.print(F("printRequest: request:"));
  printCommandsList(request.commands_list, serial);
  serial.print(F("\nphone_number: "));
  serial.print(request.phone_number);
  serial.print(F("\nmax_temperature: "));
  serial.print(request.max_temperature);
  serial.print(F("\nmax_humidity: "));
  serial.print(request.max_humidity);
  serial.print(F("\nmin_temperature: "));
  serial.print(request.min_temperature);
  serial.print(F("\nmin_humidity: "));
  serial.print(request.min_humidity);
  serial.print(F("\nsms_number: "));
  serial.println(request.sms_number);

  return;
}

void printCommandsList(long commands_list, Stream& serial) {
  serial.print(F("commands_list: "));
  serial.print(commands_list);
  serial.print(F("\DELETE_SMS_ALL = "));
  serial.print(commands_list & DELETE_SMS_ALL ? '1' : '0');
  serial.print(F("\nCHECK_OK = "));
  serial.print(commands_list & CHECK_OK ? '1' : '0');
  serial.print(F("\nCMGF_EN = "));
  serial.print(commands_list & CMGF_EN ? '1' : '0');
  serial.print(F("\nGET_LAST_SMS_ID = "));
  serial.print(commands_list & GET_LAST_SMS_ID ? '1' : '0');
  serial.print(F("\nSET_NUMBER = "));
  serial.print(commands_list & SET_NUMBER ? '1' : '0');
  serial.print(F("\nSET_MAX_TEMPERATURE = "));
  serial.print(commands_list & SET_MAX_TEMPERATURE ? '1' : '0');
  serial.print(F("\nSET_MAX_HUMIDITY = "));
  serial.print(commands_list & SET_MAX_HUMIDITY ? '1' : '0');
  serial.print(F("\nSET_MIN_TEMPERATURE = "));
  serial.print(commands_list & SET_MIN_TEMPERATURE ? '1' : '0');
  serial.print(F("\nSET_MIN_HUMIDITY = "));
  serial.print(commands_list & SET_MIN_HUMIDITY ? '1' : '0');
  serial.print(F("\nPRINT_SMS = "));
  serial.print(commands_list & PRINT_SMS ? '1' : '0');
  serial.print(F("\nPRINT_SMS_ALL = "));
  serial.print(commands_list & PRINT_SMS_ALL ? '1' : '0');
  serial.print(F("\nPRINT_STORED_DATA = "));
  serial.print(commands_list & PRINT_STORED_DATA ? '1' : '0');
  serial.print(F("\nPRINT_MEASURED_DATA = "));
  serial.println(commands_list & PRINT_MEASURED_DATA ? '1' : '0');

  return;
}























