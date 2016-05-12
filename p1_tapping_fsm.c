/*
 * File:   fsm_tap_p1.c
 * Author: jelle
 *
 * Created on April 12, 2016, 3:27 PM
 */

/** I N C L U D E S *************************************************/
#include "p1_tapping_fsm.h"

/** D E F I N E S ***************************************************/
#define NOTHING_PRESSED (0x0Fu)  /* NO FINGER PRESSED */
#define P1_IDLE         NOTHING_PRESSED
#define P1_F1_PRESSED   (0x07u)  /* INDEX FINGER    */
#define P1_F2_PRESSED   (0x0Bu)  /* MIDDLE FINGER   */
#define P1_F3_PRESSED   (0x0Du)  /* RING FINGER     */
#define P1_F4_PRESSED   (0x0Eu)  /* PINKY FINGER    */
#define P1_F1_RELEASED  (0xF8u)  /* _INDEX  (Not a valid sample) */
#define P1_F2_RELEASED  (0xF4u)  /* _MIDDLE (Not a valid sample) */
#define P1_F3_RELEASED  (0xF2u)  /* _RING   (Not a valid sample) */
#define P1_F4_RELEASED  (0xF1u)  /* _PINKY  (Not a valid sample) */

/** P R I V A T E   V A R I A B L E S *******************************/
static unsigned char p1_sample_due = SAMPLE_PERIOD;
static unsigned char state = P1_IDLE;

/** P U B L I C   V A R I A B L E S *********************************/
unsigned char p1_pressed = 0;

static void fsm_p1_precondition(void)
{
    /* When the 2 players press at the same time, a short may occur
     * through the path going from the active high control line to 
     * the active low control line, like so:
     *                 |
     *          >>>>>>>|>v        |          |          |
     *          ^ +----+ v   +----+     +----+     +----+
     *     >>>>>> |    | v  \     |    \     |    \     |
     * '1' -------+----|-v---+----|-----+----|-----+----|
     *                 | v        |          |          |
     *          <<<<<<<|<<        |          |          |
     *          v +----+     +----+     +----+     +----+
     *     <<<<<< |    |    \     |    \     |    \     |
     * '0' -------+----|-----+----|-----+----|-----+----|
     *
     * To overcome this problem we configure the control line of 
     * the active low control line as an input, so it doesn't 
     * matter. The value of the control line is inherentely lost
     * by shifting to the right below so we don't have to take
     * this into account.
     */
    T_CTRL_P2 = INPUT;
    
    /* Clear the control line of player 1 */
    D_CTRL_P1 = 0;
}

static void fsm_p1_postcondition(void)
{
    /* Restore control lines */
    D_CTRL_P1 = 1;
    D_CTRL_P2 = 1;
    
    /* Make player 2 control line output again  */
    T_CTRL_P2 = OUTPUT;
}

/********************************************************************/
/** P R I V A T E   D E C L A R A T I O N S *************************/
static unsigned char fsm_p1_sample(void)
{
    unsigned char sample = 0;
    
    fsm_p1_precondition();
    
    /* Right justified value: inputs are at RB7:RB4 */
    sample = PORTB >> 4;
    
    fsm_p1_postcondition();
    
    return sample;
}

void p1_tapping_fsm_init(void)
{
    state = P1_IDLE;
    p1_sample_due = SAMPLE_PERIOD;
    p1_pressed = 0;
}

/********************************************************************/
/** P U B L I C   D E C L A R A T I O N S ***************************/
void p1_tapping_fsm(void)
{
    if (!(--p1_sample_due)) { /* Only execute this FSM every 20ms */
        /* Reset timer */
        p1_sample_due = SAMPLE_PERIOD;
        
        /* Actual FSM */
        switch(state) {
            case P1_IDLE:
            // *** outputs ***
                p1_pressed = NONE;
            // *** transitions ***
                state = fsm_p1_sample();
                break;
            case P1_F1_PRESSED:
            // *** outputs ***
                
            // *** transitions ***
                if (P1_F1_PRESSED != fsm_p1_sample())
                    state = P1_F1_RELEASED;
                break;
            case P1_F2_PRESSED:
            // *** outputs ***

            // *** transitions ***
                if (P1_F2_PRESSED != fsm_p1_sample())
                    state = P1_F2_RELEASED;
                break;
            case P1_F3_PRESSED:
            // *** outputs ***

            // *** transitions ***
                if (P1_F3_PRESSED != fsm_p1_sample())
                    state = P1_F3_RELEASED;
                break;
            case P1_F4_PRESSED:
            // *** outputs ***
                
            // *** transitions ***
                if (P1_F4_PRESSED != fsm_p1_sample())
                    state = P1_F4_RELEASED;
                break;
            case P1_F1_RELEASED:
            // *** outputs ***
                p1_pressed = INDEX;
                PATTERN_setPattern(PLAYER_1, PATTERN_INDEX);
                LEDS_update();
            // *** transitions ***
                state = P1_IDLE;    /* Unconditionally go back to IDLE */  
                break;
            case P1_F2_RELEASED:
            // *** outputs ***
                p1_pressed = MIDDLE;
                PATTERN_setPattern(PLAYER_1, PATTERN_MIDDLE);
                LEDS_update();
            // *** transitions ***
                state = P1_IDLE;    /* Unconditionally go back to IDLE */  
                break;
            case P1_F3_RELEASED:
            // *** outputs ***
                p1_pressed = RING;
                PATTERN_setPattern(PLAYER_1, PATTERN_RING);
                LEDS_update();
            // *** transitions ***
                state = P1_IDLE;    /* Unconditionally go back to IDLE */  
                break;
            case P1_F4_RELEASED:
            // *** outputs ***
                p1_pressed = PINKY;
                PATTERN_setPattern(PLAYER_1, PATTERN_PINKY);
                LEDS_update();
            // *** transitions ***
                state = P1_IDLE;    /* Unconditionally go back to IDLE */  
                break;
            default:
                state = P1_IDLE;
        }
    } else {
        p1_pressed = NONE;
    }
}
