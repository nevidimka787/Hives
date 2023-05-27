#pragma once

#include "message_generator.hpp"
#include "return_codes.hpp"

typedef enum {
  NORMAL = 0,
  HAPPEN,
  SEND,
} system_warning_status_t;

struct system_info {
  system_warning_status_t humidity_warning;
  system_warning_status_t temperature_warning;
  system_warning_status_t weight_warning;
  system_warning_status_t power_warning;
};

using namespace return_codes;

return_code_t system_main_action(struct system_info& global_system_info) {
  int last_sms_id = 0;
  return_code_t result = SUCCESS;
  
  result = getLastSMSId(last_sms_id);
  if (last_sms_id == 0) {
    printDebug(F("system_main_action: NO_REQUEST\n"));
    return NO_REQUEST;
  }

  struct ParsRequest request = {0};

  if (parsSMS(last_sms_id, request) != SUCCESS) {
    printError(F("system_main_action: parsSMS error\n"));
    return ERROR;
  }

  result = doRequestAsSIM800(request);
  if (result == ERROR) {
    printError(F("system_main_action: doRequestAsSIM800 error\n"));
    return ERROR;
  }

  deleteSMSAll();

  printDebug(F("system_main_action: SUCCESS\n"));

  return SUCCESS;
}

return_code_t system_fix_action() {
  return Sim800Config();
}