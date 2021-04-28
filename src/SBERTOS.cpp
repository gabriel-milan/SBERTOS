/*
 *  SBERTOS - Simple But Effective RTOS
 *  
 *  This project was inspired by https://www.embarcados.com.br/elementos-de-rtos-no-arduino/
 *  Thank you Caio Moraes!
 * 
 */

#include "SBERTOS.h"

/*
 * Initializations
 */
volatile bool timerTriggered = false;
volatile uint32_t sysTickCounter = 0;
volatile bool taskInExecution = false;
volatile uint16_t executionTime = 0;

/*
 * Interrupt Service Routine
 */
#ifdef ESP8266
void ICACHE_RAM_ATTR isrTimer(void)
#elif __AVR__
void isrTimer(void)
#endif
{
  timerTriggered = true;
  sysTickCounter++;

  if (taskInExecution)
  {
    executionTime--;
    if (!executionTime)
    {
#ifdef ESP8266
      // Enable WDT just in case reset fails
      ESP.wdtEnable(15);
      // Tries to reset module
      ESP.reset();
#elif __AVR__
      // Enables WDT for resetting
      wdt_enable(WDTO_15MS);
#endif
      // Infinite loop
      while (true)
        ;
    }
  }
}

/*
 * Constructor
 */
SBERTOS::SBERTOS()
{
  this->tasks = LinkedList<TaskHandle *>();
  this->taskCounter = LinkedList<uint32_t>();
  this->taskWdt = LinkedList<int16_t>();
#ifdef ESP8266
  this->timer.attachInterruptInterval(1000, isrTimer);
#elif __AVR__
  this->timer.initialize(1000);
  this->timer.attachInterrupt(isrTimer);
#endif
  // this->addTask(loop, "arduino_loop", 0, true, this->loopTask);
}

/*
 * Tasks handling
 */
char SBERTOS::addTask(TaskHandle *_task, ptrFunc _func, const char *_taskName, bool _enabled, uint16_t _period, uint16_t _wdtPeriod)
{
  _task->func = _func;
  _task->taskName = _taskName;
  _task->enabled = _enabled;
  _task->period = _period;
  _task->wdtPeriod = _wdtPeriod;

  for (int i = 0; i < this->tasks.size(); i++)
    if (this->tasks.get(i) == _task)
      return SUCCESS;

  if ((this->tasks.add(_task)) && (this->taskCounter.add(0)) && (this->taskWdt.add(_wdtPeriod)))
    return SUCCESS;

  return FAIL;
}

char SBERTOS::removeTask(TaskHandle *_task)
{
  for (int i = 0; i < this->tasks.size(); i++)
    if (this->tasks.get(i) == _task)
    {
      this->tasks.remove(i);
      return SUCCESS;
    }
  return FAIL;
}

/*
 * Main execution
 */
void SBERTOS::run()
{
  int i;
  for (;;)
  {
    if (timerTriggered)
      for (i = 0; i < this->tasks.size(); i++)
      {
        TaskHandle *task = this->tasks.get(i);
        if (((sysTickCounter - this->taskCounter.get(i)) > task->period) && (task->enabled))
        {
          taskInExecution = true;
          executionTime = this->taskWdt.get(i);
          task->func();
          taskInExecution = false;
          this->taskCounter.set(i, sysTickCounter);
        }
      }
    yield();
  }
}
