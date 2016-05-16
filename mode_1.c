/*********************************************************************
 *
 *                  EE 4 Project - Mode 1
 *
 *********************************************************************
 * Processor:       PIC18F2550
 * Compiler:        XC8 1.35+
 * Author:          Wiktoria Serek
 * Updated:         22/04/2016
 ***********************************************************/

/** I N C L U D E S *************************************************/
#include "mode_1.h"

/** D E F I N E S ***************************************************/
#define PUSHED 0

//===----------------------------------------------------------------------===//
//  PRIVATE VARIABLES
//===----------------------------------------------------------------------===//

static unsigned char blink_count = 0;
static unsigned char patterns = 0;
static unsigned short counter = 0;
static unsigned short timer = 0;
static bool running;

static enum {
    INITIALIZE, 
    DISPLAY_ON, 
    DISPLAY_OFF, 
    PATTERN_ON, 
    LIGHT_RED,
    LIGHT_YELLOW,
    LIGHT_GREEN,
    PLAY,
    BOTH_CORRECT,
    P1_CORRECT_P2_WRONG,
    P1_CORRECT_P2_NOTHING,
    P1_WRONG_P2_CORRECT,
    BOTH_WRONG,
    P1_WRONG_P2_NOTHING,
    P1_NOTHING_P2_CORRECT,
    P1_NOTHING_P2_WRONG,
    P1_WIN,
    P1_WIN_OFF,
    P2_WIN,
    P2_WIN_OFF,
    WIN_STATE,
    LOSE_STATE,
    ROUND_CHECK,
    EXIT_MODE1 
} current_state, state_to_recover;

//===----------------------------------------------------------------------===//
//  PUBLIC VARIABLES
//===----------------------------------------------------------------------===//

unsigned char P1_right_counter;
unsigned char P2_right_counter;
char p1_tapped;
char p2_tapped;

//===----------------------------------------------------------------------===//
//  PUBLIC FUNCTIONS
//===----------------------------------------------------------------------===//

void mode1_fsm_init(void) {
	current_state = INITIALIZE;
    state_to_recover = INITIALIZE;
    running = FALSE;
	counter = 0;
    timer = 0;
}


void mode1_fsm_play(void) {
    if (!running) {
        running = TRUE;
        game_ended = FALSE;
        current_state = INITIALIZE;
    }
}

#define correct(a) ((a) == CORRECT)
#define none(a) ((a) == NONE)
#define wrong(a) ((a) == WRONG)

void mode1_fsm(void) {
    
    switch (current_state) {                
        case INITIALIZE :
            if (running) {
                
                // *** outputs ***
                SCORE_setScore(0);
                LIVES_setLives(PLAYER_1,5);
                LIVES_setLives(PLAYER_2,5);
                STATE_setState(PLAYER_1,STATE_NONE);
                STATE_setState(PLAYER_2,STATE_NONE);
                MODE_setMode(1);
                PATTERN_setPattern(PLAYER_1,PATTERN_NONE);
                PATTERN_setPattern(PLAYER_2,PATTERN_NONE);
                LEDS_update();

                blink_count = 0;
                counter = 0;
                timer = 0;
                
                // *** transitions ***
                current_state = DISPLAY_ON;
            } else {
                // *** outputs ***
                running = FALSE;
                
                // *** transitions ***
                current_state = INITIALIZE;
            }
            break;
        case DISPLAY_ON :
        
            // *** outputs ***
            if (!counter) {
                PATTERN_setPattern(PLAYER_1,PATTERN_ALL);
                PATTERN_setPattern(PLAYER_2,PATTERN_ALL);
                LEDS_update();
            }
            
            counter++;
        
            // *** transitions ***
            if (counter == 200) 
                current_state = DISPLAY_OFF;
            
            break;
        case DISPLAY_OFF :
        
            // *** outputs ***
            if (200 == counter) {
                PATTERN_setPattern(PLAYER_1,PATTERN_NONE);
                PATTERN_setPattern(PLAYER_2,PATTERN_NONE);

                LEDS_update();
            }
            
            counter--;
            
            // *** transitions ***
            if (!counter) {
                if (blink_count < 3) {
                    current_state = DISPLAY_ON; 
                    blink_count++;
                } else {
                    current_state = PATTERN_ON;
                }
            } else {
                current_state = DISPLAY_OFF;
            }
            
            break;
        case PATTERN_ON :
            
            // *** outputs ***
            if (!counter) {
                round++;
                
                LIVES_setLives(PLAYER_1,5);
                LIVES_setLives(PLAYER_2,5);
                LEDS_update();
                
                /* There's double the amount of patterns in the round indicated
                 * by the round number */
                patterns = round << 1;
                
                /* Trigger pattern display FSM */
                display_pattern();
                
                /* Just a flag so the above only happens once */
                counter = 1;
            }
            
            // *** transitions ***
            if (pattern_done) {
                current_state = LIGHT_RED; 
                counter = 0;
            }
            
            break;
        case LIGHT_RED :
            
            // *** outputs ***
            if (!counter) {
                AUDIO_playSound(SOUND_READY);
                STATE_setState(PLAYER_1,STATE_READY);
                STATE_setState(PLAYER_2,STATE_READY);
                LEDS_update();
            }
            
            counter++;
            
            // *** transitions ***
            if (counter == 1000) {
                current_state = LIGHT_YELLOW; 
                counter = 0;
            }
            
            break;
        case LIGHT_YELLOW :
            
            // *** outputs ***
            if (!counter) {
                AUDIO_playSound(SOUND_SET);
                STATE_setState(PLAYER_1,STATE_SET);
                STATE_setState(PLAYER_2,STATE_SET);
                LEDS_update();
            }
            
            counter++;
            
            // *** transitions ***
            if (counter == 1000) {
                current_state = LIGHT_GREEN; 
                counter = 0; 
            }
            
            break;
        case LIGHT_GREEN :
            
            // *** outputs ***
            if(counter == 0) {
                AUDIO_playSound(SOUND_GO);
                STATE_setState(PLAYER_1,STATE_GO);
                STATE_setState(PLAYER_2,STATE_GO);
                LEDS_update();
            }
            
            /* Enable checking of taps in display pattern FSM */
            enable_checking();
            
            // *** transitions ***
            current_state = PLAY; /* Unconditional transition */
            break;
        case PLAY:
            
            // *** outputs ***
            round_ended = 0;
            
            // *** transitions ***
            if (++timer >= 30000) {
                current_state = ROUND_CHECK;
            } else {
                
                if (wrong(p1_tapped)) {
                    FEEDBACK_giveFeedback(PLAYER_1);
                    LIVES_setLives(PLAYER_1, LIVES_getLives(PLAYER_1) - 1);
                    
                    if (!LIVES_getLives(PLAYER_1))
                        current_state = ROUND_CHECK;
                } else if (correct(p1_tapped)) {
                    FEEDBACK_positiveFeedback(PLAYER_1);
                    
                    if (P1_right_counter == patterns)
                        current_state = ROUND_CHECK;
                }
                
                if (wrong(p2_tapped)) {
                    FEEDBACK_giveFeedback(PLAYER_2);
                    LIVES_setLives(PLAYER_2, LIVES_getLives(PLAYER_2) - 1);
                    
                    if (!LIVES_getLives(PLAYER_2))
                        current_state = ROUND_CHECK;
                } else if (correct(p2_tapped)) {
                    FEEDBACK_positiveFeedback(PLAYER_2);
                    
                    if (P2_right_counter == patterns)
                        current_state = ROUND_CHECK;
                }
            }
            
            break;
        
        case ROUND_CHECK:
            round_ended = TRUE;
            
            blink_count = 0;
            counter = 0;
            timer = 0;
            
            if (round < 5) {
                if (P1_right_counter == patterns)
                    SCORE_updateScore(2);
                if (P2_right_counter == patterns)
                    SCORE_updateScore(-2);
                if (!LIVES_getLives(PLAYER_1))    
                    SCORE_updateScore(-2);
                if (!LIVES_getLives(PLAYER_2))
                    SCORE_updateScore(2);
                current_state = DISPLAY_ON;
            } else {
                if (SCORE_getScore() < 0 || !LIVES_getLives(PLAYER_1)) {
                    winner = PLAYER_2;
                } else if (SCORE_getScore() > 0 || !LIVES_getLives(PLAYER_2)) {
                    winner = PLAYER_1;
                } else {
                    winner = NONE;
                }
                
                current_state = EXIT_MODE1;
            }
            break;
        case EXIT_MODE1 :
            
            game_ended = TRUE;
            running = FALSE;
            
            blink_count = 0;
            counter = 0;
            timer = 0;
            
            break;
        default:
            current_state = INITIALIZE;
            
            break;
    }
    
}