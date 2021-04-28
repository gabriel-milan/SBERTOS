# SBERTOS - Simple But Effective RTOS

A very simple implementation of a RTOS with a non-preemptive scheduler meant to be compatible with both AVR- and ESP8266-based boards. This project is inspired on [this](https://www.embarcados.com.br/elementos-de-rtos-no-arduino/) tutorial and its simplistic approach. Other awesome projects worth mentioning, as I'm using them for development of SBERTOS:

- [ivanseidel/LinkedList](https://github.com/ivanseidel/LinkedList)
- [khoih-prog/ESP8266TimerInterrupt](https://github.com/khoih-prog/ESP8266TimerInterrupt)
- [PaulStoffregen/TimerOne](https://github.com/PaulStoffregen/TimerOne)

SBERTOS is still in very early development, any suggestion/effort is appreciated.

## Development status

Please submit issues with feature requests, I'm running out of ideas!

- [x] AVR- and ESP8266-based boards compatibility
- [x] Basic non-preemptive scheduler
- [x] WDT for tasks with different timeouts
- [ ] Logging functionalities
- [ ] Add tasks with arguments
- [ ] Implementation of priority
- [ ] Priority queue scheduling
