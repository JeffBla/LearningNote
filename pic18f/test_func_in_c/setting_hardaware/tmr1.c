#include "tmr1.h"

#include <stdbool.h>
#include <xc.h>

void TMR1Init(bool isINT) {
    T1CONbits.RD16 = 0;
    T1CONbits.TMR1CS = 0;     // Fosc/4
    T1CONbits.T1CKPS = 0b00;  // 1:1

    if (isINT) {
        PIR1bits.TMR1IF = 0;
        IPR1bits.TMR1IP = 1;  // High priority
        PIE1bits.TMR1IE = 1;
    }
}

void TMR1Run(int high_byte, int low_byte) {
    T1CONbits.TMR1ON = 0;
    TMR1H = high_byte;
    TMR1L = low_byte;
    T1CONbits.TMR1ON = 1;  // Enable Timer1
}