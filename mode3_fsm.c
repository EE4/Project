/* 
 * File:   mode 3 reaction game.c
 * Author: gil
 *
 * Created on 15 april 2016, 16:39
 */

#include "mode3_fsm.h"

//===----------------------------------------------------------------------===//
//  PRIVATE VARIABLES
//===----------------------------------------------------------------------===//

#define WIN_BLINK_TIME  (400)

static int blink_count;
static int counter;
static bool running;

static unsigned char prev_p1;
static unsigned char prev_p2;

static enum {
    IDLE = 0,
    LIGHT_RED,
    LIGHT_YELLOW,
    LIGHT_GREEN,
    PLAY,
    P1_WIN,
    P1_WIN_OFF,
    P2_WIN,
    P2_WIN_OFF,
    DELIBERATE,
    GAME_DONE
} current_state;

void mode3_fsm_init(void) 
{
    current_state = IDLE;
}

void mode3_fsm_play(void)
{
    if (!running) {
        running = TRUE;
        game_ended = FALSE;
        current_state = IDLE;
        counter = 0;
    }
}
        
void mode3_fsm(void){
    
    switch (current_state) {                
        case IDLE:
            if (running) {
                // *** outputs ***
                AUDIO_playSound(SOUND_READY);
                blink_count = 0;
                
                // *** transitions ***
                current_state = LIGHT_RED;
            } else {
                current_state = IDLE;
            }
            break;
        case LIGHT_RED:
            if (0 == counter) {
                STATE_setState(PLAYER_1, STATE_READY);
                STATE_setState(PLAYER_2, STATE_READY);

                LEDS_update();
            }

            counter++;
            
            if (counter == 1000) {
                
                AUDIO_playSound(SOUND_READY);
                current_state = LIGHT_YELLOW;
            }
            break;
        case LIGHT_YELLOW:
            if (1000 == counter) {
                STATE_setState(PLAYER_1, STATE_SET);
                STATE_setState(PLAYER_2, STATE_SET);

                LEDS_update();
            }
            
            counter++;
            
            if (counter == 2000) {
               AUDIO_playSound(SOUND_READY);
               current_state = LIGHT_GREEN;
            }
            break;
        case LIGHT_GREEN:
            if (2000 == counter) {
                STATE_setState(PLAYER_1, STATE_GO);
                STATE_setState(PLAYER_2, STATE_GO);

                LEDS_update();
            }
            counter = 0;
            current_state = PLAY;
            break;
        case PLAY:
            counter++;
            
            if (p1_pressed && !p2_pressed) {
                SCORE_updateScore(1);
            } else if (p2_pressed && !p1_pressed) {
                SCORE_updateScore(-1);
            } else {
                /* Doesn't do anything */
            }
            
            if (SCORE_getScore() > 7) {
                counter = 0;
                current_state = P1_WIN;
            } else if (SCORE_getScore() < -7) {
                counter = 0;
                current_state = P2_WIN;
            } else if (counter > 30000) {
                counter = 0;
                current_state = DELIBERATE;
            } else {
                current_state = PLAY;
            }
            
            break;
            
        case DELIBERATE:
            // *** outputs ***
            
            // *** transition conditions ***
            if (SCORE_getScore() > 0) {
                current_state = P1_WIN;
            } else if (SCORE_getScore() < 0) {
                current_state = P2_WIN;
            } else {
                current_state = GAME_DONE;
            }
            break;
        case P1_WIN:
            // *** outputs ***
            if (!counter) {
                AUDIO_playSound(SOUND_WON);
                PATTERN_setPattern(PLAYER_1, PATTERN_ALL);
                PATTERN_setPattern(PLAYER_2, PATTERN_NONE);
                LEDS_update();
            }

            ++counter;
            
            // *** transitions ***
            if (WIN_BLINK_TIME == counter) {
                current_state = P1_WIN_OFF;
            } else {
                current_state = P1_WIN;
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
                    current_state = P1_WIN;
                    ++blink_count;
                } else {
                    current_state = P1_WIN_OFF; 
                }
            } else {
                current_state = GAME_DONE;
            }
            
            break;
        case P2_WIN:
            
            // *** outputs ***
            if (!counter) {
                AUDIO_playSound(SOUND_WON);
                PATTERN_setPattern(PLAYER_1, PATTERN_NONE);
                PATTERN_setPattern(PLAYER_2, PATTERN_ALL);
                LEDS_update();
            }

            ++counter;
            
            // *** transitions ***
            if (WIN_BLINK_TIME == counter) {
                current_state = P2_WIN_OFF;
            } else {
                current_state = P2_WIN;
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
                    current_state = P2_WIN;
                    ++blink_count;
                } else {
                    current_state = P2_WIN_OFF; 
                }
            } else {
                current_state = GAME_DONE;
            }
            break;
        case GAME_DONE:
            
            // *** outputs ***
            running = FALSE;
            game_ended = TRUE;
            
            // *** transitions ***
            current_state = IDLE; // Unconditional transition
            break;
        default:
            current_state = IDLE;
            game_ended = TRUE;
            running = FALSE;
    }
}