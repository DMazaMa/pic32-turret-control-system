#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include "hcsr04.h"
#include "uart.h"
#include "motor.h"
#include "Retardos.h"


#define PBCLK 5000000  // Peripheral Bus Clock a 5 MHz

void iniLaser(void) {
    TRISB &= ~(1 << 14);
    ANSELB &= ~(1 << 14);
    LATB |= (1 << 14);
}

int main() {
    uint16_t distance;

    U1STAbits.UTXEN = 0;
    U1STAbits.UTXEN = 1;


    initHC_SR04();
    initUART();
    initMotor();
    iniLaser();
    

    while (1) {



        distance = getDistance();

        // Control de la velocidad del motor según la distancia
        uint16_t speed;
        if (distance < 200) {
            speed = 180; // Velocidad baja si el objetivo está cerca
        } else if (distance < 300) {
            speed = 220; // Velocidad media
        } else {
            speed = 255; // Velocidad máxima si el objetivo está lejos
        }

        controlMotor(speed);

        // Enviar distancia y velocidad por UART
        char buffer[40];
        sprintf(buffer, "Distancia: %d cm, Motor: %d\r\n", distance, speed);
        sendString(buffer);

        delay_ms(500);
    }
}

// Implementación de delay con Timer1 basado en PBCLK de 5 MHz

