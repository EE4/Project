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

void enable_tapping(void)
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
            // *** transitions ***
            if (TRUE == display)
                state = STATE_DISPLAY;
            break;
        case STATE_DISPLAY:
            // *** outputs ***
            timer = 0;
            
            /* Generate random pattern and store it */
            patterns[index] = pattern_generate();
            
            /* Display same pattern for both players */
            PATTERN_setPattern(PLAYER_1, patterns[index]);
            PATTERN_setPattern(PLAYER_2, patterns[index]);
            
            /* Trigger update */
            LEDS_update();
            
            /* Increment index for next pattern */
            index++;
            
            // *** transitions ***
            if (index < 10) {
                state = STATE_DELAY;
            } else {
                state = STATE_WAIT;
                pattern_done = 1;
            }
            break;
        case STATE_DELAY:
            // *** outputs ***
            timer ++;
            // *** transitions ***
            if (PATTERN_DELAY == timer)
                state = STATE_DISPLAY;
            break;
        case STATE_WAIT:
            if (playing)
                state = STATE_DONE;
            else 
                state = STATE_WAIT;
        case STATE_DONE:
            // *** outputs ***
            if (p1_pressed == patterns[P1_right_counter]) {p1_tapped = CORRECT; P1_right_counter++;}
            else if (!p1_pressed) p1_tapped = NONE;
            else p1_tapped = WRONG;
            
            if (p2_pressed == patterns[P2_right_counter]) {p2_tapped = CORRECT; P2_right_counter++;}
            else if (!p2_pressed) p2_pressed = NONE;
            else p2_pressed = WRONG;
            /* TODO: Check for taps, and compare if a player is correct or not */
            
            
            // *** transitions ***
            if (round_ended) {
                display = FALSE;
                state = STATE_IDLE;
            }
            break;
        default:
            state = STATE_IDLE;
            break;
    }
}
