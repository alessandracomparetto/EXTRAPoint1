#include "button.h"
#include "lpc17xx.h"
#include "../led/led.h"
#include "../timer/timer.h"
#include "../RIT/RIT.h"	
#include "../utilities.h"

extern uint8_t posizione[];
extern unsigned char direzione;
extern const unsigned char run;
uint8_t GPIO1 = 0; // flag che consentono di eseguire correttamente l'azione nel RIT_IRQHandler
uint8_t GPIO2 = 0;

//*********************************************************************************
// Handler del tasto INT0, per avviare il gioco
//*********************************************************************************
void EINT0_IRQHandler (void)
{		
	NVIC_ClearPendingIRQ(EINT1_IRQn);        // cencello le evenutuali interruzioni pendenti degli altri pulsanti
  NVIC_ClearPendingIRQ(EINT2_IRQn);  
  
  NVIC_EnableIRQ(EINT1_IRQn);              // abilito le interruzioni e imposto le priorità 
  NVIC_SetPriority(EINT1_IRQn,1);         
  NVIC_EnableIRQ(EINT2_IRQn);              
  NVIC_SetPriority(EINT2_IRQn,1);         
	
  posizione[0] = 7;													//posizione iniziale, casella (7, 7)
	posizione[1] = 7;
	direzione = 2; 														//direzioen EST
	
	LED_On(direzione); 												// accendo il led direzione EAST
	blinkLedDirezione();											// calcolo della distanza e blink del led direzione
	
  NVIC_DisableIRQ(EINT0_IRQn);    					// disabilito le interruzione di INT0
	LPC_SC->EXTINT &= (1 << 0);    						/* clear pending interrupt        */
  LPC_SC->EXTINT &= (1 << 1);     					/* clear pending interrupt         */
  LPC_SC->EXTINT &= (1 << 2);     					/* clear pending interrupt         */    
}

//*********************************************************************************
// Handler del tasto KEY1, per ruotare il robot
//*********************************************************************************
void EINT1_IRQHandler (void)	  	
{
	GPIO1 = 1;																
	enable_RIT();															// enable RIT to count 50ms				 
	LPC_PINCON->PINSEL4    &= ~(1 << 22);     // GPIO pin selection 
	NVIC_DisableIRQ(EINT1_IRQn);							// disable Button interrupts			
  LPC_SC->EXTINT &= (1 << 1);     
}

//*********************************************************************************
// Handler del tasto KEY2, per muovere il robot
//*********************************************************************************
void EINT2_IRQHandler (void)
{
	GPIO2 = 1;
	enable_RIT();                   				  // enable RIT to count 100ms
	LPC_PINCON->PINSEL4    &= ~(1 << 24);			// GPIO pin selection 
  NVIC_DisableIRQ(EINT2_IRQn);     					// disable Button interrupts											 
	LPC_SC->EXTINT &= (1 << 2);      
}

