/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           timer.h
** Descriptions:        Prototypes of functions included in the lib_led, funct_led .c files
** Correlated files:    lib_led.c, funct_led.c
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/

/* lib_led */
void LED_init(void);
void LED_deinit(void);

/* funct_led */
void LED_On (unsigned int num);
void LED_Off (unsigned int num);
void LED_Out(unsigned int value);

