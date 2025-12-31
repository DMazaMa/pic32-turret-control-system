#include <xc.h>
#include <stdint.h>
#include "ModuloServo.h"
#include "Pic32Ini.h"  // Si tienes inicializaciones globales aquí

int sumando = 1; // Controla la dirección del movimiento del servo

// ? Interrupción del Timer 3
__attribute__((vector(20), interrupt(IPL5SOFT), nomips16))
void InterrupcionT3(void) {
    IFS0bits.T5IF = 0; // Limpiar bandera de interrupción

    if (sumando) {
        sumAngulo(30); // Aumenta el ángulo 10°
    } else {
        sumAngulo(-30); // Disminuye el ángulo 10°
    }

    // Cambiar de dirección al alcanzar los extremos
    if (getGrados() == 90) {
        sumando = 0;
    } else if (getGrados() == -90) {
        sumando = 1;
    }
}

int main(void) {
    // Configurar pines RC0-RC3 como digitales
    ANSELC &= ~0xF;
    LATC |= 0xF;

    // Inicializar servo
    inicializacionServo();

    // Configurar Timer3
    T5CON = 0;
    TMR5 = 0;
    PR5 = 39062; // ~500ms si PBCLK = 5 MHz y prescaler = 64
    T5CON = 0x8070; // ON, prescaler 1:64

    // Configurar interrupción de Timer3
    IPC5bits.T5IP = 3;  // Prioridad
    IPC5bits.T5IS = 0;  // Subprioridad
    IFS0bits.T5IF = 0;  // Limpiar flag
    IEC0bits.T5IE = 1;  // Habilitar interrupción

    // Habilitar interrupciones globales
    INTCONbits.MVEC = 1;
    asm("ei");

    while (1) {
        // El movimiento del servo se gestiona por interrupción
    }

    return 0;
}
