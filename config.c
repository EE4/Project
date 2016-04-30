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
static void OpenTimer3(unsigned char intEnable);

///
/// INITIALISE TAPPING GRID
///
void HAPTIC_init(void)
{
    D_HAP_P1 = 0;
    D_HAP_P2 = 0;
    T_HAP_P1 = OUTPUT;
    T_HAP_P2 = OUTPUT;
}

///
/// INITIALISE TAPPING GRID
///
void TAP_init(void)
{
    LATB = 0x00;
    TRISB = 0xF0;
    INTCON2bits.RBPU = 0;   /* Turn on weak pull-up */ 
}

void MODE_init(void)
{
    D_MODE1 = 0;
    D_MODE2 = 0;
    D_MODE3 = 0;
    T_MODE1 = OUTPUT;
    T_MODE2 = OUTPUT;
    T_MODE3 = OUTPUT;
}

void SERVO_init(void)
{
    D_SERVO = 0;
    T_SERVO = OUTPUT;
}

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
    
#if PWM_CHANNELS > 0
    PWM_init();
#endif
    
    SERVO_init();
    
    MODE_init();
    
    /* Initialise PORTB for detecting taps */
    TAP_init();
    
    /* Initalise PWM (Timer 2) and Sampler (Timer 0) for Audio */
    AUDIO_init();
    
    /* Initialise haptic feedback */
    HAPTIC_init();
    
    /* Initialise SIPO chain */
    LEDS_init();
    
	OpenTimer3(FALSE);
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
	while (!PIR2bits.TMR3IF)
    {
        /* Do nothing while interrupt flag is low, blocking function */
        NOP();
    };
    
    TMR3H = 0xD1;   /* First high byte */
    TMR3L = 0xDF;   /* Then low, single operation */
    
	PIR2bits.TMR3IF = 0;
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
#if PWM_CHANNELS > 0
    PWM_ISR();
#endif
    
    AUDIO_ISR();
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
static void OpenTimer3(unsigned char intEnable) 
{
    T3CONbits.T3RD16 = 1;   /* Read write in a single 16-bit operation */
    
    TMR3H = 0xD1;   /* First high byte */
    TMR3L = 0xDF;   /* Then low, single 16-bit operation */
    
    T3CONbits.T3CKPS = 0b00;/* No postscaler */
    T3CONbits.TMR3ON = 1;   /* Turn on  */
    T3CONbits.TMR3CS = 0;   /* Internal clock source  */
    T3CONbits.T3NSYNC = 1;  /* Do not synchronize */
    
    PIE2bits.TMR3IE = intEnable & 0x01;
    INTCONbits.GIE = (intEnable & 0x01) | INTCONbits.GIE;
    INTCONbits.PEIE = (intEnable & 0x01) | INTCONbits.PEIE;
    IPR2bits.TMR3IP = 0;
    PIR2bits.TMR3IF = 0;      // Clear Timer1 overflow flag
}
//EOF-------------------------------------------------------------------------
