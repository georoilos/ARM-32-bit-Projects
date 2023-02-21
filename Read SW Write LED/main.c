/*  p2_7.c: Read a switch and write it to the LED */

/*  This program reads SW1 of Tiva LaunchPad and writes the inverse of the value to the green LED. SW1 is low when pressed (Normally High). LED is on when high. */

#include "TM4C123GH6PM.h"

int main(void)
{
    unsigned int value;
    SYSCTL->RCGCGPIO |= 0x20;   /* enable clock to GPIOF */
    GPIOF->DIR = 0x08;          /* set PORTF3 pin as output (LED) pin */
                                /* and PORTF4 as input, SW1 is on PORTF4 */
    GPIOF->DEN = 0x18;          /* set PORTF pins 4-3 as digital pins */
    GPIOF->PUR = 0x10;          /* enable pull up for pin 4 */

    while(1)
    {   
        value = GPIOF->DATA;    /* read data from PORTF */
        value = ~value;         /* switch is low active; LED is high active */
        value = value >> 1;     /* shift it right to display on green LED */
        GPIOF->DATA = value;    /* put it on the green LED */
    }
}

/* This function is called by the startup assembly code to perform system specific initialization tasks. */

void SystemInit(void)
{
    /* Grant coprocessor access */
    /* This is required since TM4C123G has a floating point coprocessor */
    SCB->CPACR |= 0x00F00000;
} 