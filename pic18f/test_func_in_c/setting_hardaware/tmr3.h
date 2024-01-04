#ifndef TMR3_H
#define TMR3_H

#include <stdbool.h>
#include <xc.h>

void TMR3Init(bool isINT);

void TMR3Run(int high_byte, int low_byte);

#endif /* TMR3_H */
