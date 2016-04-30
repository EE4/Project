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
    STATE_DONE
} state;

static bool display = FALSE; /* Trigger flag */

static unsigned short timer = 0;
static unsigned char patterns[MAX_PATTERNS] = {0};

/*
 * Indexes for keeping track scores afterwards
 */
static unsigned char p1_index;
static unsigned char p2_index;

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
}

void display_pattern_fsm(void)
{
    unsigned char index;
    
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
            if (index != round * 2) {
                state = STATE_DELAY; /* Unconditional transition */
            } else {
                state = STATE_DONE;
                pattern_done = TRUE;
            }
            break;
        case STATE_DELAY:
            // *** outputs ***
            timer ++;
            // *** transitions ***
            if (PATTERN_DELAY == timer)
                state = STATE_DISPLAY;
            break;
        case STATE_DONE:
            // *** outputs ***
            
            /* TODO: Check for taps, and compare if a player is correct or not */
            
            // *** transitions ***
            
            break;
        default:
            state = STATE_IDLE;
            break;
    }
}