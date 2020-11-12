/**
 * Author: Tinasoft.vn
 */
#ifndef APP_DEBUG_H
#define APP_DEBUG_H

#define DEBUG

#ifdef DEBUG
#define ECHOABLE 1
#define ECHO(m) ECHOABLE == 1 ? Serial.print(m) : 0
#define ECHOLN(m) ECHOABLE == 1 ? Serial.println(m) : 0
#define FREE_HEAP() ({ECHOLN(String("FREE_HEAP: ") + String(ESP.getFreeHeap()));})
#else
#define ECHO(m)
#define ECHOLN(m)
#define FREE_HEAP()
#endif

#endif /* APP_DEBUG_H */
