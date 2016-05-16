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

void interrupt interrupt_handler(void);

///
/// INITIALISE TAPPING GRID
///
void TAP_init(void)
{
    LATB = 0x03;
    TRISB = 0xF0;
    INTCON2bits.RBPU = 0;   /* Turn on weak pull-up */ 
}


void tap_feedback_enable(int enable)
{
    AUDIO_tap_enable(enable);
    p1_tap_display_enable(enable);
    p2_tap_display_enable(enable);
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
    
    PORTA = 0x00;           // Initial PORTA
    TRISA = OUTPUT;         // Define PORTA as input
    //T_SCORE = INPUT;        // Make N/C pin input
    ADCON1 = 0x0F;          // Turn off ADcon
    CMCON = 0x07;           // Turn off Comparator
    PORTC = 0x00;           // Initial PORTC
    TRISC = OUTPUT;         // Define PORTC as output
	INTCONbits.GIE = 1;     // Turn on global interrupt
    
#if PWM_CHANNELS > 0
    PWM_init();
#endif
    
    /* Initialise some pins of PORTB as input for detecting taps */
    TAP_init();
    
    /* Initalise PWM (Timer 2) and Sampler (Timer 0) for Audio */
    AUDIO_init();
    
    /* Initialise haptic feedback (active low, make them high = off) */
    FEEDBACK_init();
    
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
void interrupt interrupt_handler(void)
{
#if PWM_CHANNELS > 0
    PWM_ISR();
#endif
    
    AUDIO_ISR();
    
    INTCONbits.GIE = TRUE;
    INTCONbits.PEIE = TRUE;
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
    T3CONbits.TMR3ON = 1;   /* Turn on */
    T3CONbits.TMR3CS = 0;   /* Internal clock source  */
    T3CONbits.T3NSYNC = 1;  /* Do not synchronize */
    
    PIE2bits.TMR3IE = FALSE;
    IPR2bits.TMR3IP = TRUE;
    PIR2bits.TMR3IF = FALSE;      // Clear Timer1 overflow flag
}
//EOF-------------------------------------------------------------------------
