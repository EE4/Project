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
    WAIT,
    PRE_MODE1,
    PRE_MODE2,
    PRE_MODE3,
    P1_WIN,
    P1_WIN_OFF,
    P2_WIN,
    P2_WIN_OFF
} state;

static unsigned short timer = 0;
static unsigned short counter = 0;
static unsigned char blink_count = 0;

unsigned char winner = 0;

//===----------------------------------------------------------------------===//
//  API FUNCTIONS
//===----------------------------------------------------------------------===//
    
void general_fsm_init(void)
{
    blink_count = 0;
    state = RESET;
    counter = 0;
    winner = 0;
    timer = 0;
}

void general_fsm(void)
{
    switch (state) {
        case RESET:
            // *** outputs ***
            timer = 0;
            
            game_ended = TRUE;
            blink_count = 0;
            winner = NONE;
            counter = 0;
            round = 0;
            
            PATTERN_setPattern(PLAYER_1, PATTERN_ALL);
            PATTERN_setPattern(PLAYER_2, PATTERN_ALL);
            STATE_setState(PLAYER_1, STATE_NONE);
            STATE_setState(PLAYER_2, STATE_NONE);
            LIVES_setLives(PLAYER_1, 5);
            LIVES_setLives(PLAYER_2, 5);
            SCORE_setScore(0);
            
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
                    state = PRE_MODE2;
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
            state = WAIT;
            break;
        case PRE_MODE2:
            // *** outputs ***
            MODE_setMode(2);
            mode2_fsm_play();
            // *** transitions ***
            state = WAIT;
            break;
        case PRE_MODE3:
            // *** outputs ***
            MODE_setMode(3);
            mode3_fsm_play();
           
            // *** transitions ***
            state = WAIT; /* Unconditional transition */
            break;
        case WAIT:
            // *** outputs ***
            // *** transitions ***
            if (game_ended) {
                if (PLAYER_1 == winner) {
                    state = P1_WIN;
                } else if (PLAYER_2 == winner) {
                    state = P2_WIN;
                } else {
                    state = RESET;
                }
            }
            break;
        case P1_WIN:
            // *** outputs ***
            if (!counter) {
                if (!blink_count)
                    AUDIO_playSound(SOUND_WON);
                /* Turn off loser's LEDs */
                PATTERN_setPattern(PLAYER_2, PATTERN_NONE);
                STATE_setState(PLAYER_2, STATE_NONE);
                LIVES_setLives(PLAYER_2, 0);
                
                /* Turn on winner's LEDs */
                PATTERN_setPattern(PLAYER_1, PATTERN_ALL);
                STATE_setState(PLAYER_1, STATE_ALL);
                LIVES_setLives(PLAYER_1, 5);
                
                LEDS_update();
            }

            ++counter;
            
            // *** transitions ***
            if (WIN_BLINK_TIME == counter) {
                state = P1_WIN_OFF;
            } else {
                state = P1_WIN;
            }
            break;
        case P1_WIN_OFF:
            
            // *** outputs ***
            if (WIN_BLINK_TIME == counter) {
                PATTERN_setPattern(PLAYER_1, PATTERN_NONE);
                LEDS_update();
            }
            
            --counter;
            
            // *** transitions ***
            if (blink_count < 3) {
                if (!counter) {
                    state = P1_WIN;
                    ++blink_count;
                } else {
                    state = P1_WIN_OFF; 
                }
            } else {
                state = RESET;
            }
            
            break;
        case P2_WIN:
            
            // *** outputs ***
            if (!counter) {
                if (!blink_count)
                    AUDIO_playSound(SOUND_WON);
                /* Turn off loser's LEDs */
                PATTERN_setPattern(PLAYER_1, PATTERN_NONE);
                STATE_setState(PLAYER_1, STATE_NONE);
                LIVES_setLives(PLAYER_1, 0);
                
                /* Turn on winner's LEDs */
                PATTERN_setPattern(PLAYER_2, PATTERN_ALL);
                STATE_setState(PLAYER_2, STATE_ALL);
                LIVES_setLives(PLAYER_2, 5);
                
                LEDS_update();
            }

            ++counter;
            
            // *** transitions ***
            if (WIN_BLINK_TIME == counter) {
                state = P2_WIN_OFF;
            } else {
                state = P2_WIN;
            }
            break;
        case P2_WIN_OFF:
            
            // *** outputs ***
            if (WIN_BLINK_TIME == counter) {
                PATTERN_setPattern(PLAYER_2, PATTERN_NONE);
                LEDS_update();
            }
            
            --counter;
            
            // *** transitions ***
            if (blink_count < 3) {
                if (!counter) {
                    state = P2_WIN;
                    ++blink_count;
                } else {
                    state = P2_WIN_OFF; 
                }
            } else {
                state = RESET;
            }
            break;
        default:
            break;
    }
}

//===----------------------------------------------------------------------===//
//  API FUNCTIONS
//===----------------------------------------------------------------------===//
    
