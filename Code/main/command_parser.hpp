#pragma once

#include "eeprom_commands.hpp"
#include "print_debug.hpp"
#include "return_codes.hpp"
#include "send_commands.hpp"
#include "str_macroses.hpp"

#define IF_GOTO(contition, if_goto) { \
  if (contition) {                    \
    goto if_goto;                     \
  }                                   \
}

//#define getSymbolIfAvailableAndNotSpace_BEGUG
#ifdef getSymbolIfAvailableAndNotSpace_BEGUG
char last_char;
#endif

struct parsCondition {
  int special_symbol_detected;
};

int getSymbolIfAvailable() {
  if (Serial.available()) {
#ifdef getSymbolIfAvailableAndNotSpace_BEGUG
    last_char = Serial.read();
    return last_char;
#else // getSymbolIfAvailableAndNotSpace_BEGUG
    return Serial.read();
#endif // getSymbolIfAvailableAndNotSpace_BEGUG
  }
  return -1;
}

int getSymbolIfAvailableAndNotSpace() {
getSymbolIfAvailableAndNotSpace_BEGIN:
  if (Serial.available()) {
    char c = Serial.read();
#ifdef getSymbolIfAvailableAndNotSpace_BEGUG
    last_char = c;
#endif // getSymbolIfAvailableAndNotSpace_BEGUG
    return isSpaceSymbol(c) ? -1 : c;
  }

  for (int i = 0; i < 1000; ++i) {
    delay(10);
    if (Serial.available()) {
      goto getSymbolIfAvailableAndNotSpace_BEGIN;
    }
  }
  return -1;
}

return_code_t parsRequestFromSerial() {
  /*
  commands list:
  CHECK_OK
  CMGF_EN
  SET_NUMBER
  SET_MAX_TEMPERATURE
  SET_MAX_HUMIDITY
  SET_MIN_TEMPERATURE
  SET_MIN_TEMPERATURE
  PRINT_SMS
  PRINT_STORED_DATA
  */

  if (!Serial.available()) {
    return NO_REQUEST;
  }
  switch (getSymbolIfAvailableAndNotSpace()) {
  case 'C':
    switch (getSymbolIfAvailableAndNotSpace()) {
    case 'H':
      IF_GOTO(getSymbolIfAvailableAndNotSpace() != 'E', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace() != 'C', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace() != 'K', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace() != '_', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace() != 'O', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace() != 'K', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace() != -1,  RETURN_ERROR);

      printDebug("parsRequestFromSerial: Sim800Check\n");
      return Sim800Check();
    case 'M':
      IF_GOTO(getSymbolIfAvailableAndNotSpace() != 'G', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace() != 'F', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace() != '_', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace() != 'E', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace() != 'N', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace() != -1,  RETURN_ERROR);
      
      printDebug("parsRequestFromSerial: Sim800Config_CMGF\n");
      return Sim800Config_CMGF();
    default: goto RETURN_ERROR; 
    }
  case 'S':
    IF_GOTO(getSymbolIfAvailableAndNotSpace() != 'E', RETURN_ERROR);
    IF_GOTO(getSymbolIfAvailableAndNotSpace() != 'T', RETURN_ERROR);
    IF_GOTO(getSymbolIfAvailableAndNotSpace() != '_', RETURN_ERROR);
    
    switch (getSymbolIfAvailableAndNotSpace()) {
    case 'M':   
      switch (getSymbolIfAvailableAndNotSpace()) {
      case 'I':
        IF_GOTO(getSymbolIfAvailableAndNotSpace() != 'N', RETURN_ERROR);
        IF_GOTO(getSymbolIfAvailableAndNotSpace() != '_', RETURN_ERROR);
        
        switch (getSymbolIfAvailableAndNotSpace()) {
        case 'H':
          IF_GOTO(getSymbolIfAvailableAndNotSpace() != 'U', RETURN_ERROR);
          IF_GOTO(getSymbolIfAvailableAndNotSpace() != 'M', RETURN_ERROR);
          IF_GOTO(getSymbolIfAvailableAndNotSpace() != 'I', RETURN_ERROR);
          IF_GOTO(getSymbolIfAvailableAndNotSpace() != 'D', RETURN_ERROR);
          IF_GOTO(getSymbolIfAvailableAndNotSpace() != 'I', RETURN_ERROR);
          IF_GOTO(getSymbolIfAvailableAndNotSpace() != 'T', RETURN_ERROR);
          IF_GOTO(getSymbolIfAvailableAndNotSpace() != 'Y', RETURN_ERROR);
          IF_GOTO(getSymbolIfAvailableAndNotSpace() != -1,  RETURN_ERROR);

          printDebug("parsRequestFromSerial: setMinHumidityFromSerial\n");
          return setMinHumidityFromSerial();
        case 'T':
          IF_GOTO(getSymbolIfAvailableAndNotSpace() != 'E', RETURN_ERROR);
          IF_GOTO(getSymbolIfAvailableAndNotSpace() != 'M', RETURN_ERROR);
          IF_GOTO(getSymbolIfAvailableAndNotSpace() != 'P', RETURN_ERROR);
          IF_GOTO(getSymbolIfAvailableAndNotSpace() != 'E', RETURN_ERROR);
          IF_GOTO(getSymbolIfAvailableAndNotSpace() != 'R', RETURN_ERROR);
          IF_GOTO(getSymbolIfAvailableAndNotSpace() != 'A', RETURN_ERROR);
          IF_GOTO(getSymbolIfAvailableAndNotSpace() != 'T', RETURN_ERROR);
          IF_GOTO(getSymbolIfAvailableAndNotSpace() != 'U', RETURN_ERROR);
          IF_GOTO(getSymbolIfAvailableAndNotSpace() != 'R', RETURN_ERROR);
          IF_GOTO(getSymbolIfAvailableAndNotSpace() != 'E', RETURN_ERROR);
          IF_GOTO(getSymbolIfAvailableAndNotSpace() != -1,  RETURN_ERROR);

          printDebug("parsRequestFromSerial: setMinTemperatureFromSerial\n");
          return setMinTemperatureFromSerial();
        default: goto RETURN_ERROR;
        }
      case 'A':
        IF_GOTO(getSymbolIfAvailableAndNotSpace() != 'X', RETURN_ERROR);
        IF_GOTO(getSymbolIfAvailableAndNotSpace() != '_', RETURN_ERROR);
        
        switch (getSymbolIfAvailableAndNotSpace()) {
        case 'H':
          IF_GOTO(getSymbolIfAvailableAndNotSpace() != 'U', RETURN_ERROR);
          IF_GOTO(getSymbolIfAvailableAndNotSpace() != 'M', RETURN_ERROR);
          IF_GOTO(getSymbolIfAvailableAndNotSpace() != 'I', RETURN_ERROR);
          IF_GOTO(getSymbolIfAvailableAndNotSpace() != 'D', RETURN_ERROR);
          IF_GOTO(getSymbolIfAvailableAndNotSpace() != 'I', RETURN_ERROR);
          IF_GOTO(getSymbolIfAvailableAndNotSpace() != 'T', RETURN_ERROR);
          IF_GOTO(getSymbolIfAvailableAndNotSpace() != 'Y', RETURN_ERROR);
          IF_GOTO(getSymbolIfAvailableAndNotSpace() != -1,  RETURN_ERROR);

          printDebug("parsRequestFromSerial: setMaxHumidityFromSerial\n");
          return setMaxHumidityFromSerial();
        case 'T':
          IF_GOTO(getSymbolIfAvailableAndNotSpace() != 'E', RETURN_ERROR);
          IF_GOTO(getSymbolIfAvailableAndNotSpace() != 'M', RETURN_ERROR);
          IF_GOTO(getSymbolIfAvailableAndNotSpace() != 'P', RETURN_ERROR);
          IF_GOTO(getSymbolIfAvailableAndNotSpace() != 'E', RETURN_ERROR);
          IF_GOTO(getSymbolIfAvailableAndNotSpace() != 'R', RETURN_ERROR);
          IF_GOTO(getSymbolIfAvailableAndNotSpace() != 'A', RETURN_ERROR);
          IF_GOTO(getSymbolIfAvailableAndNotSpace() != 'T', RETURN_ERROR);
          IF_GOTO(getSymbolIfAvailableAndNotSpace() != 'U', RETURN_ERROR);
          IF_GOTO(getSymbolIfAvailableAndNotSpace() != 'R', RETURN_ERROR);
          IF_GOTO(getSymbolIfAvailableAndNotSpace() != 'E', RETURN_ERROR);
          IF_GOTO(getSymbolIfAvailableAndNotSpace() != -1,  RETURN_ERROR);

          printDebug("parsRequestFromSerial: setMaxTemperatureFromSerial\n");
          return setMaxTemperatureFromSerial();
        default: goto RETURN_ERROR;
        }
      default: goto RETURN_ERROR;
      }
    case 'N':
      IF_GOTO(getSymbolIfAvailableAndNotSpace() != 'U', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace() != 'M', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace() != 'B', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace() != 'E', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace() != 'R', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace() != -1,  RETURN_ERROR);

      printDebug("parsRequestFromSerial: setPhoneNumberFromSerial\n");
      return setPhoneNumberFromSerial();
    default: goto RETURN_ERROR;
    }
  case 'P':
    IF_GOTO(getSymbolIfAvailableAndNotSpace() != 'R', RETURN_ERROR);
    IF_GOTO(getSymbolIfAvailableAndNotSpace() != 'I', RETURN_ERROR);
    IF_GOTO(getSymbolIfAvailableAndNotSpace() != 'N', RETURN_ERROR);
    IF_GOTO(getSymbolIfAvailableAndNotSpace() != 'T', RETURN_ERROR);
    IF_GOTO(getSymbolIfAvailableAndNotSpace() != '_', RETURN_ERROR);
    IF_GOTO(getSymbolIfAvailableAndNotSpace() != 'S', RETURN_ERROR);

    switch (getSymbolIfAvailableAndNotSpace()) {
    case 'M':
      IF_GOTO(getSymbolIfAvailableAndNotSpace() != 'S', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace() != -1,  RETURN_ERROR);

      printDebug("parsRequestFromSerial: printSMSToSerial\n");
      return printSMSToSerial();
    case 'T':
      IF_GOTO(getSymbolIfAvailableAndNotSpace() != 'O', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace() != 'R', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace() != 'E', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace() != 'D', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace() != '_', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace() != 'D', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace() != 'A', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace() != 'T', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace() != 'A', RETURN_ERROR);
      IF_GOTO(getSymbolIfAvailableAndNotSpace() != -1,  RETURN_ERROR);
      
      printDebug("parsRequestFromSerial: setPhoneNumberFromSerial\n");
      return printStoredDataToSerial();
    default:  goto RETURN_ERROR;
    }
  case '?':
    IF_GOTO(getSymbolIfAvailableAndNotSpace() != -1, RETURN_ERROR);

    printDebug("parsRequestFromSerial: printAreaparametersToSerial\n");
    return printAreaparametersToSerial();
  default: goto RETURN_ERROR;
  }
  
RETURN_ERROR:
  printError("parsRequestFromSerial: Command unrecognised.\n");
#ifdef getSymbolIfAvailableAndNotSpace_BEGUG
  printDebug("parsRequestFromSerial: last char: \'");
  printDebugInLine(last_char);
  printDebugInLine("\'\n");
#endif // getSymbolIfAvailableAndNotSpace_BEGUG
  return ERROR;

}






















