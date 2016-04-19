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
 *  These are also already provided by config.h
 */
static bool p1tap;
static bool p2tap;
static bool p1lock;
static bool p2lock;
static int arrow;
static bool audio;
static bool motor;
static bool gloves;
static int lives;
static int modes;
static bool pattern;
static bool hapticFeedback;
static int trafficLights;

static enum {
    INITIALIZE,
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

static void init(void){
     current_state = INITIALIZE ;
     counter = 0;
}

void mode3_fsm(void){

    switch (current_state) {                
        case INITIALIZE:
            motor = FALSE;
            gloves = TRUE;
            lives = 5;
            modes = 2;
            pattern = 0;
            audio = 0;
            hapticFeedback = 0;
            arrow = 16;
            LEDS_Update();
            ARROW_SetPosition(16);
            ARROW_UpdateArrow();
            current_state = LIGHT_RED;
            break;
            
        case LIGHT_RED:
            trafficLights=001;
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
                
                                           
