#pragma once

enum COMMANDS {
  EMPTY_COMMAND       = 0,        // nothing is executed

  CALCULATE_SCALE     = 1L << 0,  // calculate scale of scales depends on setted weight
  CHECK_OK            = 1L << 1,  // send AT to sim800 and waiy OK (only Serial)
  CMGF_EN             = 1L << 2,  // config sim800 (only Serial)
  DELETE_SMS_ALL      = 1L << 3,  // delete all sms in connected sim800 module (only Serial)
  GET_LAST_SMS_ID     = 1L << 4,  // get id of the last sms sended by setted phone number in sim800 module (only Serial)
  GET_TIME            = 1L << 5,  // get current time in seconds (only Serial)
  SEND_SMS            = 1L << 6,  // send sms to setted phone number (only Serial)
  SET_NUMBER          = 1L << 7,  // set phone number (only Serial)
  SET_MAX_TEMPERATURE = 1L << 8,  // set max temperature
  SET_MAX_HUMIDITY    = 1L << 9,  // set min temperature
  SET_MAX_WEIGHT      = 1L << 10,  // set max weight
  SET_MIN_TEMPERATURE = 1L << 11, // set max numidity
  SET_MIN_HUMIDITY    = 1L << 12, // set min numidity
  SET_MIN_WEIGHT      = 1L << 13, // set min weight
  SET_WEIGHT_OFFSET   = 1L << 14, // set ofset of weight
  SET_WEIGHT_SCALE    = 1L << 15, // set scale of weight
  SET_SEND_TIME       = 1L << 16, // set time of sending SMS that contain enviromental data
  PRINT_SMS           = 1L << 17, // print selected SMS to serial (only Serial)
  PRINT_SMS_ALL       = 1L << 18, // print all SMS to serial (only Serial)
  PRINT_STORED_DATA   = 1L << 19, // print data that stored in eeprom (differense for Serial and sim800)
  PRINT_MEASURED_DATA = 1L << 20, // print enviromental data that have been measured (differense for Serial and sim800)
  TARE                = 1L << 21, // tare the scales
  UPDATE_DATE_TIME    = 1L << 22, // synchronise time (only Serial)

  DEBUG_COMM          = 1L << 31  // doesn't has default action
};
