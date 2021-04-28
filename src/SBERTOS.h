/*
 *  SBERTOS - Simple But Effective RTOS
 *  
 *  This project was inspired by https://www.embarcados.com.br/elementos-de-rtos-no-arduino/
 *  Thank you Caio Moraes!
 * 
 */

#pragma once

#include <Arduino.h>
#include "LinkedList.h" // Extracted from https://github.com/ivanseidel/LinkedList, all credits to the original author(s)
#ifdef ESP8266
#include "ESP8266TimerInterrupt.h" // Extracted from https://github.com/khoih-prog/ESP8266TimerInterrupt, all credits to the original author(s)
#elif __AVR__
#include "avr/wdt.h"
#include "TimerOne.h" // Extracted from https://github.com/PaulStoffregen/TimerOne, all credits to the original author(s)
#endif

/*
 * Return codes
 */
#define SUCCESS 0
#define FAIL -1

/*
 * Constants
 */
#define SBERTOS_DEFAULT_MAX_EXECUTION_TIME 1000

/*
 * Kernel variables
 */

// Typedef for function pointer
typedef void (*ptrFunc)();

// Struct for storing task information
typedef struct
{
  ptrFunc func;
  const char *taskName;
  bool enabled;
  uint16_t period;
  uint16_t wdtPeriod;
} TaskHandle;

/*
 * SBERTOS main class
 */
class SBERTOS
{
public:
  SBERTOS();
  char addTask(TaskHandle *_task, ptrFunc _func, const char *_taskName, bool _enabled, uint16_t _period, uint16_t _wdtPeriod = SBERTOS_DEFAULT_MAX_EXECUTION_TIME);
  char removeTask(TaskHandle *_task);
  void run(void);

private:
  TaskHandle *loopTask;
  LinkedList<TaskHandle *> tasks;
  LinkedList<uint32_t> taskCounter;
  LinkedList<int16_t> taskWdt;
#ifdef ESP8266
  ESP8266Timer timer;
#elif __AVR__
  TimerOne timer;
#endif
};
