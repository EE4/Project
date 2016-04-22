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

static unsigned short timer = 0;

static pattern_t[];

//===----------------------------------------------------------------------===//
//  PUBLIC VARIABLES
//===----------------------------------------------------------------------===//

bool pattern_done;

//===----------------------------------------------------------------------===//
//  PUBLIC FUNCTIONS
//===----------------------------------------------------------------------===//
void pattern_fsm_init(void)
{
    state = STATE_IDLE;
    pattern_done = FALSE;
    timer = 0;
}

void display_pattern_fsm(void)
{
    
}