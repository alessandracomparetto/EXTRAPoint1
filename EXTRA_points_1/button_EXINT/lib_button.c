#include "button.h"
#include "lpc17xx.h"

/**
 * @brief  Function that initializes Buttons
 */
void BUTTON_init(void) {

  LPC_PINCON->PINSEL4    |= (1 << 20);		 /* External interrupt 0 pin selection */
  LPC_GPIO2->FIODIR      &= ~(1 << 10);    /* PORT2.10 defined as input          */

  LPC_PINCON->PINSEL4    |= (1 << 22);     /* External interrupt 0 pin selection */
  LPC_GPIO2->FIODIR      &= ~(1 << 11);    /* PORT2.11 defined as input          */
  
  LPC_PINCON->PINSEL4    |= (1 << 24);     /* External interrupt 0 pin selection */
  LPC_GPIO2->FIODIR      &= ~(1 << 12);    /* PORT2.12 defined as input          */

  LPC_SC->EXTMODE = 0x7;										/* buttons are edge sesitive*/
	LPC_SC->EXTPOLAR = 0x2; 									/* falling edge per KEY 2 and INT 0, raising for KEY 1*/
	
	NVIC_ClearPendingIRQ(EINT0_IRQn);   
  NVIC_EnableIRQ(EINT0_IRQn);              /* enable IRQ for INT0                 */
	NVIC_SetPriority(EINT0_IRQn, 3);				 /* priorità minore degli altri bottoni */
}
