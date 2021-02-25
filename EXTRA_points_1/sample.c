#include "LPC17xx.H"                    /* LPC17xx definitions                */
#include "led/led.h"
#include "button_EXINT/button.h"
#include "timer/timer.h"
#include "RIT/RIT.h"
#include "./utilities.h"

#ifdef SIMULATOR
extern uint8_t ScaleFlag;
#endif
/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
int main (void) {
	
	SystemInit();  												
  LED_init();                           
  BUTTON_init();												
	
	init_RIT(0x004C4B40);									/* RIT inizializzazione a 50msec => debouncing dei tasti*/
																		 
	LPC_SC->PCON |= 0x1;									/* power-down	mode										*/
	LPC_SC->PCON &= ~(0x2);						
		
  while (1) {                           /* Loop forever                       */	
		__ASM("wfi");
  }
}
