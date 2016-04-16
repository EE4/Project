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
#include "constants.h"
#include "p1_tapping_fsm.h"
#include "p2_tapping_fsm.h"

/** D E F I N E S ***************************************************/
#define PUSHED 0

/** P R I V A T E   V A R I A B L E S *******************************/
static unsigned char on_counter, off_counter;
static enum {FSM_IDLE, 
            FSM_START, 
            FSM_LED_ON, 
            FSM_LED_OFF, 
            FSM_BTN_ONCE,
            FSM_BTN_WAIT } current_state, state_to_recover;

/** P R I V A T E   P R O T O T Y P E S *****************************/
static void init(void);
static void simple_example(void);
static void complex_example(void);

/********************************************************************/
/** P U B L I C   D E C L A R A T I O N S ***************************/
/********************************************************************
 * Function:        void main(void)
 * PreCondition:    None
 * Input:           None
 * Output:          None
 * Overview:        Main program entry point
 ********************************************************************/
void main(void) {
	init();						//initialize the system
    init_tapping();
	while(timed_to_1ms()) {
        
        //**** put here a reference to one or more FSM's
        p1_tapping_fsm();
        p2_tapping_fsm(); 
       
        PORTA = p1_pressed | (p2_pressed << 3);
	}
}

/********************************************************************/
/** P R I V A T E   D E C L A R A T I O N S *************************/
/********************************************************************
 * Function:        void init(void)
 * PreCondition:    None
 * Input:           None
 * Output:          None
 * Overview:        Init is a centralize initialization routine.              
 *                  It will initialze all IO and variables, also the
 *                  hardware_init() for the hardware functions should
 *                  be called here.              
 ********************************************************************/
static void init(void) {
	//configure I/O
	TRISCbits.TRISC0 = 1;
	TRISCbits.TRISC2 = 0;
	LATCbits.LATC2 = 0;

	current_state = FSM_IDLE;
    state_to_recover = FSM_IDLE;
	on_counter = 0;
	off_counter = 0;
	
    hardware_init(); 
}

/********************************************************************
 * Function:        simple_example(void)
 * PreCondition:    None
 * Input:           None
 * Output:          None
 * Overview:        A simple FSM that will start blinking a led on 
 *                  and off once you pushed a button.          
 ********************************************************************/
static void simple_example(void) {
    
    switch (current_state) {                
        case FSM_IDLE :
        // *** outputs ***
            on_counter = 0;
        off_counter = 0;
            LATCbits.LATC2 = 0;
        // *** transitions ***
            if (PORTCbits.RC0 == PUSHED) current_state = FSM_LED_ON;
            break;

        case FSM_LED_ON :
        // *** outputs ***
            LATCbits.LATC2 = 1;
            off_counter = 0;
            on_counter = on_counter + 1;
        // *** transitions ***
            if (on_counter == 200) current_state = FSM_LED_OFF;
            break;

        case FSM_LED_OFF :
        // *** outputs ***
            LATCbits.LATC2 = 0;
            on_counter = 0;
            off_counter = off_counter + 1;
        // *** transitions ***
            if (off_counter == 200) current_state = FSM_LED_ON;
            break;

        default:
            current_state = FSM_IDLE;
            break;
    }
    
}

/********************************************************************
 * Function:        void complex_example(void)
 * PreCondition:    None
 * Input:           None
 * Output:          None
 * Overview:        A more complex example example where also a led
 *                  starts blinking, but also every time the button
 *                  is pushed a servo is changing its angle.
 ********************************************************************/
static void complex_example(void) {
    
    switch (current_state) { 
        case FSM_IDLE :
        // *** outputs ***
            // Statement 1
            // Statement 2
        // *** transitions ***
            if (PORTCbits.RC0 == PUSHED) {
                /* Can we call actions here? */
                /* Statement (Will only be called once) */
                current_state = FSM_START;
            }
            break;

        case FSM_START :
        // *** outputs ***
            on_counter = 0;
            off_counter = 0;
            LATCbits.LATC2 = 0;
            SERVO_setPosition(0,1);
        // *** transitions ***
            if (PORTCbits.RC0 != PUSHED) current_state = FSM_LED_ON;
            break;

        case FSM_LED_ON :
        // *** outputs ***
            LATCbits.LATC2 = 1;
            off_counter = 0;
            on_counter = on_counter + 1;
            state_to_recover =  FSM_LED_ON;
        // *** transitions ***
            if (on_counter == 200) current_state = FSM_LED_OFF;
            if (PORTCbits.RC0 == PUSHED) current_state = FSM_BTN_ONCE;
            break;

        case FSM_LED_OFF : 
        // *** outputs ***
            LATCbits.LATC2 = 0;
            on_counter = 0;
            off_counter = off_counter + 1;
            state_to_recover =  FSM_LED_OFF;
        // *** transitions ***
            if (off_counter == 200) current_state = FSM_LED_ON;
            if (PORTCbits.RC0 == PUSHED) current_state = FSM_BTN_ONCE;
            break;

        case FSM_BTN_ONCE : 
        // *** outputs ***
            servo_setpoint[0]++;
        // *** transitions ***
            current_state = FSM_BTN_WAIT;
            break;

        case FSM_BTN_WAIT : 
        // *** outputs ***
        // *** transitions ***
            if (servo_setpoint[0] > 32) current_state = FSM_START;
            if (PORTCbits.RC0 != PUSHED) current_state = state_to_recover;
            break;

        default:
            current_state = FSM_IDLE;
            break;
    }
    
}
//EOF-------------------------------------------------------------------------
