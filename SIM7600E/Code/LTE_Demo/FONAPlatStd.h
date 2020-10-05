/*
 * FONAPlatStd.h -- standard AVR/Arduino platform.
 *
 * This is part of the library for the Adafruit FONA Cellular Module
 *
 */


#ifndef ADAFRUIT_FONA_LIBRARY_SRC_INCLUDES_PLATFORM_FONAPLATSTD_H_
#define ADAFRUIT_FONA_LIBRARY_SRC_INCLUDES_PLATFORM_FONAPLATSTD_H_

#include "FONAConfig.h"


#if (ARDUINO >= 100)
  #include "Arduino.h"
  #if !defined(__SAM3X8E__) && !defined(ARDUINO_ARCH_SAMD)  // Arduino Due doesn't support #include <SoftwareSerial.h>
  #endif
#else
  #include "WProgram.h"
  #include <NewSoftSerial.h>
#endif

#if (defined(__AVR__))
  #include <avr/pgmspace.h>
// #elif (defined(__ARM__))
// 	#define PROGMEM const
#elif (defined(ESP8266))
	#include <pgmspace.h>
#endif

// DebugStream	sets the Stream output to use
// for debug (only applies when ADAFRUIT_FONA_DEBUG
// is defined in config)
#if defined(ARDUINO_ARCH_SAMD)
  #define DebugStream   SERIAL_PORT_USBVIRTUAL // Needed for SAMD21
#else
  #define DebugStream		Serial
#endif

#ifdef ADAFRUIT_FONA_DEBUG
// need to do some debugging...
#define DEBUG_PRINT(...)		DebugStream.print(__VA_ARGS__)
#define DEBUG_PRINTLN(...)		DebugStream.println(__VA_ARGS__)
#endif

// a few typedefs to keep things portable
typedef	Stream 						FONAStreamType;
typedef const __FlashStringHelper *	FONAFlashStringPtr;

#define prog_char  					char PROGMEM

#define prog_char_strcmp(a, b)					strcmp_P((a), (b))
// define prog_char_strncmp(a, b, c)				strncmp_P((a), (b), (c))
#define prog_char_strstr(a, b)					strstr_P((a), (b))
#define prog_char_strlen(a)						strlen_P((a))
#define prog_char_strcpy(to, fromprogmem)		strcpy_P((to), (fromprogmem))
//define prog_char_strncpy(to, from, len)		strncpy_P((to), (fromprogmem), (len))

#endif /* ADAFRUIT_FONA_LIBRARY_SRC_INCLUDES_PLATFORM_FONAPLATSTD_H_ */
