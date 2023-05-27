#pragma once

#include <SoftwareSerial.h>

#ifdef SERIAL_DEBUG
#define printError(msg) { Serial.print(F("ERROR: ")); Serial.print(msg); }
#define printDebug(msg) { Serial.print(F("DEBUG: ")); Serial.print(msg); }
#define printWarning(msg) { Serial.print(F("WARNING: ")); Serial.print(msg); }
#define printDebugInLine(msg); { Serial.print(msg); }
#else // SERIAL_DEBUG
#define printError(msg) {}
#define printDebug(msg) {}
#define printWarning(msg) {}
#define printDebugInLine(msg); {}
#endif // SERIAL_DEBUG

