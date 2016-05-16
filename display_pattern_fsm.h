/* 
 * File:   display_pattern_fsm.h
 * Author: jelle
 *
 * Created on April 19, 2016, 2:39 PM
 */

#ifndef __DISPLAY_PATTERN_FSM_H_
#define	__DISPLAY_PATTERN_FSM_H_

#ifdef	__cplusplus
extern "C" {
#endif

#include "config.h"

//===----------------------------------------------------------------------===//
//  PUBLIC FUNCTIONS
//===----------------------------------------------------------------------===//

unsigned char pattern_generate(void);
void display_pattern_fsm_init(void);
void display_pattern_fsm(void);
void display_pattern(void);
void enable_checking(void);

#ifdef	__cplusplus
}
#endif

#endif	/* __DISPLAY_PATTERN_FSM_H_ */

