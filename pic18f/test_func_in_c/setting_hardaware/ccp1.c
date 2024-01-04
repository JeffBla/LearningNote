#include <xc.h>

void CCP1_Initialize() {
    TRISCbits.TRISC2 = 1;  // RC2 pin is output.
    PIR1bits.CCP1IF = 0;
    IPR1bits.CCP1IP = 1;  // High priority
    PIE1bits.CCP1IE = 1;  // Enable CCP1 interrupt
}

void CCP1CaptureInit() {
    T3CONbits.T3CCP2 = 0;  // Use Timer1 as the source clock
    T3CONbits.T3CCP1 = 0;
    CCP1CON = 4;  // Capture mode, every falling edge
    CCPR1H = 0x00;
    CCPR1L = 0x00;
}

void CCP1CompareInit() {
    T3CONbits.T3CCP2 = 0;  // Use Timer1 as the source clock
    T3CONbits.T3CCP1 = 0;
    CCP1CON = 9;  // Compare mode, initialize CCP1 pin high, clear output on compare match
    CCPR1H = 0x03;
    CCPR1L = 0xE8;
}

/**
 * PWM period
 * = (PR2 + 1) * 4 * Tosc * (TMR2 prescaler)
 * = (0xFF + 1) * 4 * 0.25µs * 16
 * ~= 4ms
 * PR2 = 0xFF;
 *
 * Duty cycle
 * = (CCPR1L:CCP1CON<5:4>) * Tosc * (TMR2 prescaler)
 * = (0x0b*4 + 0b01) * 8µs * 4
 * = 0.00144s ~= 1450µs
 * CCPR1L = 0x0b;
 * CCP1CONbits.DC1B = 0b01;
 */
void CCP1PwmInit() {
    CCP1CONbits.CCP1M = 0b1100;  // PWM mode, P1A, P1C active-high; P1B, P1D active-high
    CCPR1L = 0x0b;
    CCP1CONbits.DC1B = 0b01;
}
