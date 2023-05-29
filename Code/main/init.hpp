#pragma once

#include <SoftwareSerial.h>

#include "command_parser.hpp"
#include "eeprom_commands.hpp"
#include "print_debug.hpp"
#include "return_codes.hpp"
#include "send_commands.hpp"

/*
@param connecting_attemps_count count of requests that will be sended to sim800 before timeout
*/
return_code_t init(
  int connecting_attemps_count
) {
  if (connecting_attemps_count > 0) {
    for (; connecting_attemps_count > 0; --connecting_attemps_count) {
      if (Sim800Check(1000) == SUCCESS) {
        break;
      }
    }
    if (connecting_attemps_count <= 0) {
      printError(F("init: SIM800 connecting timeout\n"));
      return ERROR;
    }
  } else {
    while (Sim800Check(1000) != SUCCESS) {}
  } 
  printDebug(F("init: SIM800 connecting success\n"));

  return Sim800Config();
}