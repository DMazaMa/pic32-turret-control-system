#include <xc.h>
#include <stdint.h>
#include <stdbool.h>

#define _XTAL_FREQ 8000000UL  // Solo para referencia de frecuencia si usas __delay

int grados = -90;

// ------------------- Inicialización del Servo en OC1 (RB15) -------------------
void inicializacionServo(void) {
    ANSELB &= ~(1 << 15);   // Pin RB15 como digital

    // PPS: Remapear OC1 a RB15
    SYSKEY = 0xAA996655;
    SYSKEY = 0x556699AA;
    RPB15R = 5; // OC1 -> RB15
    SYSKEY = 0x1CA11CA1;

    // Configurar OC1 (PWM)
    OC1CON = 0;
    OC1R = 1250;     // Ciclo inicial
    OC1RS = 1250;    // Registro de comparación
    OC1CON = 0x8006; // Modo PWM, usar Timer2

    // Configurar Timer2 para 50 Hz (20 ms periodo)
    T2CON = 0;
    TMR2 = 0;
    PR2 = 49999;        // (5 MHz / 8) / 50 Hz = 12500 ? PR2 = 49999 para 1:8
    T2CON = 0x8010;     // Timer ON, prescaler 1:8
}

// ------------------- Sumar Ángulo al Servo -------------------
void sumAngulo(int delta_grados) {
    if (delta_grados > 40 || delta_grados < -40) {
        delta_grados = 0;
    }

    grados += delta_grados;

    if (grados > 90) grados = 90;
    if (grados < -90) grados = -90;

    // Mapeo a pulso: 1250 (1 ms) a 2500 (2 ms) ? 14 unidades por grado
    OC1RS = 1250 + (grados + 90) * 14;
}

// ------------------- Inicialización ADC para leer joystick -------------------
void ADC_Init(void) {
    ANSELAbits.ANSA0 = 1; // AN0 como analógico
    TRISAbits.TRISA0 = 1; // Entrada

    AD1CON1bits.FORM = 0;       // Resultado como entero
    AD1CON1bits.SSRC = 7;       // Auto trigger
    AD1CON1bits.ASAM = 1;       // Auto sample

    AD1CON2 = 0;
    AD1CON3bits.ADCS = 2;
    AD1CON3bits.SAMC = 15;

    AD1CON1bits.ADON = 1;       // Encender ADC
}

// ------------------- Leer valor del ADC -------------------
uint16_t ADC_Read(uint8_t canal) {
    AD1CHSbits.CH0SA = canal;
    AD1CON1bits.SAMP = 1;
    for (volatile int i = 0; i < 20; i++); // Pequeño delay (~10 us)
    AD1CON1bits.SAMP = 0;
    while (!AD1CON1bits.DONE);
    return ADC1BUF0;
}

// ------------------- Delay en milisegundos usando Timer1 -------------------
void delay_ms(uint16_t ms) {
    for (uint16_t i = 0; i < ms; i++) {
        T1CON = 0x8000;     // Timer ON, prescaler 1:1
        TMR1 = 0;
        while (TMR1 < (5000000 / 1000));  // Esperar 1 ms con PBCLK = 5 MHz
        T1CONbits.ON = 0;
    }
}

// ------------------- Función Principal -------------------
int main(void) {
    ADC_Init();
    inicializacionServo();

    uint16_t anterior = 512;

    while (1) {
        uint16_t vrx = ADC_Read(0); // Leer joystick en AN0

        if (vrx > anterior + 50) {
            sumAngulo(5); // Mover a la derecha
            anterior = vrx;
        } else if (vrx < anterior - 50) {
            sumAngulo(-5); // Mover a la izquierda
            anterior = vrx;
        }

        delay_ms(100);
    }

    return 0;
}
