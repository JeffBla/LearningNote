#include "adc.h"

#include <xc.h>

void ADC_Initialize(bool isINT) {
    TRISA = 0xff;         // Set as input port
    ADCON1 = 0x0e;        // Ref vtg is VDD & Configure pin as analog pin
    ADCON2bits.ADFM = 1;  // Right Justifie
    ADCON2bits.ADCS = 1;  // Clock conversion FOSC/8
    ADCON2bits.ACQT = 1;  // Acquition time 2 TAD
    ADRESH = 0;           // Flush ADC output Register
    ADRESL = 0;

    TRISAbits.RA0 = 1;  // analog input port

    if (isINT) {
        PIR1bits.ADIF = 0;  // Clear ADC Interrupt flag
        IPR1bits.ADIP = 1;  // Enable ADC interrupt in high priority
        PIE1bits.ADIE = 1;  // Enable ADC Interrupt
    }
}

int ADC_Read(int channel) {
    int digital;

    ADCON0bits.CHS = channel;  // Select Channe0
    ADCON0bits.ADON = 1;
    ADCON0bits.GO = 1;

    while (ADCON0bits.GO_nDONE == 1)
        ;

    digital = (ADRESH << 8) | (ADRESL);
    return (digital);
}

/**
 * @brief use this function to start ADC conversion with INT
 *
 * @param channel
 */
void AdcGO(int channel) {
    ADCON0bits.CHS = channel;  // Select Channe0
    ADCON0bits.ADON = 1;
    ADCON0bits.GO = 1;
}