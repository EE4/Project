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
                blink_count = 0;
                
                // *** transitions ***
                current_state = LIGHT_RED;
            } else {
                current_state = IDLE;
            }
            break;
        case LIGHT_RED:
            if (0 == counter) {
                AUDIO_playSound(SOUND_READY);
                STATE_setState(PLAYER_1, STATE_READY);
                STATE_setState(PLAYER_2, STATE_READY);

                LEDS_update();
            }
            
            tap_feedback_enable(FALSE);

            counter++;
            
            if (counter == 1000) {
                current_state = LIGHT_YELLOW;
            }
            break;
        case LIGHT_YELLOW:
            if (1000 == counter) {
                AUDIO_playSound(SOUND_SET);
                STATE_setState(PLAYER_1, STATE_SET);
                STATE_setState(PLAYER_2, STATE_SET);

                LEDS_update();
            }
            
            counter++;
            
            if (counter == 2000) {
               current_state = LIGHT_GREEN;
            }
            break;
        case LIGHT_GREEN:
            
            // *** OUTPUTS ***
            counter = 0;
            AUDIO_playSound(SOUND_GO);
            STATE_setState(PLAYER_1, STATE_GO);
            STATE_setState(PLAYER_2, STATE_GO);

            LEDS_update();
            
            // *** TRANSITIONS ***
            current_state = PLAY;
            break;
        case PLAY:
            tap_feedback_enable(TRUE);
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
                winner = PLAYER_1;
            } else if (SCORE_getScore() < 0) {
                winner = PLAYER_2;
            } else {
                winner = NONE;
            }
            
            current_state = GAME_DONE;
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