/* 
 * File:   display_pattern_fsm.h
 * Author: jelle
 *
 * Created on April 19, 2016, 2:39 PM
 */

#include "display_pattern_fsm.h"

//===----------------------------------------------------------------------===//
//  CONSTANTS
//===----------------------------------------------------------------------===//

/* Delay in ms between 2 successive patterns */
#define PATTERN_DELAY (1000)

//===----------------------------------------------------------------------===//
//  PRIVATE VARIABLES
//===----------------------------------------------------------------------===//

static enum {
    STATE_IDLE = 0,
    STATE_DISPLAY,
    STATE_DELAY,
    STATE_DISPLAY_OFF,
    STATE_DELAY_OFF,
    STATE_WAIT,
    STATE_DONE
} state;

static bool display = FALSE; /* Trigger flag */
static bool playing = FALSE; /**/ 

static unsigned short timer = 0;
static unsigned char patterns[MAX_PATTERNS] = {0};

/*
 * Indexes for keeping track scores afterwards
 */
unsigned char P1_right_counter;
unsigned char P2_right_counter;
static unsigned char index;

//===----------------------------------------------------------------------===//
//  PUBLIC VARIABLES
//===----------------------------------------------------------------------===//

bool pattern_done;

//===----------------------------------------------------------------------===//
//  PRIVATE FUNCTIONS
//===----------------------------------------------------------------------===//

unsigned char pattern_generate(void)
{
    return (RAND_gen() % 4) + 1;
}

//===----------------------------------------------------------------------===//
//  PUBLIC FUNCTIONS
//===----------------------------------------------------------------------===//

void display_pattern_fsm_init(void)
{
    state = STATE_IDLE;
    pattern_done = TRUE;
    timer = 0;
}

void display_pattern(void)
{
    if (!display)
        display = TRUE;
    pattern_done = FALSE;
    playing = FALSE;
}

void enable_checking(void)
{
    playing = TRUE;
}

void display_pattern_fsm(void)
{
    
    switch (state) {
        case STATE_IDLE:
            // *** outputs ***
            timer = 0;
            index = 0;
            P1_right_counter = 0;
            P2_right_counter = 0;
            p1_tapped = NONE;
            p2_tapped = NONE;
            // *** transitions ***
            if (TRUE == display)
                state = STATE_DISPLAY;
            break;
        case STATE_DISPLAY:
            // *** outputs ***
            timer = 0;
            display = FALSE;
            
            if (index < (round << 1)) {
                /* Generate random pattern and store it */
                patterns[index] = pattern_generate();

                /* Display same pattern for both players */
                PATTERN_setPattern(PLAYER_1, patterns[index]);
                PATTERN_setPattern(PLAYER_2, patterns[index]);

                /* Trigger update */
                LEDS_update();
            }
            
            // *** transitions ***
            if (index < (round << 1)) {
                index++;
                state = STATE_DELAY;
            } else {
                state = STATE_WAIT;
            }
            break;
        case STATE_DELAY:
            // *** outputs ***
            timer ++;
            // *** transitions ***
            if (PATTERN_DELAY == timer)
                state = STATE_DISPLAY_OFF;
            break;
        case STATE_DISPLAY_OFF:
            // *** outputs ***
            timer = 0;
            
            PATTERN_setPattern(PLAYER_1, PATTERN_NONE);
            PATTERN_setPattern(PLAYER_2, PATTERN_NONE);
            
            /* Trigger update */
            LEDS_update();
            
            state = STATE_DELAY_OFF;
            break;
        case STATE_DELAY_OFF:
            // *** outputs ***
            timer ++;
            // *** transitions ***
            if (PATTERN_DELAY == timer)
                state = STATE_DISPLAY;
            break;
        case STATE_WAIT:
            // *** outputs ***
            pattern_done = 1;
                
            // *** transitions ***
            if (playing)
                state = STATE_DONE;
            else 
                state = STATE_WAIT;
        case STATE_DONE:
            // *** outputs ***
            if (P1_right_counter < (round << 1)) {
                if (p1_pressed) {
                    if (p1_pressed == patterns[P1_right_counter]) {
                        p1_tapped = CORRECT;
                        P1_right_counter++;
                    } else {
                        p1_tapped = WRONG;
                    }
                } else {
                    p1_tapped = NONE;
                }
            } else {
                p1_tapped = NONE;
            }
            
            if (P2_right_counter < (round << 1)) {
                if (p2_pressed) {
                    if (p2_pressed == patterns[P2_right_counter]) {
                        p2_tapped = CORRECT;
                        P2_right_counter++;
                    } else {
                        p2_tapped = WRONG;
                    }
                } else {
                    p2_tapped = NONE;
                }
            } else {
                p2_tapped = NONE;
            }
            
            // *** transitions ***
            if (display) {
                timer = 0;
                index = 0;
                P1_right_counter = 0;
                P2_right_counter = 0;
                p1_tapped = NONE;
                p2_tapped = NONE;
                state = STATE_DISPLAY;
            } else if (game_ended) {
                state = STATE_IDLE;
            } else {
                state = STATE_DONE;
            }
            break;
        default:
            state = STATE_IDLE;
            break;
    }
}
