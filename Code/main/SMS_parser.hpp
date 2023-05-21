#pragma once

#include "str_macroses.hpp"

namespace pars_states {
  typedef enum {
    INPUT_COMMAND_REPEAT  = 0,
    HEAD        = 1,
    ERROR       = 2,
    CMGL        = 3,
    GET_SMS_ID  = 4,

    BEGIN_SMS_TYPE  = 5,
    SMS_TYPE        = 6,
    END_SMS_TYPE    = 7,
    BEGIN_NUMBER    = 8,
    NUMBER          = 9,
    END_NUMBER      = 10,
    BEGIN_ALPHA     = 11,
    ALPHA           = 12,
    END_ALPHA       = 13,
    BEGIN_DATE_TIME = 14,
    DATE_TIME       = 15,
    END_DATE_TIME   = 16,

    PARS_MESSAGE  = 17,

    SKIP_LINE = 18,

    PARS_STATES_COUNT
  } pars_state_t;

  typedef enum {
    BEGIN,
    SKIP,

    P,
    PR,
    PRI,
    PRIN,
    PRINT,
    PRINT_,
    PRINT_S,
    PRINT_SM,
    PRINT_SMS,

    S,
    SE,
    SET,
    SET_,
    SET_M,

    SET_MA,
    SET_MAX,

    SET_MI,
    SET_MIN,

    SET_M_,
    SET_M_T,
    SET_M_TE,
    SET_M_TEM,
    SET_M_TEMP,
    SET_M_TEMPI,
    SET_M_TEMPIR,
    SET_M_TEMPIRA,
    SET_M_TEMPIRAT,
    SET_M_TEMPIRATU,
    SET_M_TEMPIRATUR,
    SET_M_TEMPIRATURE,

    SET_M_H,
    SET_M_HU,
    SET_M_HUM,
    SET_M_HUMI,
    SET_M_HUMID,
    SET_M_HUMIDI,
    SET_M_HUMIDIT,
    SET_M_HUMIDITU,

    PARS_SMS_STATES_COUNT
  } pars_sms_state_t;

  pars_state_t getSMSID() {
    bool skip_spaces = true;
    bool overflow = false;
    long exp = 1L;
    long id = 0L;

PARS_SMS_ID:
    while (sim800.available) {
      char c = sim800.read();

      if (skip_spaces && c != ' ' && c != '\t') {
        skip_spaces = false;
      }
      if (!skip_spaces) {
        if (isDigitSymbol(c)) {
          if (overflow) {
            return return_codes::ERROR;
          }
          id += (long)(c - '0') * exp;
          if (exp >= 1000000000L) {
            overflow = true;
          } else {
            exp *= 10L;
          }
        }
      }

      for (int i = 0; i < 1000; ++i) {
        delay(10);
        if (sim800.available()) {
          goto PARS_SMS_ID;
        }
      }
    }

    id = reversNumber(id);
  }

  pars_state_t parsNumber() {
PARS_MUNBER:

    int exp = 16;
    long exp1 = 1000000000;
    long exp2 = 1000000000;

    while (sim800.available()) {
      char c = sim800.read();

      
    }
    
    for (int i = 0; i < 1000; ++i) {
      delay(10);
      if (sim800.available()) {
        goto PARS_MUNBER;
      }
    }
  }
};

using namespace pars_states;

return_code_t parsResponse() {
  pars_state_t pars_state = INPUT_COMMAND_REPEAT;

PARS_SIM800_RESPONSE:
  while (sim800.available) {
    char c = sim800.read();

#ifdef SERIAL_DEBUG
  pars_state_t last_pars_state = pars_state;
#endif // SERIAL_DEBUG

    switch (pars_state) {
      case INPUT_COMMAND_REPEAT:  pars_state = (c == '\n') ? HEAD : INPUT_COMMAND_REPEAT;                 break;
      case HEAD:                  pars_state = isSpaceSymbol(c) ? HEAD : (c == '+') ? CMGL : ERROR;       break;
      case CMGL:                  pars_state = isSpaceSymbol(c) ? ERROR : (c == ':') ? GET_SMS_ID : CMGL; break;
      case GET_SMS_ID:            pars_state = getSMSID();                                                break;
      
      case BEGIN_SMS_TYPE:  pars_state = (c == '\n') ? ERROR : (c == '\"') : SMS_TYPE ? BEGIN_SMS_TYPE;     break;
      case SMS_TYPE:        pars_state = (c == '\n') ? ERROR : (c == '\"') : END_SMS_TYPE ? SMS_TYPE;       break;
      case END_SMS_TYPE:    pars_state = (c == '\n') ? ERROR : (c == ',')  : BEGIN_NUMBER ? END_SMS_TYPE;   break;
      case BEGIN_NUMBER:    pars_state = (c == '\n') ? ERROR : (c == '\"') : NUMBER ? BEGIN_NUMBER;         break;
      case NUMBER:          pars_state = parsNumber();                                                      break;
      case END_NUMBER:      pars_state = (c == '\n') ? ERROR : (c == ',')  : BEGIN_ALPHA ? END_NUMBER;      break;
      case BEGIN_ALPHA:     pars_state = (c == '\n') ? ERROR : (c == '\"') : ALPHA ? BEGIN_ALPHA;           break;
      case ALPHA:           pars_state = (c == '\n') ? ERROR : (c == '\"') : END_ALPHA ? ALPHA;             break;
      case END_ALPHA:       pars_state = (c == '\n') ? ERROR : (c == ',')  : BEGIN_DATE_TIME ? END_ALPHA;   break;
      case BEGIN_DATE_TIME: pars_state = (c == '\n') ? ERROR : (c == '\"') : BEGIN_DATE_TIME ? DATE_TIME;   break;
      case DATE_TIME:       pars_state = (c == '\n') ? ERROR : (c == '\"') : DATE_TIME ? END_DATE_TIME;     break;
      case END_DATE_TIME:   pars_state = (c == '\n') : END_DATE_TIME ? PARS_MESSAGE;                        break;
      
      case PARS_MESSAGE:    pars_message(); break;

      case SKIP_LINE: pars_state = (c == '\n') : HEAD ? SKIP_LINE;

      case ERROR:
        printDebug("parsResponse: pars error: last char: ");
        printDebugInLine(c == '\n' ? "\\n" : c == '\r' ? "\\r" : c == '\t' ? "\\t" : c);
        printDebugInLine(" last_pars_state: ");
#ifdef SERIAL_DEBUG
        printDebugInLine(last_pars_state);
#endif // SERIAL_DEBUG
        printDebugInLine("\n\n");
        
        pars_state = SKIP_LINE;
      
      default:
        printError("parsResponse: unknow state: ");
        printDebugInLine(pars_state);
        printDebugInLine("\n\n");
        return returs_codes::ERROR;
    }
  }

  for (int i = 0; i < 1000; ++i) {
    delay(10);
    if (sim800.available()) {
      goto PARS_SIM800_RESPONSE;
    }
  }

  return returs_codes::SUCCESS;
}

return_code_t parsMessage() {

}