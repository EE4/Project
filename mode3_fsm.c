/* 
 * File:   mode 3 reaction game.c
 * Author: gil
 *
 * Created on 15 april 2016, 16:39
 */

#include <xc.h>

// We have to include header files to know where things are 
#include "config.h"
#include "mode3_fsm.h"

static int counter;

/*
 *  These are already defined in config.h
 */



/*
 *  Check which are already in config and which are
 *  mode-specific
 */

static bool p1lock;
static bool p2lock;
static int arrow;


static int lives ;


static enum {
    INITIALIZE = 0,
    LIGHT_RED,
    LIGHT_YELLOW,
    LIGHT_GREEN,
    PLAY,
    P1_TAP,
    P2_TAP,
    IDLE,
    P1_UNLOCK,
    P2_UNLOCK,
    P1_WIN,
    P2_WIN,
    GAME_OVER
} current_state;
        

static void mode3(void);

void main(void) {
    return;
}


void mode3_fsm(void){
    
    switch (current_state) {                
        case INITIALIZE:
            
            // motor = FALSE;
            SCORE_setScore(0);
            
            LIVES_setLives(PLAYER_1, 5);
            LIVES_setLives(PLAYER_2, 5);
            
            MODE_setMode(2);
            
            PATTERN_setPattern(PLAYER_1, PATTERN_NONE);
            PATTERN_setPattern(PLAYER_2, PATTERN_NONE);
            
            STATE_setState(PLAYER_1, STATE_NONE);
            STATE_setState(PLAYER_2, STATE_NONE);
            
            feedback_p1 = 0;
            feedback_p2 = 0;
            
            LEDS_update();
            
            /* Uncondtional transition */
            current_state = LIGHT_RED;
            break;
            
        case LIGHT_RED:
            
            STATE_setState(PLAYER_1, STATE_READY);
            STATE_setState(PLAYER_2, STATE_READY);
            
             LEDS_Update();
            AUDIO_playSound(SOUND_READY);
             counter++;
             if (counter == 1000) current_state = LIGHT_YELLOW;
            break; 
            
        case LIGHT_YELLOW:
            STATE_setState(PLAYER_1, STATE_SET);
            STATE_setState(PLAYER_2, STATE_SET);
            
             LEDS_Update();
             AUDIO_playSound(SOUND_SET);
             counter++;
             if (counter == 2000) current_state = LIGHT_GREEN;
              break;   
              
        case LIGHT_GREEN: 
            STATE_setState(PLAYER_1, STATE_GO);
            STATE_setState(PLAYER_2, STATE_GO);
            
             LEDS_Update();
             AUDIO_playSound(SOUND_GO);
             counter++;
             if (counter == 3000) current_state = PLAY;
         break;
         
        case PLAY:
            if ( p1_pressed == TRUE) current_state = P1_TAP;
            if ( p2_pressed == TRUE) current_state = P2_TAP;
             break;
             
        case P1_TAP:
            p1lock = TRUE;
             arrow++;
            ARROW_SetPosition(arrow);
            ARROW_UpdateArrow();
            if (p2_pressed == TRUE&&p2lock == FALSE) current_state = P2_TAP;
            if(arrow>10) current_state = P1_WIN;
            else (current_state = IDLE);
             break;
             
        case P2_TAP:
            p2lock = TRUE;
            arrow--;
            ARROW_SetPosition(arrow);
            ARROW_UpdateArrow();
            if (p1_pressed  == TRUE&&p1lock == FALSE) current_state = P1_TAP;
             if(arrow<-10) current_state = P2_WIN;
            else (current_state = IDLE);
             break;
             
        case IDLE:
             if (p2_pressed == TRUE&&p2lock == FALSE) current_state = P2_TAP;
             if (p1_pressed  == TRUE&&p1lock == FALSE) current_state = P1_TAP;
             if (p1_pressed  == FALSE&&p1lock == TRUE) current_state = P1_UNLOCK;
             if (p2_pressed == FALSE&&p2lock == TRUE) current_state = P2_UNLOCK;
             break;
             
        case P1_UNLOCK:
            p1lock = FALSE;
            current_state = IDLE;
     break;
     
        case P2_UNLOCK:
            p1lock = FALSE;
            current_state = IDLE;
                break; 
                
        case P1_WIN :
            AUDIO_playSound(SOUND_WON);
             current_state = GAME_OVER;
                 break;
        case P2_WIN :
            AUDIO_playSound(SOUND_WON);
            current_state = GAME_OVER;
                 break;
        case GAME_OVER :
             break;
    }
}         
                
                                           
