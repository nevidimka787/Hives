#pragma once

#include <Stream.h>

#include "return_codes.hpp"
#include "serial_helper.hpp"

namespace date_time {
  enum MONTS {
    JANUARY   = 0,  // 31
    FEBRUARY  = 1,  // 28 (29)
    MARCH     = 2,  // 31
    APRIL     = 3,  // 30
    MAY       = 4,  // 31
    JUNE      = 5,  // 30
    JULY      = 6,  // 31
    AUGUST    = 7,  // 31
    SEPTEMBER = 8,  // 30
    OCTOBER   = 9,  // 31
    NOVEMBER  = 10, // 30
    DECEMBER  = 11  // 31
  };

  struct date_time {
    char year;
    char month;
    char day;
    char hour;
    char minute;
    char second;
  };
}


using namespace return_codes;
using namespace date_time;


struct date_time global_date_time; // defined in date_time_struct.hpp

void printDateTime(struct date_time& date_time_i, Stream& serial) {
  serial.print("printDateTime: yy/mm/dd,hh:mm:ss ");
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

bool isLeapYear(uint8_t year) {
  return year % 400 == 0 || (year % 4 == 0 && year % 100 != 0);
}

int8_t getDaysInCurrentMonth(const struct date_time& date) {
  const int8_t days_in_months[12] = {
    (int8_t)31,
    (int8_t)28,
    (int8_t)31,
    (int8_t)30,
    (int8_t)31,
    (int8_t)30,
    (int8_t)31,
    (int8_t)31,
    (int8_t)30,
    (int8_t)31,
    (int8_t)30,
    (int8_t)31
  };

  if (date.month < 1 || date.month > 12) {
    printError(F("getDaysInCurrentMonth: month out of range"));
    return -1;
  }
  if (date.month - 1 == FEBRUARY && isLeapYear(date.year)) {
    return 29;
  }
  return days_in_months[date.month - 1];
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

return_code_t scanTime(struct date_time& date_time_i, Stream& serial) {
  if (getByteFromSerial(serial, date_time_i.hour) != SUCCESS) {
    return ERROR;
  }
  if (getByteFromSerial(serial, date_time_i.minute) != SUCCESS) {
    return ERROR;
  }
  return getByteFromSerial(serial, date_time_i.second);
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

return_code_t scanTime(struct date_time& date_time_i, Stream& serial, char& last_char) {
  if (getByteFromSerial(serial, date_time_i.hour) != SUCCESS) {
    return ERROR;
  }
  if (getByteFromSerial(serial, date_time_i.minute) != SUCCESS) {
    return ERROR;
  }
  return getByteFromSerial(serial, date_time_i.second, last_char);
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

/*
return delay in miliseconds
*/
unsigned long getdateTimeUpdateDelay(const struct date_time& current_date_time) {
  return ((3600L * 24L + 3600L - (unsigned long)current_date_time.hour * 3600L - (unsigned long)current_date_time.minute * 60L - (unsigned long)current_date_time.second) % (3600L * 24L)) * 1000L;
}

unsigned long getEventDelay(const struct date_time& event_date_time, const struct date_time& current_date_time) {
  (3600L * 24L + (unsigned long)event_date_time.hour * 3600L + (unsigned long)event_date_time.minute * 60L + (unsigned long)event_date_time.second -
  (unsigned long)current_date_time.hour * 3600L - (unsigned long)current_date_time.minute * 60L - (unsigned long)current_date_time.second) % (3600L * 24L) * 1000L;
}

int getDaysToNextYear(const struct date_time& date) {
  int days_until_next_year = getDaysInCurrentMonth(date) - date.day;
  if (days_until_next_year < 0) {
    return -1;
  }
  
  for (uint8_t m = date.month + 1; m < 12; ++m) {
    days_until_next_year += getDaysInCurrentMonth((struct date_time){.year = date.year, .month = m, .day = date.day});
  }

  if (isLeapYear(date.year) && (date.month == FEBRUARY && date.day < 29)) {
    ++days_until_next_year;
  }

  return days_until_next_year;
}

/*
@return -1 if date_1 < date_2, 0 if date_1 == date_2, 1 if date_1 > date_2
*/
int8_t compareDate(const struct date_time& date_1, const struct date_time& date_2) {
  if (date_1.year > date_2.year) { return 1; }
  if (date_1.year < date_2.year) { return -1; }
  if (date_1.month > date_2.month) { return 1; }
  if (date_1.month < date_2.month) { return -1; }
  if (date_1.day > date_2.day) { return 1; }
  if (date_1.day < date_2.day) { return -1; }

  return 0;
}

/*
@return -1 if date_1 < date_2, 0 if date_1 == date_2, 1 if date_1 > date_2
*/
int8_t compareTime(const struct date_time& date_1, const struct date_time& date_2) {
  if (date_1.hour > date_2.hour) { return 1; }
  if (date_1.hour < date_2.hour) { return -1; }
  if (date_1.minute > date_2.minute) { return 1; }
  if (date_1.minute < date_2.minute) { return -1; }
  if (date_1.second > date_2.second) { return 1; }
  if (date_1.second < date_2.second) { return -1; }

  return 0;
}

/*
@return -1 if date_1 < date_2, 0 if date_1 == date_2, 1 if date_1 > date_2
*/
int8_t compareDateTime(const struct date_time& date_1, const struct date_time& date_2) {
  int8_t result = compareDate(date_1, date_2);
  if (result == 0) {
    return compareTime(date_1, date_2);
  }
  return result;
}

unsigned long getSecondsToNextDay(const struct date_time& date) {
  return 3600UL * (24 - date.hour) - 60UL * date.minute - date.second;
}

unsigned long getTimeInSecconds(const struct date_time& date) {
  return 3600UL * date.hour + 60UL * date.minute + date.second;
}

//y1 >= y2
int getDaysBetweenYears(uint8_t year1, uint8_t year2) {
  int days = 0;

  for (uint8_t y = year2; y < year1; ++y) {
    days += isLeapYear(y) ? 366 : 365;
  }

  return days;
}

// date1 >= date2
unsigned long getDaysBeetweenDates(const struct date_time& date_1, const struct date_time& date_2) {
  unsigned long days_diff = getDaysToNextYear(date_1) - getDaysToNextYear(date_2);
  days_diff += getDaysBetweenYears(date_1.year, date_2.year);
  return days_diff;  
}

// date1 >= date2
unsigned long getSecondsBeetweenDates(const struct date_time& date_1, const struct date_time& date_2) {
  unsigned long diff = getDaysBeetweenDates(date_1, date_2);
  diff *= 3600L * 24L;
  diff += ((long)date_1.hour - date_2.hour) * 3600L + ((int)date_1.minute - date_2.minute) * 60 + (int)date_1.second - date_2.second;
  return diff;
}

return_code_t getCurrentTimeInSeconds(unsigned long& time_in_sec) {
  time_in_sec = ((millis() - date_time_last_update_time_point) / 1000 + getTimeInSecconds(global_date_time)) % (3600LU * 24);
  return SUCCESS;
}

return_code_t setNextDay(struct date_time& date) {
  if (getDaysInCurrentMonth(date) == -1) {
    return ERROR;
  }

  ++date.day;
  while (date.day > getDaysInCurrentMonth(date)) {
    date.day -= getDaysInCurrentMonth(date);
    ++date.month;
    if (date.month > 12) {
      date.month -= 12;
      ++date.year;
    }
  }

  return SUCCESS;
}













