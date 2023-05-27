#pragma once

enum COMMANDS {
  EMPTY_COMMAND       = 0,

  DELETE_SMS_ALL      = 1 << 0,
  CHECK_OK            = 1 << 1,
  CMGF_EN             = 1 << 2,
  GET_LAST_SMS_ID     = 1 << 3,
  SEND_SMS            = 1 << 4,
  SET_NUMBER          = 1 << 5,
  SET_MAX_TEMPERATURE = 1 << 6,
  SET_MAX_HUMIDITY    = 1 << 7,
  SET_MIN_TEMPERATURE = 1 << 8,
  SET_MIN_HUMIDITY    = 1 << 9,
  PRINT_SMS           = 1 << 10,
  PRINT_SMS_ALL       = 1 << 11,
  PRINT_STORED_DATA   = 1 << 12,
  PRINT_MEASURED_DATA = 1 << 13,
  UPDATE_DATE_TIME    = 1 << 14,

  DEBUG_COMM          = 1 << 15
};
