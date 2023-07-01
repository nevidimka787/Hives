#pragma once

#include <Stream.h>

#include "return_codes.hpp"
#include "print_debug.hpp"

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

// check that time in valid range
// @param time - input time
// @return SUCCESS if time is valid else ERORR
return_code_t checkTimeValid(const struct date_time& time);

// compare dates
// @param date_1 - first date
// @patam date_2 - second date
// @return 1 if date_1 newer then date_2, 0 if date are same, -1 if date_2 newer then date_1
int8_t compareDate(const struct date_time& date_1, const struct date_time& date_2);

// compare dates include times
// @param date_1 - first date and time
// @patam date_2 - second date and time
// @return 1 if date_1 newer then date_2, 0 if date are same, -1 if date_2 newer then date_1
int8_t compareDateTime(const struct date_time& date_1, const struct date_time& date_2);

// compare dates
// @param date_1 - first time
// @patam date_2 - second time
// @return 1 if date_1 newer then date_2, 0 if date are same, -1 if date_2 newer then date_1
int8_t compareTime(const struct date_time& date_1, const struct date_time& date_2);

// @param time_in_sec - current time in second (0 is 00:00:00) will be writed to here
// @return always SUCCESS;
return_code_t getCurrentTimeInSeconds(unsigned long& time_in_sec);

// get seconds to the next date time update
// @param current_date_time - current date and time
// @return count of secconds to the next update
unsigned long getDateTimeUpdateDelay(const struct date_time& current_date_time);

// get days from date_2 to date_1
// @param date_1 - end date
// @param date_2 - begin date
// @return days count from date_2 to date_1 (from 23/01/01 to 23/01/02 is one day)
unsigned long getDaysBetweenDates(const struct date_time& date_1, const struct date_time& date_2);

// get days from year2 to year1
// @param year1 - end year
// @param year2 - begin year
// @return days count from year2 to year1 
int getDaysBetweenYears(uint8_t year1, uint8_t year2);

// get days in month
// @param date - store month and year
// return days count
int8_t getDaysInCurrentMonth(const struct date_time& date);

// get days to the nex year
// @param date - begin date
// @param days count
int getDaysToNextYear(const struct date_time& date);

// add a day to date
// @param date - changed date
// @return ERROR if date is incorrect, else SUCCESS
return_code_t setNextDay(struct date_time& date);

// get seconds from date_2 to date_1
// @param date_1 - end date
// @param date_2 - begin date
// @return count of seconds 
unsigned long getSecondsBetweenDates(const struct date_time& date_1, const struct date_time& date_2);

// get seconds to the next day
// @param time - begin time
// @return count of seconds
unsigned long getSecondsToNextDay(const struct date_time& time);

// convert time to seconds
// @param time - input time
// @return time in seconds
unsigned long getTimeInSecconds(const struct date_time& time);

// @param year - number of year
// @return true if year is leap, else false
bool isLeapYear(uint8_t year);

// print date and time to serial
// format yy/mm/dd,hh:mm:ss
// @param date_time_i - date and time
// @param serial - output stream
void printDateTime(struct date_time& date_time_i, Stream& serial);

struct date_time global_date_time; // defined in date_time_struct.hpp

return_code_t checkTimeValid(const struct date_time& time) {
  if (time.hour > (char)23 || time.minute > (char)59 || time.second > (char)59) {
     return ERROR;
  }

  return SUCCESS;
}

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
    printError(F("getDaysInCurrentMonth: month: "));
    printDebugInLine((int)date.month);
    printDebugInLine(F(" out of range\n"));
    return -1;
  }
  if (date.month - 1 == FEBRUARY && isLeapYear(date.year)) {
    return 29;
  }
  return days_in_months[date.month - 1];
}

/*
return delay in miliseconds
*/
unsigned long getDateTimeUpdateDelay(const struct date_time& current_date_time) {
  return ((3600L * 24L + 3600L - (unsigned long)current_date_time.hour * 3600L - (unsigned long)current_date_time.minute * 60L - (unsigned long)current_date_time.second) % (3600L * 24L)) * 1000L;
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

int8_t compareDate(const struct date_time& date_1, const struct date_time& date_2) {
  if (date_1.year > date_2.year) { return 1; }
  if (date_1.year < date_2.year) { return -1; }
  if (date_1.month > date_2.month) { return 1; }
  if (date_1.month < date_2.month) { return -1; }
  if (date_1.day > date_2.day) { return 1; }
  if (date_1.day < date_2.day) { return -1; }

  return 0;
}

int8_t compareTime(const struct date_time& date_1, const struct date_time& date_2) {
  if (date_1.hour > date_2.hour) { return 1; }
  if (date_1.hour < date_2.hour) { return -1; }
  if (date_1.minute > date_2.minute) { return 1; }
  if (date_1.minute < date_2.minute) { return -1; }
  if (date_1.second > date_2.second) { return 1; }
  if (date_1.second < date_2.second) { return -1; }

  return 0;
}

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

int getDaysBetweenYears(uint8_t year1, uint8_t year2) {
  int days = 0;

  for (uint8_t y = year2; y < year1; ++y) {
    days += isLeapYear(y) ? 366 : 365;
  }

  return days;
}

unsigned long getDaysBetweenDates(const struct date_time& date_1, const struct date_time& date_2) {
  unsigned long days_diff = getDaysBetweenYears(date_1.year, date_2.year);
  days_diff += getDaysToNextYear(date_2);
  days_diff -= getDaysToNextYear(date_1);
  return days_diff;  
}

unsigned long getSecondsBetweenDates(const struct date_time& date_1, const struct date_time& date_2) {
  unsigned long diff = getDaysBetweenDates(date_1, date_2);
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













