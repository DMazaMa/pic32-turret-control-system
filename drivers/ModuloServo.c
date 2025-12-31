#include <xc.h>
#include <stdint.h>
#include "Retardos.h"

int grados = -90;      // Servo principal
int barrera_activa = 0; // Estado de la barrera (0 = cerrada, 1 = abierta)

void inicializacionServo(void) {
    ANSELB &= ~(1 << 15);  // RB15 digital (servo principal)
    ANSELC &= ~(1 << 6);   // RB3 digital (servo barrera)
    TRISB &= ~(1<<15);
    TRISC &= ~(1<<6);

    // PPS: OC1 ? RB15, OC4 ? RB3
    SYSKEY = 0xAA996655;
    SYSKEY = 0x556699AA;
    RPB15R = 5; // OC1
    RPC6R  = 5; // OC4
    SYSKEY = 0x1CA11CA1;

    // Timer3 para ambos servos
    T3CON = 0;
    TMR3 = 0;
    PR3 = 49999;
    T3CON = 0x8010;

    // OC1 (servo principal)
    OC1CON = 0;
    OC1R = 1250;
    OC1RS = 1250;
    OC1CONbits.OCTSEL=1;
    OC1CON |= 0x8006;


    
    //OC4 (servo barrera)
    OC4CON = 0;
    OC4R = 1250;      // Barrera inicialmente cerrada (1 ms)
    OC4RS = 0;
    OC4CONbits.OCTSEL=1;
    OC4CON |= 0x8006;
}

void sumAngulo(int delta) {
    if (delta > 40 || delta < -40) delta = 0;
    grados += delta;
    if (grados > 90) grados = -90;
    if (grados < -90) grados = 90;
    OC1RS = 1250 + (grados + 90) * 14;
}

void setGrados(int nuevo_grado) {
    if (nuevo_grado > 90) nuevo_grado = 90;
    if (nuevo_grado < -90) nuevo_grado = -90;
    grados = nuevo_grado;
    OC1RS = 1250 + (grados + 90) * 14;
}

int getGrados(void) {
    return grados;
}

void activarBarrera() {
    OC4RS = 3770; // ~2 ms ? posición abierta //ASI GIRARIAMOS SOLO 90 GRADOS, NO 180 CON 1250
    barrera_activa = 1;
}

void cerrarBarrera() {
    OC4RS = 1250; // ~1 ms ? posición cerrada
    barrera_activa = 0;
}

int estadoBarrera() {
    return barrera_activa;
}

void delayServoPorAngulo(int delta) {
    // Aprox. 500ms por 60° => 8.3ms por grado
    int tiempo = delta * 8.3;
    delay_ms((uint16_t) tiempo);
}