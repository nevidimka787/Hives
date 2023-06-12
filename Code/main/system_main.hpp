#pragma once

#include "blink.hpp"
#include "eeprom_commands.hpp"
#include "events_manager.hpp"
#include "message_generator.hpp"
#include "return_codes.hpp"
#include "system_info_struct.h"

using namespace return_codes;

// if value grater then max or less then min, then change status.
// @param value - input value
// @param max - high edge
// @param min - low edge
// @param current_status - status of this value (can be changed)
void checkMaxMin(float value, float max, float min, system_warning_status_t& current_status);

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

void checkMaxMin(float value, float max, float min, system_warning_status_t& current_status) {
  if (value > max || value < min) { // if max or min is NAN then egde isn't checked
    if (current_status == WAIT_NORMALISATION) {
      return;
    }
    current_status = SEND;
    return;
  }
   
  if (current_status == WAIT_NORMALISATION) {
    printDebug(F("checkMaxMin: value: "));
    printDebugInLine(value);
    printDebugInLine(F(" in range: ( "));
    printDebugInLine(max);
    printDebugInLine(F(" : "));
    printDebugInLine(min);
    printDebugInLine(F(" ) return to normal\n"));

    current_status = NORMAL;
  }
  return;
}

return_code_t systemMainAction(struct system_info& global_system_info) {
  LEDOn();

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

  struct StoredData stored_data = getStoredData();

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  float weight = 0.0f/0.0f;
  if (scale.wait_ready_timeout(1000)) {
    scale.set_offset(stored_data.weight_offset);
    scale.set_scale(stored_data.weight_scale);
    weight = scale.get_units(10);
  }

  checkMaxMin(humidity, stored_data.max_humidity, stored_data.min_humidity, global_system_info.humidity_warning);
  checkMaxMin(temperature, stored_data.max_temperature, stored_data.min_temperature, global_system_info.temperature_warning);
  checkMaxMin(weight, stored_data.max_weight, stored_data.min_weight, global_system_info.weight_warning);
  
  if (global_system_info.humidity_warning == SEND || global_system_info.temperature_warning == SEND || global_system_info.weight_warning == SEND) {
    if (global_system_info.humidity_warning == SEND) {
      global_system_info.humidity_warning = WAIT_NORMALISATION;
    }
    if (global_system_info.temperature_warning == SEND) {
      global_system_info.temperature_warning = WAIT_NORMALISATION;
    }
    if (global_system_info.weight_warning == SEND) {
      global_system_info.weight_warning = WAIT_NORMALISATION;
    }
#ifdef SERIAL_DEBUG
    if (global_system_info.humidity_warning == WAIT_NORMALISATION) {
      Serial.print(F("WARNING: Humidity out of range!\n"));
    }
    if (global_system_info.temperature_warning == WAIT_NORMALISATION) {
      Serial.print(F("WARNING: Temperature out of range!\n"));
    }
    if (global_system_info.weight_warning == WAIT_NORMALISATION) {
      Serial.print(F("WARNING: Weight out of range!\n"));
    }
    
    shortPrintMeasuredDataTo(Serial);
    shortPrintStoredDataTo(Serial);
#endif // SERIAL_DEBUG

    // return result;

    char phone_number[20];
    getPhoneNumber(phone_number);
    sim800.print(F("AT+CMGS=\"+"));
    sim800.print(phone_number);
    sim800.println(F("\""));
    waitAvailable(sim800);

    if (global_system_info.humidity_warning == WAIT_NORMALISATION) {
      sim800.print(F("WARNING: Humidity out of range!\n"));
    }
    if (global_system_info.temperature_warning == WAIT_NORMALISATION) {
      sim800.print(F("WARNING: Temperature out of range!\n"));
    }
    if (global_system_info.weight_warning == WAIT_NORMALISATION) {
      sim800.print(F("WARNING: Weight out of range!\n"));
    }

    shortPrintMeasuredDataTo(sim800);
    shortPrintStoredDataTo(sim800);

    sim800.println((char)26);

    for(char attemp = 0; attemp < 10; ++attemp) {
      if (checkSim800OK(1000) == SUCCESS) {
        return SUCCESS;
      }
    }
    return ERROR;
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
