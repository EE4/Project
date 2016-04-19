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

//typedef int bool;
//#define TRUE 1
//#define FALSE 0 

/*
 *  Check which are already in config and which are
 *  mode-specific
 */
static bool p1tap;
static bool p2tap;
static bool p1lock;
static bool p2lock;
//static int arrow;
//static bool audio;
//static bool motor;
//static bool gloves;

static int lives ;

//static int modes;
//static bool pattern;
//static bool hapticFeedback;
//static int trafficLights;

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
        
static void init(void);
static void mode3(void);

void main(void) {
    return;
}
/* Redundant
static void init(void){
     current_state = INITIALIZE ;
     counter = 0;
}
 */

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
             audio = 1;
             update_Audio();
             counter++;
             if (counter == 1000) current_state = LIGHT_YELLOW;
            break; 
            
        case LIGHT_YELLOW:
            trafficLights = 010;
             LEDS_Update();
             audio = 1;
            update_Audio();
             counter++;
             if (counter == 2000) current_state = LIGHT_GREEN;
              break;   
              
        case LIGHT_GREEN: 
            trafficLights = 100;
             LEDS_Update();
             audio = 1;
            update_Audio();
             counter++;
             if (counter == 3000) current_state = PLAY;
         break;
         
        case PLAY:
            if (p1tap == TRUE) current_state = P1_TAP;
            if (p2tap == TRUE) current_state = P2_TAP;
             break;
             
        case P1_TAP:
            p1lock = TRUE;
             arrow++;
            ARROW_SetPosition(arrow);
            ARROW_UpdateArrow();
            if (p2tap == TRUE&&p2lock == FALSE) current_state = P2_TAP;
            if(arrow>26) current_state = P1_WIN;
            else (current_state = IDLE);
             break;
             
        case P2_TAP:
            p2lock = TRUE;
            arrow--;
            ARROW_SetPosition(arrow);
            ARROW_UpdateArrow();
            if (p1tap == TRUE&&p1lock == FALSE) current_state = P1_TAP;
             if(arrow<6) current_state = P2_WIN;
            else (current_state = IDLE);
             break;
             
        case IDLE:
             if (p2tap == TRUE&&p2lock == FALSE) current_state = P2_TAP;
             if (p1tap == TRUE&&p1lock == FALSE) current_state = P1_TAP;
             if (p1tap == FALSE&&p1lock == TRUE) current_state = P1_UNLOCK;
             if (p2tap == FALSE&&p2lock == TRUE) current_state = P2_UNLOCK;
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
            audio = 1;
            update_Audio();
             current_state = GAME_OVER;
                 break;
        case P2_WIN :
            audio = 1;
            update_Audio();
            current_state = GAME_OVER;
                 break;
        case GAME_OVER :
             break;
    }
}         
                
                                           
