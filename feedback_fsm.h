/* 
 * File:   feedback_fsm.h
 * Author: jelle
 *
 * Created on April 18, 2016, 9:11 PM
 */

#ifndef FEEDBACK_FSM_H
#define	FEEDBACK_FSM_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include "config.h"

//===----------------------------------------------------------------------===//
//  API PROTOTYPES
//===----------------------------------------------------------------------===//

void FEEDBACK_giveFeedback(unsigned char player);

//===----------------------------------------------------------------------===//
//  PUBLIC PROTOTYPES
//===----------------------------------------------------------------------===//

void feedback_fsm_init(void);
void feedback_fsm(void);

#ifdef	__cplusplus
}
#endif

#endif	/* FEEDBACK_FSM_H */

