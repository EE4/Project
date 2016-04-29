/*
 * File:   fsm_tap_p2.c
 * Author: jelle
 *
 * Created on April 12, 2016, 3:27 PM
 */

/** I N C L U D E S *************************************************/
#include "p2_tapping_fsm.h"

/** D E F I N E S ***************************************************/
#define NOTHING_PRESSED (0x0Fu)  /* NO FINGER PRESSED */
#define P2_IDLE         NOTHING_PRESSED
#define P2_F1_PRESSED   (0x0Eu)  /* RING FINGER     */
#define P2_F2_PRESSED   (0x0Du)  /* MIDDLE FINGER   */
#define P2_F3_PRESSED   (0x0Bu)  /* RING FINGER     */
#define P2_F4_PRESSED   (0x07u)  /* PINKY FINGER    */
#define P2_F1_RELEASED  (0xF1u)  /* _RING   (Not a valid sample) */
#define P2_F2_RELEASED  (0xF2u)  /* _MIDDLE (Not a valid sample) */
#define P2_F3_RELEASED  (0xF4u)  /* _RING   (Not a valid sample) */
#define P2_F4_RELEASED  (0xF8u)  /* _PINKY  (Not a valid sample) */

#define P2_CONTROL      (0x08u)  /* Control line for player 1 */
#define P1_CONTROL      (0x04u)  /* Control line for player 2 */

/** P R I V A T E   V A R I A B L E S *******************************/
static unsigned char p2_timer_20ms = 20;
static unsigned char state = P2_IDLE;

/** P U B L I C   V A R I A B L E S *********************************/
unsigned char p2_pressed = 0;

/********************************************************************/
/** P R I V A T E   D E C L A R A T I O N S *************************/
static unsigned char fsm_p2_sample(void)
{
    unsigned char sample = 0;
    
    /* When the 2 players press at the same time, a short may occur
     * through the path going from the active high control line to 
     * the active low control line, like so:
     *                 |
     *          >>>>>>>|>v        |          |          |
     *          ^ +----+ v   +-/ -+     +-/ -+     +-/ -+
     *     >>>>>> |    | v   |    |     |    |     |    |
     * '1' -------+----|-v---+----|-----+----|-----+----|
     *                 | v        |          |          |
     *          <<<<<<<|<<        |          |          |
     *          v +----+     +-/ -+     +-/ -+     +-/ -+
     *     <<<<<< |    |     |    |     |    |     |    |
     * '0' -------+----|-----+----|-----+----|-----+----|
     *
     * To overcome this problem we configure the control line of 
     * the active low control line as an input, so it doesn't 
     * matter. The value of the control line is inherentely lost
     * by shifting to the right below so we don't have to take
     * this into account.
     */ 
    T_CTRL_P1 = INPUT;
    
    /* Clear RB3, the control line of player 2 */
    D_CTRL_P2 = 0;
    
    /* Right justified value: inputs are at RB7:RB4 */
    sample = PORTB >> 4;
    
    /* Restore control lines */
    D_CTRL_P1 = 1;
    D_CTRL_P2 = 1;
    
    /* Make player 1 control line output again  */
    T_CTRL_P1 = OUTPUT;
    
    return sample;
}

void p2_tapping_fsm_init(void)
{
    state = P2_IDLE;
    p2_timer_20ms = 20;
    p2_pressed = 0;
}

/********************************************************************/
/** P U B L I C   D E C L A R A T I O N S ***************************/
void p2_tapping_fsm(void)
{
    if (!(--p2_timer_20ms)) { /* Only execute this FSM every 20ms */
        /* Reset timer */
        p2_timer_20ms = 20;
        
        /* Actual FSM */
        switch(state) {
            case P2_IDLE:
            // *** outputs ***
                p2_pressed = NONE;
            // *** transitions ***
                state = fsm_p2_sample();
                break;
            case P2_F1_PRESSED:
            // *** outputs ***
                
            // *** transitions ***
                if (NOTHING_PRESSED == fsm_p2_sample())
                    state = P2_F1_RELEASED;
                break;
            case P2_F2_PRESSED:
            // *** outputs ***

            // *** transitions ***
                if (NOTHING_PRESSED == fsm_p2_sample())
                    state = P2_F2_RELEASED;
                break;
            case P2_F3_PRESSED:
            // *** outputs ***

            // *** transitions ***
                if (NOTHING_PRESSED == fsm_p2_sample())
                    state = P2_F3_RELEASED;
                break;
            case P2_F4_PRESSED:
            // *** outputs ***
                
            // *** transitions ***
                if (NOTHING_PRESSED == fsm_p2_sample())
                    state = P2_F4_RELEASED;
                break;
            case P2_F1_RELEASED:
            // *** outputs ***
                p2_pressed = INDEX;
            // *** transitions ***
                state = P2_IDLE;    /* Unconditionally go back to IDLE */  
                break;
            case P2_F2_RELEASED:
            // *** outputs ***
                p2_pressed = MIDDLE;
            // *** transitions ***
                state = P2_IDLE;    /* Unconditionally go back to IDLE */  
                break;
            case P2_F3_RELEASED:
            // *** outputs ***
                p2_pressed = RING;
            // *** transitions ***
                state = P2_IDLE;    /* Unconditionally go back to IDLE */  
                break;
            case P2_F4_RELEASED:
            // *** outputs ***
                p2_pressed = PINKY;
            // *** transitions ***
                state = P2_IDLE;    /* Unconditionally go back to IDLE */  
                break;
            default:
                state = P2_IDLE;
        }
    }
}
