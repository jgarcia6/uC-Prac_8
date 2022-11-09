/* Reaction time game */
#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include “myTimer.h”
#include “myUart.h”

// UART 0
#define PC_UART_PORT    (0)
#define PC_UART_RX_PIN  (3)
#define PC_UART_TX_PIN  (1)

#define CLOCK_X_POS     5
#define CLOCK_Y_POS     5

struct
{
    uint32_t seconds:8;
    uint32_t minutes:8;
    uint32_t hours:8;
}clockTime;


void clockInit(uint8_t hh, uint8_t mm, uint8_t ss)
{
    clockTime.hours = hh;
    clockTime.minutes = mm;
    clockTime.seconds = ss;
}

void clockUpdate(void)
{
    // TODO Fixme: Add logic to count hours, minutes and seconds
    clockTime.seconds++;
}

void clockDisplay(void)
{
    char str[10];
    uartGotoxy(0,CLOCK_X_POS,CLOCK_Y_POS);
    // TODO Fix me: Add HH:MM:SS print format
    myItoa(clockTime.seconds,cad,10);
    uartPuts(0,cad);
}

int app_main()
{
    
    uartInit(PC_UART_PORT, 57600, 8, 1, 2, PC_UART_TX_PIN,PC_UART_RX_PIN);
    delayMs(500);
    uartClrScr(PC_UART_PORT);

    timer0Init();
    clockInit(23,59,50);

    while(1){ 
        if( timer0SecFlag() ) 
        { 
            clockUpdate();
            clockDisplay();
        }

        // Prevent watchdog fault
        delayMs(10);
    } 
    return 0; 
}