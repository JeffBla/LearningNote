#ifndef _ADC_H
#define _ADC_H

#include <stdbool.h>
#include <xc.h>

void ADC_Initialize(bool isINT);

int ADC_Read(int channel);

#endif
