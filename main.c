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
/** D E F I N E S ***************************************************/

/** P R I V A T E   V A R I A B L E S *******************************/
unsigned int counter;
unsigned int counter_servo;

/** P R I V A T E   P R O T O T Y P E S *****************************/
static void init(void);

/********************************************************************/
/** P U B L I C   D E C L A R A T I O N S ***************************/
/********************************************************************
 * Function:        void main(void)
 * PreCondition:    None
 * Input:           None
 * Output:          None
 * Overview:        Main program entry point
 ******************************************s**************************/
void main(void) {
	init();						//initialize the system
    
    SCORE_setScore(0);
	while(timed_to_1ms()) {
        
        //**** put here a reference to one or more FSM's
        //p1_tapping_fsm();
        //p2_tapping_fsm();
        if ((++counter) == 20) {
            PWM_duty[1] = (PWM_duty[1] + 2) % 200;
            counter = 0;
        }
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
static void init(void) 
{
    hardware_init(); 
}

//EOF-------------------------------------------------------------------------
