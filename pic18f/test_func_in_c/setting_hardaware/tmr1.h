#ifndef TMR1_H
#define TMR1_H

#include <xc.h>
#include <stdbool.h>

void TMR1Init(bool isINT);

void TMR1Run(int high_byte, int low_byte);

#endif /* TMR1_H */
