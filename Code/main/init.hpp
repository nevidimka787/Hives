#pragma once

#include <SoftwareSerial.h>

#include "command_parser.hpp"
#include "eeprom_commands.hpp"
#include "print_debug.hpp"
#include "return_codes.hpp"
#include "send_commands.hpp"

// @param connecting_attemps_count - count of requests that will be sended to sim800 before timeout. Timeout = connecting_attemps_count * 10 secconds
// @return SUCCESS if sim800 sonnected successfully, else return ERROR 
return_code_t initSim800();

return_code_t initSim800(int connecting_attemps_count) {
  int attemp = connecting_attemps_count;
  if (attemp > 0) {
    for (; attemp > 0; --attemp) {
      if (Sim800Check(1000) == SUCCESS) {
        break;
      }
    }
    if (attemp <= 0) {
      printError(F("init: SIM800 connecting timeout\n"));
      return ERROR;
    }
  } else {
    while (Sim800Check(1000) != SUCCESS) {}
  } 
  printDebug(F("init: SIM800 connecting success\n"));

  attemp = connecting_attemps_count;
  while (attemp > 0) {
    --attemp;
    if (Sim800Config(1000) == SUCCESS) {
      printDebug(F("init: SIM800 configure success\n"));
      return SUCCESS;
    }
  }

  printError(F("init: SIM800 configure timeout\n"));
  return ERROR;
}













