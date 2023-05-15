#pragma once

#include "eeprom_commands.hpp"
#include "print_debug.hpp"
#include "return_codes.hpp"
#include "send_commands.hpp"
#include "str_macroses.hpp"

struct parsCondition {
  int special_symbol_detected;
};

return_code_t parsRequestFromSerial() {
  if (!Serial.available()) {
    return NO_REQUEST;
  }

  char word[128] = "";
  int pointer = 0;

  while (Serial.available()) {
    char symbol = Serial.read();
    if (isSpaceSymbol(symbol)) {
      word[pointer] = '\0';
      break;
    }
    word[pointer] = symbol;

    ++pointer;
    if (pointer >= 127) {
      word[127] = '\0';
      break;
    }
  }

  if (compareStrs(word, "CHECK_OK") == 0) {
    printDebug("parsRequestFromSerial: Sim800Check\n");
    return Sim800Check();
  }
  if (compareStrs(word, "CMGF_EN") == 0) {
    printDebug("parsRequestFromSerial: Sim800Config_CMGF\n");
    return Sim800Config_CMGF();
  }
  if (compareStrs(word, "SET_NUMBER") == 0) {
    printDebug("parsRequestFromSerial: setPhoneNumberFromSerial\n");
    return setPhoneNumberFromSerial();
  }
  if (compareStrs(word, "SET_MAX_TEMPERATURE") == 0) {
    printDebug("parsRequestFromSerial: setPhoneNumberFromSerial\n");
    return setMaxTemperatureFromSerial();
  }
  if (compareStrs(word, "SET_MIN_TEMPERATURE") == 0) {
    printDebug("parsRequestFromSerial: setPhoneNumberFromSerial\n");
    return setMinTemperatureFromSerial();
  }
  if (compareStrs(word, "SET_MAX_HUMIDITY") == 0) {
    printDebug("parsRequestFromSerial: setPhoneNumberFromSerial\n");
    return setMaxHumidityFromSerial();
  }
  if (compareStrs(word, "SET_MIN_HUMIDITY") == 0) {
    printDebug("parsRequestFromSerial: setPhoneNumberFromSerial\n");
    return setMinHumidityFromSerial();
  }
  if (compareStrs(word, "PRINT_STORED_DATA") == 0) {
    printDebug("parsRequestFromSerial: setPhoneNumberFromSerial\n");
    return printStoredDataToSerial();
  }
  if (compareStrs(word, "PRINT_SMS") == 0) {
    printDebug("parsRequestFromSerial: printSMSToSerial\n");
    return printSMSToSerial();
  }

  printError("parsRequestFromSerial: Command unrecognised.\n");
  return ERROR;
}

void parsInput(struct parsCondition& pars_condition, char symbol) {
  if (pars_condition.special_symbol_detected) {
    switch (symbol) {
    case 'r':
      sim800.write('\r');
      break;
    case 'n':
      sim800.write('\n');
      break;
    case 't':
      sim800.write('\t');
      break;
    case 'e':
      sim800.write((char)27); // SUB
      break;
    case 'z':
      sim800.write((char)26); // ESC
      break;
    case '\\':
      sim800.write('\\');
      break;
    default:
      sim800.write('_');
      break;
    }

    pars_condition.special_symbol_detected = false;
  } else {
    if (symbol == '\\') {
      pars_condition.special_symbol_detected = true;
    } else {
      sim800.write(symbol);
    }
  }
}