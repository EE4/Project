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
static unsigned char seed_time; /* Value to seed */
static unsigned long long time_ms;

unsigned int counter;

//===----------------------------------------------------------------------===//
//  GLOBAL PUBLIC VARIABLES
//===----------------------------------------------------------------------===//

/* Global game variables */
unsigned char round;
bool game_ended;

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
    
	while(timed_to_1ms()) {
        RAND_seed(seed_time++); /* Try to seed the RNG - seed_time can overflow, 
                                 * but we don't care #YOLO */

        /* Main game FSM */
        general_fsm();
        
        p1_tapping_fsm();
        p2_tapping_fsm();
        
        feedback_fsm();
        
        display_pattern_fsm();
        
        LEDS_tick();
        
        mode1_fsm();
        mode3_fsm();
        
        AUDIO_tap(p1_pressed);
        AUDIO_tap(p2_pressed);
        
        if (time_ms++ < 2) {
            FEEDBACK_giveFeedback(PLAYER_1);
            FEEDBACK_giveFeedback(PLAYER_2);
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
    
    /* Lower level drivers initialisation */
    p1_tapping_fsm_init();
    p2_tapping_fsm_init();
    feedback_fsm_init();
    
    general_fsm_init();
    
    mode1_fsm_init();
    mode3_fsm_init();
    
    display_pattern_fsm_init();
}

//EOF-------------------------------------------------------------------------
