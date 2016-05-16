/* 
 * File:   p1_tapping_fsm.h
 * Author: jelle
 *
 * Created on April 12, 2016, 3:27 PM
 */

#ifndef __P1_TAPPING_FSM
#define	__P1_TAPPING_FSM

#ifdef	__cplusplus
extern "C" {
#endif
    
#include "config.h"
    
//===----------------------------------------------------------------------===//
//  PUBLIC PROTOTYPES
//===----------------------------------------------------------------------===//
    
void p1_tap_display_enable(int enable);
void p1_tapping_fsm_init(void);
void p1_tapping_fsm(void);

#ifdef	__cplusplus
}
#endif

#endif	/* __P1_TAPPING_FSM */

