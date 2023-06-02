#pragma once

#include <Stream.h>

#include "commands_list.hpp"
#include "date_time_struct.hpp"

struct ParsRequest {
  long commands_list;
  char phone_number[20];
  float max_temperature;
  float max_humidity;
  float max_weight;
  float min_temperature;
  float min_humidity;
  float min_weight;
  int sms_number;
  struct date_time date_time;
};

#ifdef SERIAL_DEBUG

// print list of active commands
// @param commands_list - list of commands
// @param serial - output stream
void printCommandsList(long commands_list, Stream& serial);

// print all parameters in request
// @param request - request with parameters
// @param serial - output stream
void printRequest(struct ParsRequest& request, Stream& serial = Serial);

void printRequest(struct ParsRequest& request, Stream& serial = Serial) {
  serial.print(F("printRequest: request:"));
  printCommandsList(request.commands_list, serial);
  serial.print(F("\nphone_number: "));
  serial.print(request.phone_number);
  serial.print(F("\nmax_temperature: "));
  serial.print(request.max_temperature);
  serial.print(F("\nmax_humidity: "));
  serial.print(request.max_humidity);
  serial.print(F("\nmax_weight: "));
  serial.print(request.max_weight);
  serial.print(F("\nmin_temperature: "));
  serial.print(request.min_temperature);
  serial.print(F("\nmin_humidity: "));
  serial.print(request.min_humidity);
  serial.print(F("\nmin_weight: "));
  serial.print(request.min_weight);
  serial.print(F("\nsms_number: "));
  serial.println(request.sms_number);

  return;
}

void printCommandsList(long commands_list, Stream& serial) {
  printDebug(F("printCommandsList:\n"));
  serial.print(F("commands_list: "));
  serial.println(commands_list);
  serial.print(F("active commands"));
  if (commands_list & DELETE_SMS_ALL) {
    serial.print(F("\nDELETE_SMS_ALL"));
  }
  if (commands_list & CHECK_OK) {
    serial.print(F("\nCHECK_OK"));
  }
  if (commands_list & CMGF_EN) {
    serial.print(F("\nCMGF_EN"));
  }
  if (commands_list & GET_LAST_SMS_ID) {
    serial.print(F("\nGET_LAST_SMS_ID"));
  }
  if (commands_list & GET_TIME) {
    serial.print(F("\nGET_TIME"));
  }
  if (commands_list & SEND_SMS) {
    serial.print(F("\nSEND_SMS"));
  }
  if (commands_list & SET_NUMBER) {
    serial.print(F("\nSET_NUMBER"));
  }
  if (commands_list & SET_MAX_TEMPERATURE) {
    serial.print(F("\nSET_MAX_TEMPERATURE"));
  }
  if (commands_list & SET_MAX_HUMIDITY) {
    serial.print(F("\nSET_MAX_HUMIDITY"));
  }
  if (commands_list & SET_MAX_WEIGHT) {
    serial.print(F("\nSET_MAX_WEIGHT"));
  }
  if (commands_list & SET_MIN_TEMPERATURE) {
    serial.print(F("\nSET_MIN_TEMPERATURE"));
  }
  if (commands_list & SET_MIN_HUMIDITY) {
    serial.print(F("\nSET_MIN_HUMIDITY"));
  }
  if (commands_list & SET_MIN_WEIGHT) {
    serial.print(F("\nSET_MIN_WEIGHT"));
  }
  if (commands_list & SET_SEND_TIME) {
    serial.print(F("\nSET_SEND_TIME"));
  }
  if (commands_list & PRINT_SMS) {
    serial.print(F("\nPRINT_SMS"));
  }
  if (commands_list & PRINT_SMS_ALL) {
    serial.print(F("\nPRINT_SMS_ALL"));
  }
  if (commands_list & PRINT_STORED_DATA) {
    serial.print(F("\nPRINT_STORED_DATA"));
  }
  if (commands_list & PRINT_MEASURED_DATA) {
    serial.print(F("\nPRINT_MEASURED_DATA"));
  }
  if (commands_list & UPDATE_DATE_TIME) {
    serial.print(F("\nUPDATE_DATE_TIME"));
  }
  serial.print('\n');
  return;
}

#endif // SERIAL_DEBUG























