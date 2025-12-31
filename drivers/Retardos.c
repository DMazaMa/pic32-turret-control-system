#include <xc.h>
#include <stdint.h>

#define PBCLK 5000000

void delay_us(uint16_t us) {
    T5CON = 0x8000;  // ON, prescaler 1:1
    TMR5 = 0;
    while (TMR5 < (PBCLK / 1000000) * us);
    T5CONbits.ON = 0;
}

void delay_ms(uint16_t ms) {
    for (uint16_t i = 0; i < ms; i++) {
        delay_us(1000);
    }
}
