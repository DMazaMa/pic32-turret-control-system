#include <xc.h>
#include <stdint.h>
#include "Pic32Ini.h"

// Definiciones claras de pines
#define PIN_PULSADOR     5   // RB5
#define PIN_PWM_MOTOR_A  8   // RC7 (Salida PWM)
#define PIN_DIR_MOTOR_A  9   // RC6 (Dirección)
#define PERIODO          249 // Para PWM con periodo de 50us

int main(void) {

    int pulsador_ant, pulsador_act;
    int factor_servicio = 0; // Ciclo útil inicial del PWM (0%)

    // Configuración analógica/digital
    ANSELB &= ~(1 << PIN_PULSADOR); // Pulsador como digital
    ANSELC &= ~((1 << PIN_PWM_MOTOR_A) | (1 << PIN_DIR_MOTOR_A)); // Pines motor digitales

    // Configuración de salidas
    TRISB |= (1 << PIN_PULSADOR); // RB5 como entrada (pulsador)
    TRISC &= ~((1 << PIN_PWM_MOTOR_A) | (1 << PIN_DIR_MOTOR_A)); // RC6 y RC7 como salidas

    // Inicialización salidas
    LATC &= ~((1 << PIN_PWM_MOTOR_A) | (1 << PIN_DIR_MOTOR_A)); // Pines motor a 0 inicialmente

    // Desbloquear PPS para configurar salidas
    SYSKEY = 0xAA996655;
    SYSKEY = 0x556699AA;
    RPC8R = 0x05; 
    SYSKEY = 0x1CA11CA1; // Bloquear PPS

    // Configuración PWM (OC1 usando Timer2)
    OC2CON = 0;
    OC2R = 0; // Valor inicial del pulso (apagado)
    OC2RS = 0; // Inicialmente apagado
    OC2CON = 0x8006; // PWM ON, modo PWM estándar

    // Configuración del Timer2
    T2CON = 0;
    TMR2 = 0;
    PR2 = PERIODO; // Periodo de PWM 50us
    T2CON = 0x8000; // Timer2 ON, prescaler 1:1
    
    // Lectura inicial del pulsador
    pulsador_ant = (PORTB >> PIN_PULSADOR) & 1;
    
    while (1) {

        // Lectura del pulsador actual
        pulsador_act = (PORTB >> PIN_PULSADOR) & 1;

        // Detectar flanco descendente (pulsación)
        if ((pulsador_act != pulsador_ant) && (pulsador_act == 0)) {

            factor_servicio += 10; // Incrementa el ciclo útil en 10%

            if (factor_servicio > 100) {
                factor_servicio = 0; // Vuelve al 0% al superar el 100%
            }

            OC2RS = (factor_servicio * PERIODO) / 100; // Actualiza PWM
        }

        pulsador_ant = pulsador_act; // Actualiza estado anterior
    }
}

