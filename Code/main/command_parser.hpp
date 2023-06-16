#pragma once

#include <SoftwareSerial.h>

#include "commands_list.hpp"
#include "pars_request_struct.hpp"
#include "print_debug.hpp"
#include "return_codes.hpp"
#include "serial_helper.hpp"
#include "str_macroses.hpp"

using namespace return_codes;

#define IF_GOTO(contition, if_goto) { \
  if (contition) {                    \
    goto if_goto;                     \
  }                                   \
}

//#define getSymbolIfAvailableAndNotSpace_BEGUG
#ifdef getSymbolIfAvailableAndNotSpace_BEGUG
char last_char;
#endif

// if input available return symblo else return -1
// @param serial - input
// @return symbol if one available else -1
int getSymbolIfAvailable(Stream& serial);

// if input symbol available and if one not a space symbol return symblo else return -1
// @param serial - input
// @return symbol if one available and if one not a space symbol else -1
int getSymbolIfAvailableAndNotSpace(Stream& serial);

// pars a command and its parameters from serial and write it to request
// commands list:
//
// DELETE_SMS
// CHECK_OK
// CMGF_EN
// GET_LAST_SMS_ID
// SEND_SMS
// SET_NUMBER
// SET_MAX_TEMPERATURE
// SET_MAX_HUMIDITY
// SET_MAX_WEIGHT
// SET_MIN_TEMPERATURE
// SET_MIN_TEMPERATURE
// SET_MIN_WEIGHT
// SET_SEND_TIME
// SET_WEIGHT_OFFSET
// SET_WEIGHT_SCALE
// PRINT_SMS
// PRINT_SMS_ALL
// PRINT_STORED_DATA
// UPDATE_DATE_TIME
// i, I -- PRINT_MEASURED_DATA
// # -- DEBUG_COMM
//
// @param serial - input stream
// @param requet - commands will be writed to here
return_code_t parsRequestFrom(Stream& serial, struct ParsRequest& request);

int getSymbolIfAvailable(Stream& serial) {
  if (serial.available()) {
#ifdef getSymbolIfAvailableAndNotSpace_BEGUG
    last_char = serial.read();
    return last_char;
#else // getSymbolIfAvailableAndNotSpace_BEGUG
    return serial.read();
#endif // getSymbolIfAvailableAndNotSpace_BEGUG
  }
  return -1;
}

int getSymbolIfAvailableAndNotSpace(Stream& serial) {
getSymbolIfAvailableAndNotSpace_BEGIN:
  if (serial.available()) {
    char c = serial.read();
#ifdef getSymbolIfAvailableAndNotSpace_BEGUG
    last_char = c;
#endif // getSymbolIfAvailableAndNotSpace_BEGUG
    return isSpaceSymbol(c) ? -1 : c;
  }

  for (int i = 0; i < 1000; ++i) {
    delay(10);
    if (serial.available()) {
      goto getSymbolIfAvailableAndNotSpace_BEGIN;
    }
  }
  return -1;
}

return_code_t parsRequestFrom(Stream& serial, struct ParsRequest& request) {
  /*
  commands list:

   DELETE_SMS
   CHECK_OK
   CMGF_EN
   GET_LAST_SMS_ID
   SEND_SMS
   SET_NUMBER
   SET_MAX_TEMPERATURE
   SET_MAX_HUMIDITY
   SET_MAX_WEIGHT
   SET_MIN_TEMPERATURE
   SET_MIN_TEMPERATURE
   SET_MIN_WEIGHT
   SET_SEND_TIME
   SET_WEIGHT_OFFSET
   SET_WEIGHT_SCALE
   PRINT_SMS
   PRINT_SMS_ALL
   PRINT_STORED_DATA
   UPDATE_DATE_TIME
   i, I -- PRINT_MEASURED_DATA
   # -- DEBUG_COMM

  i, I -- PRINT_MEASURED_DATA
  # -- DEBUG_COMM
  */

  if (!serial.available()) {
    return NO_REQUEST;
  }

  // Serch by tree.
  switch (getSymbolIfAvailableAndNotSpace(serial)) {
  case 'D':
      IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'E', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'L', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'E', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'T', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'E', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != '_', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'S', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'M', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'S', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != '_', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'A', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'L', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'L', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != -1,  RETURN_ERROR);

      request.commands_list |= DELETE_SMS_ALL;

      return SUCCESS;
  case 'C':
    switch (getSymbolIfAvailableAndNotSpace(serial)) {
    case 'A':
      IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'L', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'C', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'U', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'L', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'A', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'T', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'E', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != '_', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'S', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'C', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'A', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'L', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'E', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != -1,  RETURN_ERROR);

      request.commands_list |= CALCULATE_SCALE;
      return scanFloatFromSerial(serial, request.target_weight);
    case 'H':
      IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'E', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'C', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'K', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != '_', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'O', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'K', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != -1,  RETURN_ERROR);

      request.commands_list |= CHECK_OK;
      return SUCCESS;
    case 'O':
      IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'N', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'F', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'I', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'G', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'U', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'R', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'A', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'T', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'E', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != -1,  RETURN_ERROR);
      
      request.commands_list |= CMGF_EN;
      return SUCCESS;
    default: goto RETURN_ERROR; 
    }
  case 'G':
    IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'E', RETURN_ERROR);
    IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'T', RETURN_ERROR);
    IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != '_', RETURN_ERROR);

    switch(getSymbolIfAvailableAndNotSpace(serial)) {
    case 'L':
      IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'A', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'S', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'T', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != '_', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'S', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'M', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'S', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != '_', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'I', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'D', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != -1,  RETURN_ERROR);

      request.commands_list |= GET_LAST_SMS_ID;
      return SUCCESS;
    case 'T':
      IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'I', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'M', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'E', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != -1,  RETURN_ERROR);

      request.commands_list |= GET_TIME;
      return SUCCESS;
    default: goto RETURN_ERROR;
    }
  case 'I':
  case 'i':
    IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != -1, RETURN_ERROR);

    request.commands_list |= PRINT_MEASURED_DATA;
    return SUCCESS;
  case 'S':
    IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'E', RETURN_ERROR);
    switch (getSymbolIfAvailableAndNotSpace(serial)) {
    case 'N':
      IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'D', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != '_', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'S', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'M', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'S', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != -1,  RETURN_ERROR);

      request.commands_list |= SEND_SMS;
      return SUCCESS;
    case 'T':
      IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != '_', RETURN_ERROR);
      
      switch (getSymbolIfAvailableAndNotSpace(serial)) {
      case 'M':   
        switch (getSymbolIfAvailableAndNotSpace(serial)) {
        case 'I':
          IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'N', RETURN_ERROR);
          IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != '_', RETURN_ERROR);
          
          switch (getSymbolIfAvailableAndNotSpace(serial)) {
          case 'H':
            IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'U', RETURN_ERROR);
            IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'M', RETURN_ERROR);
            IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'I', RETURN_ERROR);
            IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'D', RETURN_ERROR);
            IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'I', RETURN_ERROR);
            IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'T', RETURN_ERROR);
            IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'Y', RETURN_ERROR);
            IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != -1,  RETURN_ERROR);

            request.commands_list |= SET_MIN_HUMIDITY;
            return scanFloatFromSerial(serial, request.min_humidity);
          case 'T':
            IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'E', RETURN_ERROR);
            IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'M', RETURN_ERROR);
            IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'P', RETURN_ERROR);
            IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'E', RETURN_ERROR);
            IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'R', RETURN_ERROR);
            IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'A', RETURN_ERROR);
            IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'T', RETURN_ERROR);
            IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'U', RETURN_ERROR);
            IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'R', RETURN_ERROR);
            IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'E', RETURN_ERROR);
            IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != -1,  RETURN_ERROR);

            request.commands_list |= SET_MIN_TEMPERATURE;
            return scanFloatFromSerial(serial, request.min_temperature);
          case 'W':
            IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'E', RETURN_ERROR);
            IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'I', RETURN_ERROR);
            IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'G', RETURN_ERROR);
            IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'H', RETURN_ERROR);
            IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'T', RETURN_ERROR);
            IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != -1,  RETURN_ERROR);
            
            request.commands_list |= SET_MIN_WEIGHT;
            return scanFloatFromSerial(serial, request.min_weight);
          default: goto RETURN_ERROR;
          }
        case 'A':
          IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'X', RETURN_ERROR);
          IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != '_', RETURN_ERROR);
          
          switch (getSymbolIfAvailableAndNotSpace(serial)) {
          case 'H':
            IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'U', RETURN_ERROR);
            IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'M', RETURN_ERROR);
            IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'I', RETURN_ERROR);
            IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'D', RETURN_ERROR);
            IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'I', RETURN_ERROR);
            IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'T', RETURN_ERROR);
            IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'Y', RETURN_ERROR);
            IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != -1,  RETURN_ERROR);

            request.commands_list |= SET_MAX_HUMIDITY;
            return scanFloatFromSerial(serial, request.max_humidity);
          case 'T':
            IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'E', RETURN_ERROR);
            IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'M', RETURN_ERROR);
            IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'P', RETURN_ERROR);
            IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'E', RETURN_ERROR);
            IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'R', RETURN_ERROR);
            IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'A', RETURN_ERROR);
            IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'T', RETURN_ERROR);
            IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'U', RETURN_ERROR);
            IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'R', RETURN_ERROR);
            IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'E', RETURN_ERROR);
            IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != -1,  RETURN_ERROR);

            request.commands_list |= SET_MAX_TEMPERATURE;
            return scanFloatFromSerial(serial, request.max_temperature);
          case 'W':
            IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'E', RETURN_ERROR);
            IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'I', RETURN_ERROR);
            IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'G', RETURN_ERROR);
            IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'H', RETURN_ERROR);
            IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'T', RETURN_ERROR);
            IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != -1,  RETURN_ERROR);
            
            request.commands_list |= SET_MAX_WEIGHT;
            return scanFloatFromSerial(serial, request.max_weight);
          default: goto RETURN_ERROR;
          }
        default: goto RETURN_ERROR;
        }
      case 'N':
        IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'U', RETURN_ERROR);
        IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'M', RETURN_ERROR);
        IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'B', RETURN_ERROR);
        IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'E', RETURN_ERROR);
        IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'R', RETURN_ERROR);
        IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != -1,  RETURN_ERROR);

        request.commands_list |= SET_NUMBER;
        return scanPhoneNumber(serial, request.phone_number);
      case 'S':
        IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'E', RETURN_ERROR);
        IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'N', RETURN_ERROR);
        IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'D', RETURN_ERROR);
        IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != '_', RETURN_ERROR);
        IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'T', RETURN_ERROR);
        IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'I', RETURN_ERROR);
        IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'M', RETURN_ERROR);
        IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'E', RETURN_ERROR);
        IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != -1,  RETURN_ERROR);
        
        request.commands_list |= SET_SEND_TIME;
        return scanTime(request.date_time, serial);
      case 'W':
        IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'E', RETURN_ERROR);
        IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'I', RETURN_ERROR);
        IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'G', RETURN_ERROR);
        IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'H', RETURN_ERROR);
        IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'T', RETURN_ERROR);
        IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != '_', RETURN_ERROR);

        switch (getSymbolIfAvailableAndNotSpace(serial)) {
        case 'O':
          IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'F', RETURN_ERROR);
          IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'F', RETURN_ERROR);
          IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'S', RETURN_ERROR);
          IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'E', RETURN_ERROR);
          IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'T', RETURN_ERROR);
          IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != -1, RETURN_ERROR);

          request.commands_list |= SET_WEIGHT_OFFSET;
          return scanFloatFromSerial(serial, request.weight_offset);
        case 'S':
          IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'C', RETURN_ERROR);
          IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'A', RETURN_ERROR);
          IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'L', RETURN_ERROR);
          IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'E', RETURN_ERROR);
          IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != -1, RETURN_ERROR);

          request.commands_list |= SET_WEIGHT_SCALE;
          return scanFloatFromSerial(serial, request.weight_scale);
        default: goto RETURN_ERROR;
        }

      default: goto RETURN_ERROR;
      }
    default: goto RETURN_ERROR;
    }
  case 'P':
    IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'R', RETURN_ERROR);
    IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'I', RETURN_ERROR);
    IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'N', RETURN_ERROR);
    IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'T', RETURN_ERROR);
    IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != '_', RETURN_ERROR);
    IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'S', RETURN_ERROR);

    switch (getSymbolIfAvailableAndNotSpace(serial)) {
    case 'M':
      IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'S', RETURN_ERROR);

      switch (getSymbolIfAvailableAndNotSpace(serial)) {
      case '_':
        IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'A', RETURN_ERROR);
        IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'L', RETURN_ERROR);
        IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'L', RETURN_ERROR);
        IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != -1,  RETURN_ERROR);
       
        request.commands_list |= PRINT_SMS_ALL;
        return SUCCESS;
      case -1:
        request.commands_list |= PRINT_SMS;
        request.sms_number = serial.parseInt();
        return SUCCESS;
      default: goto RETURN_ERROR;
      }

    case 'T':
      IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'O', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'R', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'E', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'D', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != '_', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'D', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'A', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'T', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'A', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != -1,  RETURN_ERROR);
      
      request.commands_list |= PRINT_STORED_DATA;
      return SUCCESS;
    default: goto RETURN_ERROR;
    }
  case 'T':
    IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'A', RETURN_ERROR);
    IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'R', RETURN_ERROR);
    IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'E', RETURN_ERROR);
    IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != -1,  RETURN_ERROR);
      
    request.commands_list |= TARE;
    return SUCCESS;
  case 'U':
    IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'P', RETURN_ERROR);
    IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'D', RETURN_ERROR);
    IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'A', RETURN_ERROR);
    IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'T', RETURN_ERROR);
    IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'E', RETURN_ERROR);
    IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != '_', RETURN_ERROR);
    IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'D', RETURN_ERROR);
    IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'A', RETURN_ERROR);
    IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'T', RETURN_ERROR);
    IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'E', RETURN_ERROR);
    IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != '_', RETURN_ERROR);
    IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'T', RETURN_ERROR);
    IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'I', RETURN_ERROR);
    IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'M', RETURN_ERROR);
    IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != 'E', RETURN_ERROR);
    IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != -1,  RETURN_ERROR);

    request.commands_list |= UPDATE_DATE_TIME;
    return SUCCESS;
  case '#':
    IF_GOTO(getSymbolIfAvailableAndNotSpace(serial) != -1, RETURN_ERROR);

    request.commands_list |= DEBUG_COMM;
    return SUCCESS;
  default: goto RETURN_ERROR;
  }
  
RETURN_ERROR:
#ifdef getSymbolIfAvailableAndNotSpace_BEGUG
  printDebug(F("parsRequestFrom: last char: \'"));
  printDebugInLine(last_char);
  printDebugInLine("\'\n");
#endif // getSymbolIfAvailableAndNotSpace_BEGUG
  return ERROR;

}






















