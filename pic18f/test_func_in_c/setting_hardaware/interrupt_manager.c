#include <stdio.h>
#include <xc.h>

#include "pin_manager.h"
#include "tmr1.h"

#define _XTAL_FREQ 4000000

int captureTMR_result;

void INTERRUPT_Initialize(void) {
    // INT0
    // TRISBbits.RB0 = 1;
    // INTCONbits.INT0IE = 1;
    // INTCONbits.INT0IF = 0;

    // Global
    RCONbits.IPEN = 1;    // enable Interrupt Priority mode
    INTCONbits.GIEH = 1;  // enable high priority interrupt
    INTCONbits.GIEL = 1;  // disable low priority interrupt
}

void __interrupt(high_priority) Hi_ISR(void) {
    if (INTCONbits.INT0IF) {
        // LED1_Toggle();
        TMR1Run(0x00, 0x00);

        __delay_ms(500);
        INTCONbits.INT0IF = 0;
    } else if (PIR1bits.TMR1IF) {
        LED1_Toggle();

        TMR1Run(0x00, 0x00);
        PIR1bits.TMR1IF = 0;
    } else if (PIR1bits.TMR2IF) {
        LED1_Toggle();

        PIR1bits.TMR2IF = 0;
    } else if (PIR1bits.CCP1IF) {
        // Capture mode
        if (T1CONbits.TMR1ON == 1) {
            T1CONbits.TMR1ON = 0;  // Stop Timer1. Please check using Time1 or not.
            captureTMR_result = CCPR1H << 8;
            captureTMR_result |= CCPR1L;
        } else {
            TMR1Run(0x00, 0x00);
            captureTMR_result -= CCPR1H << 8;
            captureTMR_result -= CCPR1L;
        }

        // Compare mode
        // T1CONbits.TMR1ON = 0;  // Stop Timer1. Please check using Time1 or not.

        PIR1bits.CCP1IF = 0;
    } else if (PIR1bits.ADIF) {
        int value = (ADRESH << 8) | (ADRESL);  // left justified
        // int value = ADRESH; // right justified

        // You need to have a minimum wait of 2 T_AD before next acquisition start,
        // then go back to step 3.
        // __delay_us(4);
        // ADCON0bits.GO = 1;
        PIR1bits.ADIF = 0;
    }
}
