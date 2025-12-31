#include <xc.h>
#include "uart.h"

#define PBCLK 5000000
#define BAUDRATE 9600
#define BRGVAL ((PBCLK / (16 * BAUDRATE)) - 1)

void initUART() {
    // Desbloqueo de PPS
    
    TRISB &=~(1<<7);
    TRISB |= (1<<13);
    SYSKEY = 0xAA996655;
    SYSKEY = 0x556699AA;

    U1RXR = 3;    // U1RX en RB13
    RPB7R = 1;    // U1TX en RB7

    SYSKEY = 0x1CA11CA1;

    // Configurar pines como digitales
    ANSELB &= ~(1 << 7);   // RB7 (TX) digital
    ANSELB &= ~(1 << 13);  // RB13 (RX) digital

    // Configurar UART1
    U1MODE = 0;
    U1BRG = BRGVAL;
    U1STA = 0;
    U1MODEbits.ON = 1;
    U1STAbits.URXEN = 1;
    U1STAbits.UTXEN = 1;
}

void sendChar(char c) {
    while (U1STAbits.TRMT==0);  // Esperar a que el buffer esté libre
    U1TXREG = c;
}

void sendString(const char *s) {
    while (*s) {
        sendChar(*s++);
    }
}

char recibirChar() {
    while (!U1STAbits.URXDA); // Esperar a que llegue un carácter
    return U1RXREG;           // Devolver carácter recibido
}

uint8_t UARTDisponible() {
    return U1STAbits.URXDA; // 1 si hay dato disponible
}
