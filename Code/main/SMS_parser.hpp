#pragma once

#include "command_parser.hpp"
#include "print_debug.hpp"
#include "serial_helper.hpp"
#include "str_macroses.hpp"

/*
@return number of SMS or 0
*/
int getNewestSMSFromNumberUntilOK(Stream& serial) {
  struct date_time {
    char year;
    char month;
    char day;
    char hour;
    char minute;
    char second;
  } max_date_time = {
    .year   = 0,
    .month  = 0,
    .day    = 0,
    .hour   = 0,
    .minute = 0,
    .second = 0
  };
  int max_id = 0;
  char c;
  
  if (waitAvailable(serial) != SUCCESS) {
    printDebug(F("getNewestSMSFromNumberUntilOK: no input\n"));
    return 0;
  }
  c = sim800.read();

  while (1) {
    if (c == '+') {
      if (waitAvailable(serial) != SUCCESS) { goto RETURN_ERROR; }
      if ((c = sim800.read()) != 'C') { continue; }
      if (waitAvailable(serial) != SUCCESS) { goto RETURN_ERROR; }
      if ((c = sim800.read()) != 'M') { continue; }
      if (waitAvailable(serial) != SUCCESS) { goto RETURN_ERROR; }
      if ((c = sim800.read()) != 'G') { continue; }
      if (waitAvailable(serial) != SUCCESS) { goto RETURN_ERROR; }
      if ((c = sim800.read()) != 'L') { continue; }
      if (waitAvailable(serial) != SUCCESS) { goto RETURN_ERROR; }
      if ((c = sim800.read()) != ':') { continue; }

      int id = sim800.parseInt();

      if (waitAvailable(serial) != SUCCESS) { goto RETURN_ERROR; }
      if ((c = sim800.read()) != ',') { continue; }
      
      if (waitAvailable(serial) != SUCCESS) { goto RETURN_ERROR; }
      if ((c = sim800.read()) != '\"') { continue; }
      do {
        if (waitAvailable(serial) != SUCCESS) { goto RETURN_ERROR; }
        c = sim800.read();
      } while (c != '\"');
      if (waitAvailable(serial) != SUCCESS) { goto RETURN_ERROR; }
      if ((c = sim800.read()) != ',') { continue; }
      
      if (waitAvailable(serial) != SUCCESS) { goto RETURN_ERROR; }
      if ((c = sim800.read()) != '\"') { continue; }
      if (waitAvailable(serial) != SUCCESS) { goto RETURN_ERROR; }
      if ((c = sim800.read()) != '+') { continue; }
      char phone_number[20] = "";
      getPhoneNumber(phone_number);
      int p = 0;
      while (1) { // read phone number
        if (waitAvailable(serial) != SUCCESS) { goto RETURN_ERROR; }
        c = sim800.read();
        
        if (c == '\"') { break; }
        if (phone_number[p] != c) {
          goto SKIP_CMGL_CHECK;
        }
        ++p;
      } // c = "
      if (waitAvailable(serial) != SUCCESS) { goto RETURN_ERROR; }
      if ((c = sim800.read()) != ',') { continue; }

      if (waitAvailable(serial) != SUCCESS) { goto RETURN_ERROR; }
      if ((c = sim800.read()) != '\"') { continue; }
      do { // skip
        if (waitAvailable(serial) != SUCCESS) { goto RETURN_ERROR; }
        c = sim800.read();
      } while (c != '\"');
      if (waitAvailable(serial) != SUCCESS) { goto RETURN_ERROR; }
      if ((c = sim800.read()) != ',') { continue; }

      if (waitAvailable(serial) != SUCCESS) { goto RETURN_ERROR; }
      if ((c = sim800.read()) != '\"') { continue; }
      struct date_time current_date_time;
      if (getByteFromSerial(sim800, current_date_time.year, c) != SUCCESS) { continue; }
      if (c != '/') { continue; }
      if (getByteFromSerial(sim800, current_date_time.month, c) != SUCCESS) { continue; }
      if (c != '/') { continue; }
      if (getByteFromSerial(sim800, current_date_time.day, c) != SUCCESS) { continue; }
      if (c != ',') { continue; }
      if (getByteFromSerial(sim800, current_date_time.hour, c) != SUCCESS) { continue; }
      if (c != ':') { continue; }
      if (getByteFromSerial(sim800, current_date_time.minute, c) != SUCCESS) { continue; }
      if (c != ':') { continue; }
      if (getByteFromSerial(sim800, current_date_time.second, c) != SUCCESS) { continue; }

      if (current_date_time.year != max_date_time.year) {
        if (current_date_time.year > max_date_time.year) {
          max_id = id;
          max_date_time = current_date_time;
        }
        continue;
      }
      if (current_date_time.month != max_date_time.month) {
        if (current_date_time.month > max_date_time.month) {
          max_id = id;
          max_date_time = current_date_time;
        }
        continue;
      }
      if (current_date_time.day != max_date_time.day) {
        if (current_date_time.day > max_date_time.day) {
          max_id = id;
          max_date_time = current_date_time;
        }
        continue;
      }
      if (current_date_time.hour != max_date_time.hour) {
        if (current_date_time.hour > max_date_time.hour) {
          max_id = id;
          max_date_time = current_date_time;
        }
        continue;
      }
      if (current_date_time.minute != max_date_time.minute) {
        if (current_date_time.minute > max_date_time.minute) {
          max_id = id;
          max_date_time = current_date_time;
        }
        continue;
      }
      if (current_date_time.second > max_date_time.second) {
        max_id = id;
        max_date_time = current_date_time;
      }
      continue;
    }
    if (c == 'O') {
      if (waitAvailable(serial) != SUCCESS) { goto RETURN_ERROR; }
      if ((c = sim800.read()) != 'K') { continue; }
      if (waitAvailable(serial) != SUCCESS) { goto RETURN_ERROR; }
      if ((c = sim800.read()) != '\r') { continue; }
      if (waitAvailable(serial) != SUCCESS) { goto RETURN_ERROR; }
      if ((c = sim800.read()) != '\n') { continue; }
      if (waitAvailable(serial) == SUCCESS) { continue; }

      return max_id;
    }
SKIP_CMGL_CHECK:
    if (waitAvailable(serial) != SUCCESS) {
      goto RETURN_ERROR;
    }
    c = sim800.read();

  }

RETURN_ERROR:
  printDebug(F("getNewestSMSFromNumberUntilOK: unexpected end of data\n"))
  return 0;
}

return_code_t parsSMSUntilOk(struct ParsRequest& request, Stream& sim800_serial = sim800) {
  for (uint8_t line_number = 1; line_number < 2;) {
    if (waitAvailable(sim800_serial) != SUCCESS) {
      return ERROR;
    }
    char c = sim800_serial.read();
    if (c == '\n') {
      ++line_number;
    }
  }

  while (waitAvailable(sim800_serial) == SUCCESS) {
    parsRequestFrom(sim800, request);
  }

  return SUCCESS;
}

return_code_t parsSMS(int sms_number, struct ParsRequest& request, Stream& sim800_serial = sim800) {
  sim800_serial.print(F("AT+CMGR="));
  sim800_serial.println(sms_number);
  
  Serial.println(F("parsSMS:\n"));

  return parsSMSUntilOk(request, sim800_serial);
}













