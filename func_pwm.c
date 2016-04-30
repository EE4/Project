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
#include "func_pwm.h"

/** P U B L I C   V A R I A B L E S *********************************/
// in order for the variable to be used in other file, it also has to
// be declared as 'extern' in the corresponding .h file
unsigned short PWM_duty[PWM_CHANNELS];

/** P R I V A T E   V A R I A B L E S *******************************/
// 'static' implies that the variable can only be used in this file
// (cfr. 'private' in Java)
static unsigned short servo_counter;
static unsigned short mode_counter;

/** P R I V A T E   P R O T O T Y P E S *****************************/
// 'static' implies that the function can only be used in this file
// (cfr. 'private' in Java)
static void OpenTimer1(unsigned char intEnable);

/********************************************************************/
/** P U B L I C   D E C L A R A T I O N S ***************************/
/********************************************************************
 * Function:        void PWM_init(void)
 * PreCondition:    None
 * Input:           None
 * Output:          None
 * Overview:        Initializes all output channels for the PWM process
 ********************************************************************/
void PWM_init(void) {
    
    /* Initialise counters */
	servo_counter = 0;
    mode_counter = 0;
    
    /* Initialise duty cycles */
	for(int i = 0; i < PWM_CHANNELS; i++) {
        PWM_duty[i] = 0;
    }
    
    /* Set-up interrupt */
    OpenTimer1(TRUE);
	
}

/********************************************************************
 * Function:        void PWM_ISR(void)
 * PreCondition:    PWM_init()
 * Input:           None
 * Output:          None
 * Overview:        PWM Interrupt service routine will process all PWM
 *                  duty cycles and set the outputs accordingly
 ********************************************************************/
void PWM_ISR(void) {
    if (PIR1bits.TMR1IF == 1) {
        /* Tuned to 20kHz interrupt frequency */
        TMR1H = 0xFD; 
        TMR1L = 0xA7;
        
        /* Tuned to 50Hz PWM-frequency with a resolution of 400, this
         * is fixed by the 400 and the interrupt frequency, do not change */
        servo_counter = (servo_counter + 1) % 400;
        
        /* Tuned to 100Hz PWM-frequency with a resolution of 200, this
         * is fixed by the 200 and the interrupt frequency, do not change */
        mode_counter = (mode_counter + 1) % 200;
        
        /* Toggle servo output on the duty cycle of the servo_counter */
        D_SERVO = (PWM_duty[0] > servo_counter);
        
        /* Toggle mode leds on the duty cycle of the mode_counter */
        if (mode_idle) {
            D_MODE1 = (PWM_duty[1] > mode_counter);
            D_MODE2 = (PWM_duty[1] > mode_counter);
            D_MODE3 = (PWM_duty[1] > mode_counter);
        }
        
        PIR1bits.TMR1IF = 0;
    }
}

/********************************************************************/
/** P R I V A T E   D E C L A R A T I O N S *************************/
/********************************************************************
 * Function:        void OpenTimer1(unsigned char intEnable)
 * PreCondition:    None
 * Input:           intEnable: enable Timer1 interrupt
 * Output:          None
 * Overview:        Will initialize Timer1 given the parameters
 ********************************************************************/
static void OpenTimer1(unsigned char intEnable) {
    T1CONbits.RD16 = 0;
    T1CONbits.TMR1CS = 0;
    T1CONbits.TMR1ON = 1;

    /* Tuned to 20kHz interrupt frequency */
    TMR1H = 0xFD;
    TMR1L = 0xA7;
    
    PIE1bits.TMR1IE = intEnable & 0x01;
    INTCONbits.GIE = (intEnable & 0x01) | INTCONbits.GIE;
    INTCONbits.PEIE = (intEnable & 0x01) | INTCONbits.PEIE;
    IPR1bits.TMR1IP = 0;
    PIR1bits.TMR1IF = 0;      // Clear Timer1 overflow flag
}
//EOF-----------------------------------------------------------------
