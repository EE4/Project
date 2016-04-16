/*********************************************************************
 *
 *                  EE 4 Project - Code Template
 *
 *********************************************************************
 * Processor:       PIC18F2550
 * Compiler:        XC8 1.35+
 * Author:          Jeroen Van Aken
 * Updated:         17/02/2016
 ********************************************************************/

/** I N C L U D E S *************************************************/
#include "config.h"

/** P U B L I C   V A R I A B L E S *********************************/
// in order for the variable to be used in other file, it also has to
// be declared as 'extern' in the corresponding .h file


/** P R I V A T E   V A R I A B L E S *******************************/
// 'static' implies that the variable can only be used in this file
// (cfr. 'private' in Java)

 
/** P R I V A T E   P R O T O T Y P E S *****************************/
// 'static' implies that the function can only be used in this file
// (cfr. 'private' in Java)
static void OpenTimer0(unsigned char intEnable);

/********************************************************************/
/** P U B L I C   D E C L A R A T I O N S ***************************/
/********************************************************************
 * Function:        void hardware_init(void)
 * PreCondition:    None
 * Input:           None
 * Output:          None
 * Overview:        Initialize all the needed hardware IO and timers
 ********************************************************************/
void hardware_init(void) {
    ADCON1 = 0x0F;      //set all AD to digital
#if SERVO_CHANNELS > 0
    SERVO_init();
#endif
#if ADC_CHANNELS > 0
    ADC_init();
#endif
#if PWM_CHANNELS > 0
    PWM_init();
#endif
	OpenTimer0(FALSE);
}

void init_tapping(void)
{
    LATB = 0x00;
    TRISB = 0xF0;
    INTCON2bits.RBPU = 0;   /* Turn on weak pull-up */ 
    PORTA = 0x00;   // Initial PORTA
    TRISA = 0x00;   // Define PORTA as output
    ADCON1 = 0x0F;  // Turn off ADcon
}

/********************************************************************
 * Function:        void timed_to_1ms(void)
 * PreCondition:    OpenTimer0(FALSE)
 * Input:           None
 * Output:          None
 * Overview:        Stays in a while loop, doing NOP until Timer0 
 *                  generates an overflow
 ********************************************************************/
unsigned char timed_to_1ms(void) {
	while(!INTCONbits.TMR0IF){Nop();};
    TMR0L = 69;     // tuned to 1ms
	INTCONbits.TMR0IF = 0;
    return 1;
}	

/********************************************************************
 * Function:        void interrupt_handler(void)
 * PreCondition:    GIE need to be set
 * Input:           None
 * Output:          None
 * Overview:        This routine is called whenever an interrupt 
 *                  condition is  reached
 ********************************************************************/	
void interrupt interrupt_handler(void) {
#if SERVO_CHANNELS > 0
    SERVO_ISR();
#endif
#if ADC_CHANNELS > 0
    ADC_ISR();
#endif
#if PWM_CHANNELS > 0
    PWM_ISR();
#endif
}

/********************************************************************/
/** P R I V A T E   D E C L A R A T I O N S *************************/
/********************************************************************
 * Function:        void OpenTimer0(unsigned char intEnable)
 * PreCondition:    None
 * Input:           intEnable: enable Timer0 interrupt
 * Output:          None
 * Overview:        Will initialize Timer0 given the parameters
 ********************************************************************/
static void OpenTimer0(unsigned char intEnable) {
    T0CONbits.T08BIT = 1;
    T0CONbits.T0CS = 0;
    T0CONbits.PSA = 0;
    T0CONbits.T0PS = 0b101;
    TMR0H = 0;                // Reset Timer0 to 0x0000
    TMR0L = 69;
    
    INTCONbits.TMR0IE = intEnable & 0x01;
    INTCONbits.GIE = (intEnable & 0x01) | INTCONbits.GIE;
    INTCONbits.TMR0IF = 0;      // Clear Timer0 overflow flag
}
//EOF-------------------------------------------------------------------------
