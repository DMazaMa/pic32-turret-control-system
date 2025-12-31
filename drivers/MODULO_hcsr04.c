#include <xc.h>
#include <stdint.h>
#include "hcsr04.h"
#include "Pic32Ini.h"

#define PIN_TRIG LATCbits.LATC4  // TRIG en RC4
#define PIN_ECHO PORTCbits.RC5   // ECHO en RC5
#define PBCLK 5000000

void delay_ms1(uint16_t ms) {
    for (uint16_t i = 0; i < ms; i++) {
        T1CON = 0x8000; // Habilitar Timer1, prescaler 1:1
        TMR1 = 0;
        while (TMR1 < (PBCLK / 1000)); // Esperar 1 ms (5000 ciclos)
        T1CONbits.ON = 0; // Apagar Timer1
    }
}
void initHC_SR04() {
    // Configurar TRIG como salida (RC4), ECHO como entrada (RC5)
    TRISC &= ~(1 << 4); // RC4 como salida (poner a 0 el bit 4 de TRISC)
    TRISC |= (1 << 5); // RC5 como entrada (poner a 1 el bit 5 de TRISC)

    // Configurar Timer1
    T1CON = 0x0000; // Apagar Timer1
    TMR1 = 0; // Reset del contador
    T1CONbits.TCKPS = 2; // Prescaler 1:64 (ajustado para precisión)
}

uint16_t getDistance() {
    uint16_t time;

    // Generar pulso de 10 us en TRIG
    PIN_TRIG = 0;
    delay_ms1(2);
    PIN_TRIG = 1;
    delay_ms1(10);
    PIN_TRIG = 0;

    // Esperar hasta que ECHO esté en alto
    while (!PIN_ECHO);
    TMR1 = 0;
    T1CONbits.ON = 1;

    // Esperar hasta que ECHO vuelva a bajo
    while (PIN_ECHO);
    T1CONbits.ON = 0;

    time = TMR1;

    // Convertir tiempo a distancia en centímetros
    return (time / 58);
}
