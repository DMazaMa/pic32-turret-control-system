#include <stdio.h>
#include <xc.h>
#include "Pic32Ini.h"
#define PIN_RB7 7
#define PIN_PULSADOR 5 // RB5 PULSADOR

int main(void) {
    char cad_tx[] = "Hola mundo";
    int i_cad_tx; // INDICE PARA NUESTRA CADENA TRANSMISIÓN
    int pulsador_ant, pulsador_act;
    ANSELB &= ~(1 << PIN_RB7);
    TRISB = 0;
    TRISB |= (1 << PIN_PULSADOR);
    LATB |= 1 << PIN_RB7;
    SYSKEY = 0xAA996655;
    SYSKEY = 0x556699AA;
    RPB7R = 1; // U1TX A RB7
    SYSKEY = 0X1CA11CA1; //
    U1BRG = 32; // 9600 baudios
    U1MODE = 0x8000; // ARRANCAMOS UART
    pulsador_ant = (PORTB >> PIN_PULSADOR) & 1;
    while (1) {
        pulsador_act = (PORTB >> PIN_PULSADOR) & 1;
        if ((pulsador_act != pulsador_ant) && (pulsador_act == 0)) {
            // TRANSMITIMOS EN FLANCO DE BAJADA
            i_cad_tx = 0;
            U1STAbits.UTXEN = 1; //HABILITAMOS TRANSMISOR
            while (cad_tx[i_cad_tx] != '\0') {
                U1TXREG = cad_tx[i_cad_tx];
                while (U1STAbits.TRMT == 0);
                i_cad_tx++;
            }
            U1STAbits.UTXEN = 0;
        }
        pulsador_ant = pulsador_act;
    }
}