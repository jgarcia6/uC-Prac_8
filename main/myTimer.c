#include <driver/timer.h>
#include "myTimer.h"
 
static volatile uint8_t SecFlag; 

#define ALARM_VAL_US    10000 // TODO Fixme for 1 sec
#define TIMER_DIVIDER   10000 // TODO Fixme for 1 sec

/* Timer interrupt service routine */
static void IRAM_ATTR timer0Isr(void *ptr)
{
    timer_group_clr_intr_status_in_isr(TIMER_GROUP_0, TIMER_0);
    timer_group_enable_alarm_in_isr(TIMER_GROUP_0, TIMER_0);
    SecFlag = 1;
}

/* Timer group0 TIMER_0 initialization */
static void timer0Init(void)
{
    // TODO Fixme : Use only Low level register 
    esp_err_t ret;
    timer_config_t config = {
        .divider = TIMER_DIVIDER,
        .counter_dir = TIMER_COUNT_UP,
        .counter_en = TIMER_START,
        .alarm_en = TIMER_ALARM_EN,
        .intr_type = TIMER_INTR_LEVEL,
        .auto_reload = 1,
    };
    ret = timer_init(TIMER_GROUP_0, TIMER_0, &config);
    ret = timer_set_counter_value(TIMER_GROUP_0, TIMER_0, 0x00000000ULL);
    ret = timer_set_alarm_value(TIMER_GROUP_0, TIMER_0, ALARM_VAL_US);
    ret = timer_enable_intr(TIMER_GROUP_0, TIMER_0);

    /* Register an ISR handler */
    /* Low level manipulation of the interrupt vector is too tedious, do not remove the following line */
    timer_isr_register(TIMER_GROUP_0, TIMER_0, timer0Isr, NULL, 0, NULL);
}

uint8_t timer0SecFlag ( void )
{
   if( SecFlag )
   {
        SecFlag=0;
        return 1;
    }
    else
    { 
        return 0;
   }
}