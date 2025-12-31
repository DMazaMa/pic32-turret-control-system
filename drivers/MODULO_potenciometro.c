#include <xc.h>
#include <stdint.h>

// Inicializa el ADC para leer el eje X del joystick (AN0 / RA0)
void initADC() {
    ANSELAbits.ANSA0 = 1;
    TRISAbits.TRISA0 = 1;

    AD1CON1bits.FORM = 0;
    AD1CON1bits.SSRC = 7;
    AD1CON1bits.ASAM = 0;

    AD1CHSbits.CH0SA = 0;
    AD1CON3bits.ADCS = 2;
    AD1CON3bits.SAMC = 10;

    AD1CON2 = 0;
    AD1CON1bits.ADON = 1;
}

// Leer valor analógico del joystick (0 a 1023)
uint16_t leerPotenciometro() {
    AD1CON1bits.SAMP = 1;
    while (!AD1CON1bits.DONE);
    return ADC1BUF0;
}

// Devuelve -1 si hay que restar ángulo, 1 si hay que sumar, 0 si no se toca
// Devuelve -1 si joystick está completamente a la izquierda,
// 1 si está completamente a la derecha,
// y 0 si está en posición neutra o intermedia.
int8_t adcToGrados(uint16_t adc_val) {
    if (adc_val < 100) {
        return -1; // Solo si está completamente a la izquierda
    } else if (adc_val > 920) {
        return 1; // Solo si está completamente a la derecha
    } else{
        return 0;
    }
}

