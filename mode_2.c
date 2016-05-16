/** I N C L U D E S *************************************************/
#include "mode_2.h"

/** D E F I N E S ***************************************************/
#define PUSHED 0

/** P R I V A T E   V A R I A B L E S *******************************/
static unsigned short counter;
static unsigned char pattern;
static unsigned long timer;
static unsigned short r;

static bool running;

static enum {
    MODE2_INIT,
    READY,
    SET,
    GO_DETECT,
    NEXT_ROUND,
    EXIT_MODE2
} current_state, state_to_recover;

void mode2_fsm_play(void) {
    if (!running) {
        running = TRUE;
        game_ended = FALSE;
    }
}

void mode2_fsm_init(void) {
	current_state = MODE2_INIT;
    state_to_recover = MODE2_INIT;
	counter = 0;
    timer = 0;
    running = FALSE;
    r = 0;
}

void mode2_fsm(void) {
    
    switch (current_state) {                
        case MODE2_INIT :
            
            if (running) {
                current_state = READY;
            } else {
                current_state = MODE2_INIT;
                running = FALSE;
                r = 0;
            }
            
            break;
        case READY:
            if (!counter) {
                STATE_setState(PLAYER_1,STATE_READY);
                STATE_setState(PLAYER_2,STATE_READY);
                AUDIO_playSound(SOUND_READY);
                LEDS_update();
            }
            
            counter++;
            
            if (counter == 1000) {
                current_state = SET;
                counter = 0;
            }
            break;
        case SET:
            if (!counter) {
                STATE_setState(PLAYER_1,STATE_SET);
                STATE_setState(PLAYER_2,STATE_SET);
                AUDIO_playSound(SOUND_SET);
                LEDS_update();
                r = (RAND_gen() * (RAND_gen() >> 3)) + 3000;
            }
            
            counter++;
            
            if (counter > r) {
                current_state = GO_DETECT;
                counter = 0;
            }
            break;
            
        case GO_DETECT:
            if (!counter) {
                pattern = pattern_generate();
                PATTERN_setPattern(PLAYER_1, pattern);
                PATTERN_setPattern(PLAYER_2, pattern);
                STATE_setState(PLAYER_1,STATE_GO);
                STATE_setState(PLAYER_2,STATE_GO);
                AUDIO_playSound(SOUND_GO);
                LEDS_update();
            }
            
            counter++;
            
            if (p1_pressed && !p2_pressed) {
                if (p1_pressed == pattern) {
                    SCORE_updateScore(2);
                    current_state = NEXT_ROUND;
                    return;
                }
            } else if (p2_pressed && !p1_pressed) {
                if (p2_pressed == pattern) {
                    SCORE_updateScore(-2);
                    current_state = NEXT_ROUND;
                    return;
                }
            } else if (p1_pressed && p2_pressed) {
                if (p1_pressed == pattern && p2_pressed != pattern) {
                    SCORE_updateScore(2);
                    current_state = NEXT_ROUND;
                    return;
                } else if (p2_pressed == pattern && p1_pressed != pattern) {
                    SCORE_updateScore(-2);
                    current_state = NEXT_ROUND;
                    return;
                } else if (p1_pressed == pattern && p2_pressed == pattern) {
                    /* Both correct, don't update, go to next round */
                    current_state = NEXT_ROUND;
                }
            } else {
                current_state = GO_DETECT;
                if (!counter) {
                    /* detect overflow, exit mode */
                    winner = NONE;
                    current_state = EXIT_MODE2;
                }
            }
            break;
        case NEXT_ROUND:
            if (SCORE_getScore() > 8) {
                winner = PLAYER_1;
                current_state = EXIT_MODE2;
            } else if (SCORE_getScore() < -8) {
                winner = PLAYER_2;
                current_state = EXIT_MODE2;
            } else {
                winner = NONE;
                current_state = READY;
                pattern = 0;
                counter = 0;
                r = 0;
            }
            
            break;
        case EXIT_MODE2:
            game_ended = TRUE;
            running = FALSE;
            current_state = MODE2_INIT;
            break;
        default:
            current_state = MODE2_INIT;
            break;
    }
}