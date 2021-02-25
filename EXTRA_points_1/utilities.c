#ifndef UTILITIES
#define UTILITIES

#include "LPC17xx.H"                    /* LPC17xx definitions                */
#include "utilities.h"
#include "timer/timer.h"
#include "led/led.h"
#include "button_EXINT/button.h"

//*********************************************************************************
// dichiarazione delle variabili
//*********************************************************************************
unsigned const char labirinto[13][15] = { {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
																				 {0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
																				 {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
																				 {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
																				 {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
																				 {1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0},
																				 {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
																				 {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
																				 {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
																				 {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
																				 {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
																				 {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
																				 {2, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2} };

const unsigned char run = 5;		//LED del movimento
uint8_t posizione[2]; 	  //posizione iniziale del robot
unsigned char direzione = 0;   //direzione iniziale del robot => 
															 //3(LED 8) NORD - 2(LED 9) EST - 1(LED 10) SUD - 0(LED 11) OVEST
uint8_t frequenza = 0;		// flag che indica la frequenza a cui lampeggia il led del movimento
uint8_t vittoria = 0;			// flag che indica la fine del gioco

																	 
//*********************************************************************************
// funzioni
//*********************************************************************************

																	 
//*********************************************************************************
//verificaDistanza() calcola la distanza dal prossimo ostacolo e la ritorna												 
//*********************************************************************************
int verificaDistanza(void)
	{
  int distanza = 0;
  int i = 0;
  int x = posizione[0];
  int y = posizione[1];
    
  if (direzione == 3 || direzione == 0){  // se guarda a NORD o a OVEST
    i = -1;
  } else{
    i = 1;
  }
  
  if (direzione == 3 || direzione == 1){ //asse verticale
    while( (x+i < 13 && x+i >= 0) && labirinto[x+i][y]!= 1) {
      x = x + i;
      distanza ++;
			if (labirinto[x][y] == 2){
				return 100; //numero "flag" per non fare lampeggiare il led
			}
    }
  }
  
  if (direzione == 2 || direzione == 0){ //asse orizzontale
    while( (y+i < 15 && y+i >= 0) && labirinto[x][y+i]!= 1) {
      y = y + i;
      distanza ++;
			if (labirinto[x][y] == 2){
				return 100;
			}
    }
  }
	
	return distanza;
}

//*********************************************************************************
// blinkLedDirezione() imposta il timer1 per il corretto blink del led di direzione
//*********************************************************************************
void blinkLedDirezione(void)
	{
	int distanza = verificaDistanza();
	disable_timer(1);
	reset_timer(1);
	
	if (distanza == 0){ //8HZ
    init_timer(1, 0x0017D784);    /* 0,0625 on, 0,0625 off => 0.0625*25*10^6*/
  }
  else if (distanza == 2 || distanza == 1){ //4HZ
    init_timer(1, 0x02FAF08);    /* 0,125 on, 0,125 off*/   
  }
  else if (distanza >=3 && distanza <=5){ //2HZ
    init_timer(1, 0x005F5E10);    /* 0,25 on, 0,25 off*/      
  }
  if (distanza <= 5){ //se distanza >=6 non deve lampeggiare, non si avvia il timer
    enable_timer(1);
  }
}

//*********************************************************************************
// prossimaCasella() controlla se la prossima casella è occupabile o meno
//*********************************************************************************
int prossimaCasella()
	{
		int i;
		if (direzione == 3 || direzione == 0){  // se guarda a NORD o a OVEST
			i = -1;
		} else{
			i = 1;
		}
		
		if (direzione == 3 || direzione == 1){ //asse orizzontale
			if((posizione[0]+i < 13 && posizione[0]+i >= 0) && labirinto[posizione[0]+i][posizione [1]]!= 1) {
				return 0; //casella occupabile
			}
		}
		if (direzione == 2 || direzione == 0){ //asse verticale
			if((posizione [1]+i < 15 && posizione [1]+i >= 0) && labirinto[posizione[0]][posizione [1]+i]!= 1) {
				return 0;
			}
		}
		return 1;
	}
//*********************************************************************************
// blinkLedMovimento() imposta il timer0 per il corretto blink del led di movimento
//*********************************************************************************
void blinkLedMovimento(int occupata)
	{
	frequenza = 0;
	
	disable_timer(0);
	reset_timer(0);

	if (occupata == 0){
		frequenza = 1;
    init_timer(0, 0x00BEBC20); //il led lampeggia a 1Hz, 0.5 on, 0.5 off 0.5*25*10^6
    }
  else{
		frequenza = 5;
		init_timer(0, 0x002625A0); 			/* 0.1 secondi on, 0.1 secondi off, 0.1*25*10^6 */
	}
	
	enable_timer(0);
}
																	 
//*********************************************************************************	
// muovi() fa spostare in avanti il robot, verifica la condizione di fine del gioco
//********************************************************************************																	 
void muovi(int occupata)
{
	int i;
	
	if (occupata == 0){ //mi dice se posso spostarmi o no
		if (direzione == 3 || direzione == 0){  // se guarda a NORD o a OVEST
			i = -1;
		} else{
			i = 1;
		}
		
		if (direzione == 3 || direzione == 1){
			posizione[0] = posizione[0]+i;
		}
		
		if (direzione == 2 || direzione == 0){ 
			posizione[1] = posizione[1]+i;
		}
		blinkLedDirezione();
	}
	
	if (labirinto[posizione[0]][posizione[1]] == 2){
		vittoria = 1;															/* flag che indica la fine del gioco*/
		
		NVIC_DisableIRQ(EINT1_IRQn);    					/* disabilito le interruzioni dei bottoni*/
		NVIC_DisableIRQ(EINT2_IRQn);    				

		disable_timer(0);													/* fermo i timer e disabilito le loro interruzioni*/
		disable_timer(1);
		NVIC_DisableIRQ(TIMER0_IRQn); 		
		NVIC_DisableIRQ(RIT_IRQn);
	
		LED_Out(0);
		
		reset_timer(1);
		init_timer(1, 0x00BEBC20); 								/*i led lampeggiano a 1Hz, 0.5 on, 0.5 off 0.5*25*10^6*/
		enable_timer(1);
		
		return;
	}
}

//*********************************************************************************
// ruota() modifica la direzione del robot
//*********************************************************************************
void ruota(void)
	{ 
	LED_Off(direzione);
	if (direzione == 0){
		direzione = 3;
	}
	else{
			direzione--;
	}
	LED_On(direzione);
}

#endif
