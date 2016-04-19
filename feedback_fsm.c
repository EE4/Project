/* 
 * File:   feedback_fsm.c
 * Author: jelle
 *
 * Created on April 18, 2016, 9:11 PM
 */

#include "feedback_fsm.h"

//===----------------------------------------------------------------------===//
//  PRIVATE VARIABLES
//===----------------------------------------------------------------------===//

static enum {
    HAPTIC_IDLE = 0,
    HAPTIC_P1,
    HAPTIC_P2,
    HAPTIC_BOTH,
} state;

static unsigned short timer_p1;
static unsigned short timer_p2;

//===----------------------------------------------------------------------===//
//  PUBLIC PROTOTYPES
//===----------------------------------------------------------------------===//

void feedback_fsm(void)
{
    switch (state) {
        case HAPTIC_IDLE:
            
            /* OUTPUTS */
            D_HAP_P1 = 0;
            D_HAP_P2 = 0;
            timer_p1 = 0;
            timer_p2 = 0;
            
            /* TRANSITION CONDITIONS */
            if (feedback_p1 && !feedback_p2) {
                state = HAPTIC_P1;
            } else if (feedback_p2 && !feedback_p2) {
                state = HAPTIC_P2;
            } else if (feedback_p1 && feedback_p2){
                state = HAPTIC_BOTH;
            } else {
                state = HAPTIC_IDLE;
            }
            
            break;
        case HAPTIC_P1:
            
            /* OUTPUTS */
            D_HAP_P1 = 1;
            D_HAP_P2 = 0;
            timer_p1++;
            timer_p2 = 0;
            
            /* Reset notification-flag */
            feedback_p1 = FALSE;
            
            /* TRANSITION CONDITIONS */
            if (feedback_p2) {
                state = HAPTIC_BOTH;
            } else if (timer_p1 == FEEDBACK_DURATION) {
                state = HAPTIC_IDLE;
            } else {
                state = HAPTIC_P1;
            }
       
            break;
        case HAPTIC_P2:
            
            /* OUTPUTS */
            D_HAP_P1 = 0;
            D_HAP_P2 = 1;
            timer_p2++;
            timer_p1 = 0;
            
            /* Reset notification-flag */
            feedback_p2 = FALSE;
            
            /* TRANSITION CONDITIONS */
            if (feedback_p1) {
                state = HAPTIC_BOTH;
            } else if (timer_p2 == FEEDBACK_DURATION) {
                state = HAPTIC_IDLE;
            } else {
                state = HAPTIC_P2;
            }
            
            break;
        case HAPTIC_BOTH:
            
            /* OUTPUTS */
            D_HAP_P1 = 1;
            D_HAP_P2 = 1;
            timer_p1++;
            timer_p2++;
            
            /* Reset notification-flags */
            feedback_p1 = FALSE;
            feedback_p2 = FALSE;
            
            /* TRANSITION CONDITIONS */
            if (FEEDBACK_DURATION == timer_p1 && 
                FEEDBACK_DURATION == timer_p2) {
                state = HAPTIC_IDLE;
            } else if (FEEDBACK_DURATION == timer_p1 &&
                       FEEDBACK_DURATION > timer_p2) {
                state = HAPTIC_P2;
            } else if (FEEDBACK_DURATION == timer_p2 &&
                       FEEDBACK_DURATION > timer_p1) {
                state = HAPTIC_P1;
            } else {
                state = HAPTIC_BOTH;
            }
            
            break;
        default:
            state = HAPTIC_IDLE;
    }
}