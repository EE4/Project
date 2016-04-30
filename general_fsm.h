/* 
 * File:   general_fsm.h
 * Author: jelle
 *
 * Created on April 30, 2016, 9:54 AM
 */

#ifndef __GENERAL_FSM_H_
#define	__GENERAL_FSM_H_

#ifdef	__cplusplus
extern "C" {
#endif
    
#include "config.h"
    
//===----------------------------------------------------------------------===//
//  API FUNCTIONS
//===----------------------------------------------------------------------===//
    
void general_fsm_init(void);
void general_fsm(void);

#ifdef	__cplusplus
}
#endif

#endif	/* __GENERAL_FSM_H_ */

