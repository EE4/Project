/* 
 * File:   display_pattern_fsm.h
 * Author: jelle
 *
 * Created on April 19, 2016, 2:39 PM
 */

#include "display_pattern_fsm.h"

//===----------------------------------------------------------------------===//
//  PRIVATE VARIABLES
//===----------------------------------------------------------------------===//

static enum {
    STATE_IDLE = 0,
    STATE_DISPLAY,
    STATE_DELAY,
    STATE_DONE,
} state;

//===----------------------------------------------------------------------===//
//  PRIVATE VARIABLES
//===----------------------------------------------------------------------===//

static unsigned short timer = 0;
static unsigned char patterns[MAX_PATTERNS] = {0};

//===----------------------------------------------------------------------===//
//  PUBLIC VARIABLES
//===----------------------------------------------------------------------===//

bool pattern_done;

static unsigned long long xor128(void) 
{
    static unsigned long long x = 123456789;
    static unsigned long long y = 362436069;
    static unsigned long long z = 521288629;
    static unsigned long long w = 88675123;
    unsigned long long t;
    t = x ^ (x << 11);   
    x = y; y = z; z = w;   
    return w = w ^ (w >> 19) ^ (t ^ (t >> 8));
}
 

//===----------------------------------------------------------------------===//
//  PUBLIC FUNCTIONS
//===----------------------------------------------------------------------===//

void display_pattern_fsm_init(void)
{
    state = STATE_IDLE;
    pattern_done = FALSE;
    timer = 0;
}

void display_pattern_fsm(void)
{
    switch (state) {
        case STATE_IDLE:
            // *** outputs ***
                
            // *** transitions ***
            
            break;
        case STATE_DISPLAY:
            // *** outputs ***

            // *** transitions ***
            
            break;
        case STATE_DELAY:
            // *** outputs ***

            // *** transitions ***
            
            break;
        case STATE_DONE:
            // *** outputs ***

            // *** transitions ***
            
            break;
        default:
            state = STATE_IDLE;
            break;
    }
}