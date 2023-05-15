#pragma once

namespace return_codes {
  typedef enum {
    ERROR     = -1,
    SUCCESS   = 0,
    PERFORMED = 1,

    NO_REQUEST = 2,

    ERROR_NUMBER_TO_LONG = 10
  } return_code_t;
};
