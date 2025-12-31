#ifndef UART_H
#define UART_H

#include <stdint.h>

void initUART(void);
void sendChar(char c);
void sendString(const char *s);
char recibirChar(void);
uint8_t UARTDisponible(void);

#endif
