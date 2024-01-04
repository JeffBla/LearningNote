#include "tmr2.h"

#include <stdbool.h>
#include <xc.h>

void TMR2Init(bool isINT) {
    T2CONbits.T2OUTPS = 0x0;  // postscale 1:1
    T2CONbits.T2CKPS = 0b11;  // prescale 1:16

    if (isINT) {
        PIR1bits.TMR2IF = 0;
        IPR1bits.TMR2IP = 1;  // High priority
        PIE1bits.TMR2IE = 1;
    }
}

void TMR2Run(int thres) {
    T2CONbits.TMR2ON = 0;
    TMR2 = 0;
    PR2 = thres;
    T2CONbits.TMR2ON = 1;  // Enable Timer2
}