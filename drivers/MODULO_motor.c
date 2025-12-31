#include <xc.h>
#include "motor.h"

#define PBCLK 5000000   // Peripheral Bus Clock a 5 MHz
#define MOTOR_PWM OC2RS // Salida de PWM para el motor

void initMotor() {
    // ---- REMAPEO ----
    // Desbloquear registros de sistema para PPS
    SYSKEY = 0xAA996655;
    SYSKEY = 0x556699AA;

    RPC8R = 0x05;

    SYSKEY = 0x1CA11CA1; // Bloquear registros de nuevo

    // Configuración del Timer2 para generar PWM a 20 kHz con PBCLK = 5 MHz
    T2CON = 0;
    TMR2 = 0;
    PR2 = (PBCLK / 20000) - 1; // PR3 = 249 para 20 kHz
    T2CON = 0X8000;

    // Configurar PWM en OC2
    OC2CON = 224;
    OC2RS = 0;
    OC2CON = 0x8006;
}

void controlMotor(uint16_t speed) {
    if (speed > 255) {
        speed = 255;
    }
    MOTOR_PWM = (speed * (PR2 + 1)) / 255;
}
