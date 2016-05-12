/* 
 * File:   tapping_fsm.h
 * Author: jelle
 *
 * Created on May 9, 2016, 9:03 PM
 */

#ifndef TAPPING_FSM_H
#define	TAPPING_FSM_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include "config.h"
    
//===----------------------------------------------------------------------===//
//  PUBLIC PROTOTYPES
//===----------------------------------------------------------------------===//

void tapping_fsm_init(void);
void tapping_fsm(void);
void tapping_tick(void);

#ifdef	__cplusplus
}
#endif

#endif	/* TAPPING_FSM_H */

