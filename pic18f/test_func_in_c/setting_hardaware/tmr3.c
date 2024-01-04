#include "tmr3.h"

#include <stdbool.h>
#include <xc.h>

void TMR3Init(bool isINT) {
    T3CONbits.RD16 = 0;
    T3CONbits.TMR3CS = 0;     // Fosc/4
    T3CONbits.T3CKPS = 0b00;  // 1:1

    if (isINT) {
        PIR2bits.TMR3IF = 0;
        IPR2bits.TMR3IP = 1;  // High priority
        PIE2bits.TMR3IE = 1;
    }
}

void TMR3Run(int high_byte, int low_byte) {
    T3CONbits.TMR3ON = 0;
    TMR3H = high_byte;
    TMR3L = low_byte;
    T3CONbits.TMR3ON = 1;  // Enable Timer1
}