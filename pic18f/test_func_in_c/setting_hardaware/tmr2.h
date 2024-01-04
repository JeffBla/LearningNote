#ifndef TMR2_H
#define TMR2_H

#include <xc.h>
#include <stdbool.h>

void TMR2Init(bool isINT);

void TMR2Run(int thres);

#endif /* TMR2_H */
