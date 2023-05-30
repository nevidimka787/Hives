#pragma once

#include "events_manager.hpp"
#include "message_generator.hpp"
#include "return_codes.hpp"
#include "system_info_struct.h"

using namespace return_codes;

// get last SMS from sim800
// execute commands from SMS as user
// @param result_system_info - system information
// @return SUCCESS if some comands was executed, NO_REQUEST if nothing was executed, ERROR if parsing or exeqution failure
return_code_t systemDoSMS(struct system_info& result_system_info);

// try to fix system errors
// reconfigure sim800
// @return SUCCESS if fix was successfully
return_code_t systemFixAction();

// execute SMS if available
// update send time if available
// @param global_system_info - system information
// @return ERROR if SMS execution or time update failure, else SUCCESS
return_code_t systemMainAction(struct system_info& global_system_info);

// update time of send SMS
// @param result_system_info - system information
// @return always SUCCESS
return_code_t systemUpdateSendTime(struct system_info& result_system_info);

return_code_t systemMainAction(struct system_info& global_system_info) {
  return_code_t result = SUCCESS;
  
  result = systemDoSMS(global_system_info);
  if (result == ERROR) {
    return ERROR;
  }

  if (global_system_info.set_send_time) {
    printDebug(F("system_main_action: systemUpdateSendTime\n"));
    result = systemUpdateSendTime(global_system_info);
    global_system_info.set_send_time = false;
  }  

  return result;
}

return_code_t systemFixAction() {
  return Sim800Config();
}

return_code_t systemUpdateSendTime(struct system_info& result_system_info) {
  getSendTime(result_system_info.send_measured_data_time);
  result_system_info.send_measured_data_time.year = global_date_time.year;
  result_system_info.send_measured_data_time.month = global_date_time.month;
  result_system_info.send_measured_data_time.day = global_date_time.day;
  result_system_info.send_measured_data = result_system_info.send_measured_data_time.hour < 24;
  printDebug("systemUpdateSendTime: result_system_info.send_measured_data: ");
  if (result_system_info.send_measured_data) {
    printDebugInLine("true\n");
  } else {
    printDebugInLine("false\n");
  }

  while (eventAvailable(result_system_info.send_measured_data_time) == SUCCESS) {
    setNextDay(result_system_info.send_measured_data_time);
  }

  return SUCCESS;
}

return_code_t systemDoSMS(struct system_info& result_system_info) {
  int last_sms_id = 0;
  return_code_t result = SUCCESS;
  
  result = getLastSMSId(last_sms_id);
  if (last_sms_id == 0) {
    return NO_REQUEST;
  }
  
  struct ParsRequest request = {0};

  if (parsSMS(last_sms_id, request) != SUCCESS) {
    printError(F("system_main_action: parsSMS error\n"));
    return ERROR;
  }

  result = doRequestAsSIM800(request, result_system_info);
  if (result == ERROR) {
    printError(F("system_main_action: doRequestAsSIM800 error\n"));
    return ERROR;
  }

  deleteSMSAll();

  return SUCCESS;
}




















