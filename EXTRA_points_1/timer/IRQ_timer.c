#include "lpc17xx.h"
#include "timer.h"
#include "../led/led.h"
#include "../utilities.h"

/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
extern unsigned char led_value;					/* defined in funct_led								*/

extern unsigned char direzione;
extern unsigned char run;
extern uint8_t vittoria;
extern uint8_t frequenza;

int c = 0;		//contatore dei cicli eseguiti dal timer 0
int casella; //settata dal RIT inizialmente 

//*********************************************************************************
// TIMER0 viene usato per blink del led e movimento
//*********************************************************************************
void TIMER0_IRQHandler (void)
{
	c++;
	LPC_GPIO2->FIOPIN ^= (1<<run); /* Toggle the LED (P2_0) */

	if ((frequenza == 5 && c == 10) || (frequenza == 1 && c == 2)){
		c = 0;
		muovi(casella);
		casella = prossimaCasella();
		if(vittoria == 0){
			blinkLedMovimento(casella);
		}
	}
	
  LPC_TIM0->IR = 1;			/* clear interrupt flag */
  return;
}

//*********************************************************************************
// TIMER1 viene usato per fare lampeggiare i led di direzione
//        quando è verificata la condizione del termine del gioco, si occupa di 
//					fare lampeggiare tutti i LED
//*********************************************************************************
void TIMER1_IRQHandler (void)
{
	if (vittoria == 0){
		LPC_GPIO2->FIOPIN ^= (1<<direzione); /* Toggle the LED (P2_0) */
	} else{ // blink di tutti i led
		if (led_value == 0){
			LED_Out(47);
		}else{
			LED_Out(0);
		}
	}
  LPC_TIM1->IR = 1;			/* clear interrupt flag */
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
