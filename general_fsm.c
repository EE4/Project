/* 
 * File:   general_fsm.h
 * Author: jelle
 *
 * Created on April 30, 2016, 9:54 AM
 */

#include "general_fsm.h"

//===----------------------------------------------------------------------===//
//  CONSTANTS
//===----------------------------------------------------------------------===//
#define BLINK_TIME (200u)

//===----------------------------------------------------------------------===//
//  PRIVATE VARIABLES
//===----------------------------------------------------------------------===//
    
static enum {
    RESET = 0,
    IDLE,
    IDLE_BLINK,
    MODE1,
    MODE2,
    MODE3,
    PRE_MODE1,
    PRE_MODE2,
    PRE_MODE3,
} state;

static unsigned short timer = 0;

//===----------------------------------------------------------------------===//
//  API FUNCTIONS
//===----------------------------------------------------------------------===//
    
void general_fsm_init(void)
{
    state = RESET;
    timer = 0;
}

void general_fsm(void)
{
    switch (state) {
        case RESET:
            // *** outputs ***
            timer = 0;
            
            game_ended = TRUE;
            round = 0;
            
            SCORE_setScore(0);
            PATTERN_setPattern(PLAYER_1, PATTERN_ALL);
            PATTERN_setPattern(PLAYER_2, PATTERN_ALL);
            STATE_setState(PLAYER_1, STATE_NONE);
            STATE_setState(PLAYER_2, STATE_NONE);
            LIVES_setLives(PLAYER_1, 5);
            LIVES_setLives(PLAYER_2, 5);
            
            /* Trigger update */
            LEDS_update();
            
            // *** transitions ***
            state = IDLE; // Unconditional transition
            break;
        case IDLE:
            // *** outputs ***
            mode_idle = TRUE;
            
            /* Update mode duty cycle of mode display */
            if (!((++timer) % 16))
                PWM_duty[1] = (PWM_duty[MODE_CHANNEL] + 2) % 200;
            // *** transitions ***
            switch (p1_pressed) {
                case INDEX:
                    /* TODO: Update to MODE1 */
                    state = PRE_MODE1;
                    break;
                case MIDDLE:
                    /* TODO: Update to MODE2 */
                    state = IDLE;
                    break;
                case RING:
                    state = PRE_MODE3;
                    break;
                default:
                    state = IDLE;
            }
            break;
        case PRE_MODE1:
            // *** outputs ***
            MODE_setMode(1);
            mode1_fsm_play();
            // *** transitions ***
            state = MODE1;
            break;
        case MODE1:
            // *** outputs ***
            
            // *** transitions ***
            if (game_ended)
                state = RESET;
            break;
        case PRE_MODE2:
            // *** outputs ***
            
            // *** transitions ***
            break;
        case MODE2:
            // *** outputs ***
            
            // *** transitions ***
            if (game_ended)
                state = RESET;
            break;
        case PRE_MODE3:
            // *** outputs ***
            MODE_setMode(3);
            mode3_fsm_play();
           
            // *** transitions ***
            state = MODE3; /* Uncondtional transition */
            break;
        case MODE3:
            // *** outputs ***
            
            // *** transitions ***
            if (game_ended)
                state = RESET;
            break;
        default:
            break;
    }
}

//===----------------------------------------------------------------------===//
//  API FUNCTIONS
//===----------------------------------------------------------------------===//
    
