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

/** P R I V A T E   V A R I A B L E S *******************************/
static unsigned char counter;
static int timer = 0;
static int blink_count = 0;
static bool running;
static enum {INITIALIZE, 
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
            WIN_STATE,
            LOSE_STATE,
            ROUND_CHECK,
            EXIT_MODE1 } current_state, state_to_recover;

            bool P1_correct;
            bool P1_wrong;
            bool P1_nothing;
            bool P2_correct;
            bool P2_wrong;
            bool P2_nothing;
            int P1_right_counter;
            int P2_right_counter;
            static unsigned char winner;
            static unsigned char looser;
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
void mode1_fsm_init(void) {
	current_state = INITIALIZE;
    state_to_recover = INITIALIZE;
	counter = 0;
    timer = 0;
	
    hardware_init();    
}


void mode1_fsm_play(void) {
    if (!running) {
        running = TRUE;
        game_ended = FALSE;
        current_state = IDLE;
        blink_count = 0;
        counter = 0;
    }
}

/********************************************************************
 * Function:        simple_example(void)
 * PreCondition:    None
 * Input:           None
 * Output:          None
 * Overview:        A simple FSM that will start blinking a led on 
 *                  and off once you pushed a button.          
 ********************************************************************/
void mode1_fsm(void) {
    
    switch (current_state) {                
        case INITIALIZE :
            if (running) {
                SCORE_setScore(0);
                LIVES_setLives(PLAYER_1,5);
                LIVES_setLives(PLAYER_2,5);
                STATE_setState(PLAYER_1,STATE_NONE);
                STATE_setState(PLAYER_2,STATE_NONE);
                MODE_setMode(1);
                PATTERN_setPattern(PLAYER_1,PATTERN_NONE);
                PATTERN_setPattern(PLAYER_2,PATTERN_NONE);
                LEDS_update();

                current_state = DISPLAY_ON;
            } else {
                current_state = INITIALIZE;
            }
            break;

        case DISPLAY_ON :
        
            PATTERN_setPattern(PLAYER_1,PATTERN_ALL);
            PATTERN_setPattern(PLAYER_2,PATTERN_ALL);
            counter++;
            LEDS_update();
        
            if (counter == 200) current_state = DISPLAY_OFF;
            break;

        case DISPLAY_OFF :
        
            PATTERN_setPattern(PLAYER_1,PATTERN_NONE);
            PATTERN_setPattern(PLAYER_2,PATTERN_NONE);
            counter--;
            LEDS_update();
            
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
            round++;
            display_pattern();
            
            if (pattern_done == 1) {current_state = LIGHT_RED; counter = 0;}
            break;
            
        case LIGHT_RED :
            STATE_setState(PLAYER_1,STATE_READY);
            STATE_setState(PLAYER_2,STATE_READY);
            AUDIO_playSound(SOUND_READY);
            counter++;
            LEDS_update();
            
            if (counter == 1000) {current_state = LIGHT_YELLOW; counter = 0;}
            break;
            
        case LIGHT_YELLOW :
            STATE_setState(PLAYER_1,STATE_SET);
            STATE_setState(PLAYER_2,STATE_SET);
            AUDIO_playSound(SOUND_SET);
            counter++;
            LEDS_update();
            
            if (counter == 1000) {current_state = LIGHT_GREEN; counter = 0;}
            break;
            
        case LIGHT_GREEN :
            STATE_setState(PLAYER_1,STATE_GO);
            STATE_setState(PLAYER_2,STATE_GO);
            AUDIO_playSound(SOUND_GO);
            counter++;
            LEDS_update();
            
            /* Enable checking of taps in display pattern FSM */
            enable_tapping();
            
            current_state = PLAY;
        
        case PLAY : 
            STATE_setState(PLAYER_1,STATE_NONE);
            STATE_setState(PLAYER_2,STATE_NONE);
            AUDIO_playSound(SOUND_NONE);
            LEDS_update();
            timer++;
            
            if (timer >= 30000) current_state = ROUND_CHECK;
            if (P1_correct & P2_correct) current_state = BOTH_CORRECT;
            if (P1_correct & P2_wrong) current_state = P1_CORRECT_P2_WRONG;
            if (P1_correct & P2_nothing) current_state = P1_CORRECT_P2_NOTHING;
            if (P1_wrong & P2_correct) current_state = P1_WRONG_P2_CORRECT;
            if (P1_wrong & P2_wrong) current_state = BOTH_WRONG;
            if (P1_wrong & P2_nothing) current_state = P1_WRONG_P2_NOTHING;
            if (P1_nothing & P2_correct) current_state = P1_NOTHING_P2_CORRECT;
            if (P1_nothing & P2_wrong) current_state = P1_NOTHING_P2_WRONG;
            if (P1_nothing & P2_nothing) current_state = PLAY;
            break;
            
        case BOTH_CORRECT :
            P1_right_counter++;
            P2_right_counter++;
            
            if (timer >= 30000) current_state = ROUND_CHECK;
            if (P1_right_counter == 10 && P2_right_counter == 10) {winner = NONE; current_state = WIN_STATE;}
            if (P1_right_counter == 10 ) {winner = PLAYER_1; current_state = WIN_STATE;}
            if (P2_right_counter == 10 ) {winner = PLAYER_2; current_state = WIN_STATE;}
            else current_state = PLAY;
            break;
         
        case P1_CORRECT_P2_WRONG :
            P1_right_counter++;
            FEEDBACK_giveFeedback(PLAYER_2);
            LIVES_setLives(PLAYER_2,-1);
            LEDS_update();
            if (timer >= 30000) current_state = ROUND_CHECK;
            if (P1_right_counter == 10 ) {winner = PLAYER_1; current_state = WIN_STATE;}
            if (LIVES_getLives(PLAYER_2) == 0) {looser = PLAYER_2; current_state = LOSE_STATE;}
            else current_state = PLAY;
            break;
            
        case P1_CORRECT_P2_NOTHING :
            P1_right_counter++;
            if (timer >= 30000) current_state = ROUND_CHECK;
            if (P1_right_counter == 10 ) {winner = PLAYER_1; current_state = WIN_STATE;}
            else current_state = PLAY;
            break;
            
        case P1_WRONG_P2_CORRECT :
            P2_right_counter++;
            FEEDBACK_giveFeedback(PLAYER_1);
            LIVES_setLives(PLAYER_1,-1);
            LEDS_update();
            if (timer >= 30000) current_state = ROUND_CHECK;
            if (P2_right_counter == 10 ) {winner = PLAYER_2; current_state = WIN_STATE;}
            if (LIVES_getLives(PLAYER_1) == 0) {looser = PLAYER_1; current_state = LOSE_STATE;}
            else current_state = PLAY;
            break;
            
        case BOTH_WRONG :
            FEEDBACK_giveFeedback(PLAYER_1);
            LIVES_setLives(PLAYER_1,-1);
            FEEDBACK_giveFeedback(PLAYER_2);
            LIVES_setLives(PLAYER_1,-1);
            LEDS_update();
            
            if (timer >= 30000) current_state = ROUND_CHECK;
            if (LIVES_getLives(PLAYER_1) == 0 && LIVES_getLives(PLAYER_2) == 0) {looser = NONE; current_state = LOSE_STATE;}
            if (LIVES_getLives(PLAYER_1) == 0) {looser = PLAYER_1; current_state = LOSE_STATE;}
            if (LIVES_getLives(PLAYER_2) == 0) {looser = PLAYER_2; current_state = LOSE_STATE;}
            else current_state = PLAY;
            break;
            
        case P1_WRONG_P2_NOTHING :
            FEEDBACK_giveFeedback(PLAYER_1);
            LIVES_setLives(PLAYER_1,-1);
            LEDS_update();
            
            if (timer >= 30000) current_state = ROUND_CHECK;
            if (LIVES_getLives(PLAYER_1) == 0) {looser = PLAYER_1; current_state = LOSE_STATE;}
            else current_state = PLAY;
            break;
            
        case P1_NOTHING_P2_CORRECT :
            P2_right_counter++;
            
            if (timer >= 30000) current_state = ROUND_CHECK;
            if (P2_right_counter == 10 ) {winner = PLAYER_2; current_state = WIN_STATE;}
            else current_state = PLAY;
            break;
            
        case P1_NOTHING_P2_WRONG :
            FEEDBACK_giveFeedback(PLAYER_2);
            LIVES_setLives(PLAYER_2,-1);
            LEDS_update();
            
            if (timer >= 30000) current_state = ROUND_CHECK;
            if (LIVES_getLives(PLAYER_2) == 0) {looser = PLAYER_2; current_state = LOSE_STATE;}
            else current_state = PLAY;
            break;
            
        case WIN_STATE :
            if (winner = NONE) { 
                PATTERN_setPattern(PLAYER_1,PATTERN_ALL); 
                STATE_setState(PLAYER_1,STATE_ALL);
                LIVES_setLives(PLAYER_1,5);
                PATTERN_setPattern(PLAYER_2,PATTERN_ALL); 
                STATE_setState(PLAYER_2,STATE_ALL);
                LIVES_setLives(PLAYER_2,5);}
            if (winner = PLAYER_1) {
                SCORE_updateScore(+2); 
                PATTERN_setPattern(PLAYER_1,PATTERN_ALL); 
                STATE_setState(PLAYER_1,STATE_ALL);
                LIVES_setLives(PLAYER_1,5);
                PATTERN_setPattern(PLAYER_2,PATTERN_NONE); 
                STATE_setState(PLAYER_2,STATE_NONE);
                LIVES_setLives(PLAYER_2,0);}
            if (winner = PLAYER_2) {
                SCORE_updateScore(-2); 
                PATTERN_setPattern(PLAYER_2,PATTERN_ALL); 
                STATE_setState(PLAYER_2,STATE_ALL);
                LIVES_setLives(PLAYER_2,5);
                PATTERN_setPattern(PLAYER_1,PATTERN_NONE); 
                STATE_setState(PLAYER_1,STATE_NONE);
                LIVES_setLives(PLAYER_1,0);}
            LEDS_update();
            AUDIO_playSound(SOUND_WON);
            current_state = ROUND_CHECK;
            break;
            
        case LOSE_STATE :
            if (looser = NONE){
                PATTERN_setPattern(PLAYER_1,PATTERN_NONE); 
                STATE_setState(PLAYER_1,STATE_NONE);
                LIVES_setLives(PLAYER_1,0);
                PATTERN_setPattern(PLAYER_2,PATTERN_NONE); 
                STATE_setState(PLAYER_2,STATE_NONE);
                LIVES_setLives(PLAYER_2,0);}
            if (looser = PLAYER_1){
                SCORE_updateScore(-2); 
                PATTERN_setPattern(PLAYER_2,PATTERN_ALL); 
                STATE_setState(PLAYER_2,STATE_ALL);
                LIVES_setLives(PLAYER_2,5);
                PATTERN_setPattern(PLAYER_1,PATTERN_NONE); 
                STATE_setState(PLAYER_1,STATE_NONE);
                LIVES_setLives(PLAYER_1,0);}
            if (looser = PLAYER_2){
                SCORE_updateScore(+2); 
                PATTERN_setPattern(PLAYER_1,PATTERN_ALL); 
                STATE_setState(PLAYER_1,STATE_ALL);
                LIVES_setLives(PLAYER_1,5);
                PATTERN_setPattern(PLAYER_2,PATTERN_NONE); 
                STATE_setState(PLAYER_2,STATE_NONE);
                LIVES_setLives(PLAYER_2,0);}
            LEDS_update();
            AUDIO_playSound(SOUND_LOST);
            current_state = ROUND_CHECK;
            break;
            
        case ROUND_CHECK :
            
            if (round < 5) current_state = DISPLAY_ON;
            else current_state = EXIT_MODE1;
            break;
            
        case EXIT_MODE1 :
            game_ended = 1;
            break;
            
        default:
            current_state = INITIALIZE;
            break;
    }
    
}