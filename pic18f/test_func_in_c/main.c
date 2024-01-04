#include "setting_hardaware/setting.h"
#include <stdlib.h>
#include "stdio.h"
#include "string.h"
// using namespace std;

void main(void) 
{
    
    SYSTEM_Initialize() ;
    
    unsigned char data;
    while(1) {
        data = MyUartRead();
        UART_Write(data);
    }
    return;
}

void __interrupt(high_priority) Hi_ISR(void)
{

}