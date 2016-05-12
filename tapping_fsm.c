/* 
 * File:   tapping_fsm.h
 * Author: jelle
 *
 * Created on May 9, 2016, 9:03 PM
 */

#include "tapping_fsm.h"

//===----------------------------------------------------------------------===//
//  CONSTANTS
//===----------------------------------------------------------------------===//

#define NOTHING_PRESSED (0x0Fu)  /* NO FINGER PRESSED */
#define TAP_IDLE         NOTHING_PRESSED
#define P1_F1_PRESSED   (0x0Eu)  /* RING FINGER     */
#define P1_F2_PRESSED   (0x0Du)  /* MIDDLE FINGER   */
#define P1_F3_PRESSED   (0x0Bu)  /* RING FINGER     */
#define P1_F4_PRESSED   (0x07u)  /* PINKY FINGER    */
#define P2_F1_PRESSED   (0x1Eu)  /* RING FINGER     */
#define P2_F2_PRESSED   (0x1Du)  /* MIDDLE FINGER   */
#define P2_F3_PRESSED   (0x1Bu)  /* RING FINGER     */
#define P2_F4_PRESSED   (0x17u)  /* PINKY FINGER    */

#define P1_F1_RELEASED  (0xE1u)  /* /RING   (Not a valid sample) */
#define P1_F2_RELEASED  (0xE2u)  /* /MIDDLE (Not a valid sample) */
#define P1_F3_RELEASED  (0xE4u)  /* /RING   (Not a valid sample) */
#define P1_F4_RELEASED  (0xE8u)  /* /PINKY  (Not a valid sample) */
#define P2_F1_RELEASED  (0xF1u)  /* /RING   (Not a valid sample) */
#define P2_F2_RELEASED  (0xF2u)  /* /MIDDLE (Not a valid sample) */
#define P2_F3_RELEASED  (0xF4u)  /* /RING   (Not a valid sample) */
#define P2_F4_RELEASED  (0xF8u)  /* /PINKY  (Not a valid sample) */

//===----------------------------------------------------------------------===//
//  PRIVATE VARIABLES
//===----------------------------------------------------------------------===//

static enum 
{
    TAP_RESET = 0,
    TAP_IDLE_P1,
    TAP_IDLE_P2,
} state;

static enum
{
    CTRL_P1 = 0,
    CTRL_P2,
} ctrl_state;

static unsigned char active_player = 0;
static unsigned long long timer = 0;
static unsigned long sample_due = 0;

//===----------------------------------------------------------------------===//
//  PRIVATE FUNCTIONS
//===----------------------------------------------------------------------===//

static void tap_precondition(unsigned char player)
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
    if (PLAYER_1 == player) {
        T_CTRL_P2 = INPUT;
    
        /* Clear the control line of player 1 */
        D_CTRL_P1 = 0;
    } else if (PLAYER_2 == player) {
        T_CTRL_P1 = INPUT;
    
        /* Clear the control line of player 2 */
        D_CTRL_P2 = 0;
    } else {
        /* Do nothing, faulty argument */
        return;
    }
}

static unsigned char tap_sample(void)
{
    unsigned char sample = 0;
    
    /* Right justified value: inputs are at RB7:RB4 */
    sample = PORTB >> 4;
    
    if (PLAYER_1 == active_player) {
        sample |= 0x10;
    } else if (PLAYER_2 == active_player) {
        
    } else {
        /* Not a valid state, return */
        return (unsigned char)TAP_IDLE;
    }
    
    return (unsigned char)TAP_IDLE;
}

static void tap_postcondition(unsigned char player)
{
    /* Restore control lines */
    D_CTRL_P1 = 1;
    D_CTRL_P2 = 1;
    
    /* Make other's player control line output again */
    if (PLAYER_1 == player) {
        T_CTRL_P2 = OUTPUT;
    } else if (PLAYER_2 == player)  {
        T_CTRL_P1 = OUTPUT;
    } else {
        /* Do nothing, faulty argument */
    }
}

//===----------------------------------------------------------------------===//
//  PUBLIC FUNCTIONS
//===----------------------------------------------------------------------===//

void tapping_tick(void)
{
    if (!(--timer)) {
        /* Reset timer */
        timer = SAMPLE_PERIOD;
        
        switch (ctrl_state) {
            case CTRL_P1:
            /* OUTPUTS */
                tap_postcondition(PLAYER_1);
                active_player = PLAYER_1;
                tap_precondition(PLAYER_1);
                
            /* TRANSITION CONDITIONS */
                ctrl_state = CTRL_P2;
            case CTRL_P2:
            /* OUTPUTS */
                tap_postcondition(PLAYER_2);
                active_player = PLAYER_2;
                tap_precondition(PLAYER_2);
                
            /* TRANSITION CONDITIONS */
                ctrl_state = CTRL_P1;
            default:
                ctrl_state = CTRL_P1;
        }
    }
}

void tapping_fsm_init(void)
{
    state = TAP_IDLE_P1;
    timer = SAMPLE_PERIOD;
    sample_due = SAMPLE_PERIOD;
    ctrl_state = CTRL_P1;
}

void tapping_fsm(void)
{
    if (!(--sample_due)) { /* Only execute this FSM every 20ms */
        /* Reset timer */
        sample_due = SAMPLE_PERIOD;
        
        /* Actual FSM */
        switch(state) {
            case TAP_IDLE:
            // *** outputs ***
                p1_pressed = NONE;
            // *** transitions ***
                
                state = tap_sample();
                break;
            case P1_F1_PRESSED:
            // *** outputs ***
                
            // *** transitions ***
                if (P1_F1_PRESSED != tap_sample())
                    state = P1_F1_RELEASED;
                break;
            case P1_F2_PRESSED:
            // *** outputs ***

            // *** transitions ***
                if (P1_F2_PRESSED != tap_sample())
                    state = P1_F2_RELEASED;
                break;
            case P1_F3_PRESSED:
            // *** outputs ***

            // *** transitions ***
                if (P1_F3_PRESSED != tap_sample())
                    state = P1_F3_RELEASED;
                break;
            case P1_F4_PRESSED:
            // *** outputs ***
                
            // *** transitions ***
                if (P1_F4_PRESSED != tap_sample())
                    state = P1_F4_RELEASED;
                break;
            case P1_F1_RELEASED:
            // *** outputs ***
                p1_pressed = INDEX;
            // *** transitions ***
                state = TAP_IDLE;    /* Unconditionally go back to IDLE */  
                break;
            case P1_F2_RELEASED:
            // *** outputs ***
                p1_pressed = MIDDLE;
            // *** transitions ***
                state = TAP_IDLE;    /* Unconditionally go back to IDLE */  
                break;
            case P1_F3_RELEASED:
            // *** outputs ***
                p1_pressed = RING;
            // *** transitions ***
                state = TAP_IDLE;    /* Unconditionally go back to IDLE */  
                break;
            case P1_F4_RELEASED:
            // *** outputs ***
                p1_pressed = PINKY;
            // *** transitions ***
                state = TAP_IDLE;    /* Unconditionally go back to IDLE */  
                break;
            default:
                state = TAP_IDLE;
        }
    } else {
        p1_pressed = NONE;
    }
}