/** I N C L U D E S *************************************************/
#include "config.h"

/** D E F I N E S ***************************************************/
#define PUSHED 0

/** P R I V A T E   V A R I A B L E S *******************************/
static unsigned char counter;
static enum {
    MODE2_INIT,
    READY,
    SET,
    GO_DETECT,
    INCREASE_SCORE,
    DECREASE_SCORE,
    SHOW_WINNER_P1,
    SHOW_WINNER_P2,
    ENABLE_TAP,
    EXIT_MODE2
} current_state, state_to_recover;

void mode_3_init(void) {
	current_state = MODE2_INIT;
    state_to_recover = MODE2_INIT;
	counter = 0;
	
    hardware_init();    
}
void mode_3(void) {
    
    switch (current_state) {                
        case MODE2_INIT :
            SCORE_setScore(0);
            LIVES_setLives(PLAYER_1,5);
            LIVES_setLives(PLAYER_2,5);
            STATE_setState(PLAYER_1,STATE_NONE);
            STATE_setState(PLAYER_2,STATE_NONE);
            MODE_setMode(3);
            PATTERN_setPattern(PLAYER_1,PATTERN_NONE);
            PATTERN_setPattern(PLAYER_2,PATTERN_NONE);
            Lock_Tap_Detection();
            AUDIO_playSound(SOUND_NONE);
            LEDS_update();

            current_state = READY;
            break;

        case READY :
            STATE_setState(PLAYER_1,STATE_READY);
            STATE_setState(PLAYER_2,STATE_READY);
            AUDIO_playSound(SOUND_READY);
            counter++;
            LEDS_update();
            round++;
            
            if (counter == 1000) current_state = SET;
            break;
            
        case SET :
            STATE_setState(PLAYER_1,STATE_SET);
            STATE_setState(PLAYER_2,STATE_SET);
            AUDIO_playSound(SOUND_SET);
            counter++;
            LEDS_update();
            
            if (counter == (2000 + rand*1000)) current_state = GO_DETECT;
            break;
            
        case GO_DETECT :
            STATE_setState(PLAYER_1,STATE_GO);
            STATE_setState(PLAYER_2,STATE_GO);
            Open_Tap_Detection();
            AUDIO_playSound(SOUND_GO);
            counter++;
            LEDS_update();
            
            if (P1_correct || P2_wrong) current_state = INCREASE_SCORE;
            if (P1_wrong || P2_correct) current_state = DECREASE_SCORE;
            break;
            
        case INCREASE_SCORE :
            SCORE_updateScore(+2);
            FEEDBACK_turnOn(PLAYER_2);
            LIVES_setLives(PLAYER_2,-1);
            Lock_Tap_Detection();
            LEDS_update();
            
            if (LIVES_getLives(player_2) == 0) current_state = SHOW_WINNER_P1;
            if (round < 7 || SCORE_getScore() = 0) current_state = READY;
            if (round >= 7 && SCORE_getScore() < 0) current_state = SHOW_WINNER_P2;
            if (round >= 7 && SCORE_getScore() > 0) current_state = SHOW_WINNER_P1;
            break;
            
        case DECREASE_SCORE :
            SCORE_updateScore(-2);
            FEEDBACK_turnOn(PLAYER_1);
            LIVES_setLives(PLAYER_1,-1);
            Lock_Tap_Detection();
            LEDS_update();
            
            if (LIVES_getLives(player_1) == 0) current_state = SHOW_WINNER_P2;
            if (round < 7 || SCORE_getScore() = 0) current_state = READY;
            if (round >= 7 && SCORE_getScore() < 0) current_state = SHOW_WINNER_P2;
            if (round >= 7 && SCORE_getScore() > 0) current_state = SHOW_WINNER_P1;
            break;
            
        case SHOW_WINNER_P1 :
            PATTERN_setPattern(PLAYER_1,PATTERN_ALL); 
            STATE_setState(PLAYER_1,STATE_ALL);
            LIVES_setLives(PLAYER_1,5);
            PATTERN_setPattern(PLAYER_2,PATTERN_NONE); 
            STATE_setState(PLAYER_2,STATE_NONE);
            LIVES_setLives(PLAYER_2,0)};
            AUDIO_playSound(SOUND_WON);
            timer++;
            
            if (timer == 1000) current_state = ENABLE_TAP;
            break;
            
        case SHOW_WINNER_P2 :
            PATTERN_setPattern(PLAYER_2,PATTERN_ALL); 
            STATE_setState(PLAYER_2,STATE_ALL);
            LIVES_setLives(PLAYER_2,5);
            PATTERN_setPattern(PLAYER_1,PATTERN_NONE); 
            STATE_setState(PLAYER_1,STATE_NONE);
            LIVES_setLives(PLAYER_1,0);}
            AUDIO_playSound(SOUND_WON);
            timer++;
            
            if (timer == 1000) current_state = ENABLE_TAP;
            break;
            
        case ENABLE_TAP :
            Open_Tab_Detection();
            
            if (p1_pressed != null || p2_pressed != null) current_state = EXIT_MODE2;
            break;
            
        case EXIT_MODE2 :
             game_ended = 1;
             break;
                 
        default:
            current_state = MODE2_INIT;
            break;
    }
}