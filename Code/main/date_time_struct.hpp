#pragma once

#include <Stream.h>

#include "return_codes.hpp"
#include "serial_helper.hpp"

struct date_time {
  char year;
  char month;
  char day;
  char hour;
  char minute;
  char second;
};

using namespace return_codes;

void printDateTime(struct date_time& date_time_i, Stream& serial) {
  serial.print("printDateTime: yy/mm/dd,hh:mm:ss ")
  if (date_time_i.year < 10) {
    serial.print('0');
  }
  serial.print((int)date_time_i.year);
  serial.print('/');
  if (date_time_i.month < 10) {
    serial.print('0');
  }
  serial.print((int)date_time_i.month);
  serial.print('/');
  if (date_time_i.day < 10) {
    serial.print('0');
  }
  serial.print((int)date_time_i.day);
  serial.print(',');
  if (date_time_i.hour < 10) {
    serial.print('0');
  }
  serial.print((int)date_time_i.hour);
  serial.print(':');
  if (date_time_i.minute < 10) {
    serial.print('0');
  }
  serial.print((int)date_time_i.minute);
  serial.print(':');
  if (date_time_i.second < 10) {
    serial.print('0');
  }
  serial.println((int)date_time_i.second);
}

return_code_t scanDateTime(struct date_time& date_time_i, Stream& serial) {
  if (getByteFromSerial(serial, date_time_i.year) != SUCCESS) {
    return ERROR;
  }
  if (getByteFromSerial(serial, date_time_i.month) != SUCCESS) {
    return ERROR;
  }
  if (getByteFromSerial(serial, date_time_i.day) != SUCCESS) {
    return ERROR;
  }
  if (getByteFromSerial(serial, date_time_i.hour) != SUCCESS) {
    return ERROR;
  }
  if (getByteFromSerial(serial, date_time_i.minute) != SUCCESS) {
    return ERROR;
  }
  return getByteFromSerial(serial, date_time_i.second);
}

return_code_t scanDateTime(struct date_time& date_time_i, Stream& serial, char& last_char) {
  if (getByteFromSerial(serial, date_time_i.year) != SUCCESS) {
    return ERROR;
  }
  if (getByteFromSerial(serial, date_time_i.month) != SUCCESS) {
    return ERROR;
  }
  if (getByteFromSerial(serial, date_time_i.day) != SUCCESS) {
    return ERROR;
  }
  if (getByteFromSerial(serial, date_time_i.hour) != SUCCESS) {
    return ERROR;
  }
  if (getByteFromSerial(serial, date_time_i.minute) != SUCCESS) {
    return ERROR;
  }
  return getByteFromSerial(serial, date_time_i.second, last_char);
}

/*
return delay in miliseconds
*/
unsigned long getdateTimeUpdateDelay(const struct date_time& current_date_time) {
  return ((3600L * 24L + 3600L - (unsigned long)current_date_time.hour * 3600L - (unsigned long)current_date_time.minute * 60L - (unsigned long)current_date_time.second) % (3600L * 24L)) * 1000L;
}















