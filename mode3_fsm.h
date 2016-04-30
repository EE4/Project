/* 
 * File:   p1_tapping_fsm.h
 * Author: gil
 *
 * Created on April 12, 2016, 3:27 PM
 */

#ifndef __P3_TAPPING_FSM_
#define	__P3_TAPPING_FSM_

#ifdef	__cplusplus
extern "C" {
#endif

#include "config.h"
    
void mode3_fsm_init(void);  // Initialisation
void mode3_fsm_play(void);  // Call to trigger mode3 FSM
void mode3_fsm(void);       // Cycle through mode3 FSM once

#ifdef	__cplusplus
}
#endif

#endif	/* __P3_TAPPING_FSM_ */
