#ifndef _PIN_MANAGER_H
#define _PIN_MANAGER_H

#include <xc.h>

#define Clean_AllLEDSignal()    LATD = 0

#define LED1_SetHigh()          LATDbits.LATD0 = 1
#define LED1_SetLow()           LATDbits.LATD0 = 0
#define LED1_Toggle()           LATDbits.LATD0 ^= 1
#define LED1_GetValue()         RD0
#define LED1_SetDigitalInput()  TRISDbits.TRISD0 = 1
#define LED1_SetDigitalOutput() TRISDbits.TRISD0 = 0

#define LED2_SetHigh()          LATDbits.LATD1 = 1
#define LED2_SetLow()           LATDbits.LATD1 = 0
#define LED2_Toggle()           LATDbits.LATD1 ^= 1
#define LED2_GetValue()         RD1
#define LED2_SetDigitalInput()  TRISDbits.TRISD1 = 1
#define LED2_SetDigitalOutput() TRISDbits.TRISD1 = 0

#define LED3_SetHigh()          LATDbits.LATD2 = 1
#define LED3_SetLow()           LATDbits.LATD2 = 0
#define LED3_Toggle()           LATDbits.LATD2 ^= 1
#define LED3_GetValue()         RD2
#define LED3_SetDigitalInput()  TRISDbits.TRISD2 = 1
#define LED3_SetDigitalOutput() TRISDbits.TRISD2 = 0

#define LED4_SetHigh()          LATDbits.LATD3 = 1
#define LED4_SetLow()           LATDbits.LATD3 = 0
#define LED4_Toggle()           LATDbits.LATD3 ^= 1
#define LED4_GetValue()         RD3
#define LED4_SetDigitalInput()  TRISDbits.TRISD3 = 1
#define LED4_SetDigitalOutput() TRISDbits.TRISD3 = 0

// void PIN_MANAGER_Initialize(void);
#endif