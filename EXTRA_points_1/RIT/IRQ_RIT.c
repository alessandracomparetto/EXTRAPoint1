#include "lpc17xx.h"
#include "RIT.h"
#include "../led/led.h"
#include "../timer/timer.h"
#include "../utilities.h"
#include "../button_EXINT/button.h"

extern unsigned char direzione;
extern uint8_t GPIO1;
extern uint8_t GPIO2;
extern int c;
extern int casella;
extern const unsigned char run;

/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler, used for debouncing
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

void RIT_IRQHandler (void)
{			
	static int down = 0;	
	down++; 
	
	if( GPIO1 ==1 && (LPC_GPIO2->FIOPIN & (1<<11)) != 0){ //Tasto KEY1 rilasciato
		reset_RIT();
		if (down == 3){ //viene effettuata la rotazione del robot e il calcolo della distanza dal prossimo ostacolo
			ruota();
			blinkLedDirezione();  
		}
	}
	
	if(GPIO2 == 1 && (LPC_GPIO2->FIOPIN & (1<<12))== 0){ //Tasto KEY 2 premuto
		reset_RIT();
		if (down == 3){
				casella = prossimaCasella();
				blinkLedMovimento(casella); 		//inizio a fare lampeggiare il led del movimento
				LPC_TIM0->TC=LPC_TIM0->MR0;			//lancio la prima interruzione
		}
	}
	else if (down > 3) {	/* tasti rilasciati */
			down=0;			
			disable_RIT();
			reset_RIT();			
		if (GPIO1==1){
			LPC_SC->EXTPOLAR = 0x0;
			LPC_PINCON->PINSEL4    |= (1 << 22);     /* External interrupt 0 pin selection */
			LPC_SC->EXTPOLAR = 0x2;
			GPIO1 = 0;
			NVIC_EnableIRQ(EINT1_IRQn);							 /* disable Button interrupts			*/
		
		}
		if (GPIO2==1){	 //nel caso in cui fosse stato premuto KEY2
			LPC_PINCON->PINSEL4    |= (1 << 24);     /* External interrupt 0 pin selection*/
			GPIO2 = 0;
			NVIC_EnableIRQ(EINT2_IRQn);							 /* disable Button interrupts			*/
			disable_timer(0); 											//fermo il timer del movimento
			reset_timer(0);
			c = 1; //resetto contatore del blink
			LED_Off(run); 													// spengo il led
		}
	}

  LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
	
  return;
}
