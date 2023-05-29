#pragma once

enum COMMANDS {
  EMPTY_COMMAND       = 0,

  DELETE_SMS_ALL      = 1L << 0,
  CHECK_OK            = 1L << 1,
  CMGF_EN             = 1L << 2,
  GET_LAST_SMS_ID     = 1L << 3,
  GET_TIME            = 1L << 4,
  SEND_SMS            = 1L << 5,
  SET_NUMBER          = 1L << 6,
  SET_MAX_TEMPERATURE = 1L << 7,
  SET_MAX_HUMIDITY    = 1L << 8,
  SET_MIN_TEMPERATURE = 1L << 9,
  SET_MIN_HUMIDITY    = 1L << 10,
  SET_SEND_TIME       = 1L << 11,
  PRINT_SMS           = 1L << 12,
  PRINT_SMS_ALL       = 1L << 13,
  PRINT_STORED_DATA   = 1L << 14,
  PRINT_MEASURED_DATA = 1L << 15,
  UPDATE_DATE_TIME    = 1L << 16,

  DEBUG_COMM          = 1L << 31
};
