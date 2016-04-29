/* 
 * File:   func_display.h
 * Author: jelle
 *
 * Created on April 18, 2016, 9:45 PM
 */

#ifndef __FUNC_DISPLAY_H_
#define	__FUNC_DISPLAY_H_

#ifdef	__cplusplus
extern "C" {
#endif

#include "config.h"

//===----------------------------------------------------------------------===//
//  API PROTOTYPES
//===----------------------------------------------------------------------===//

/*
 *  Function to set a LED-buffer with
 */
void PATTERN_setPattern(unsigned char, unsigned char);
void STATE_setState(unsigned char, unsigned char);
void LIVES_setLives(unsigned char, unsigned char);
unsigned char LIVES_getLives(unsigned char);
void MODE_setMode(unsigned char);
void MODE_clrMode(unsigned char);

/*
 *  Function to update entire SIPO-chain
 */
void LEDS_init(void);
void LEDS_update(void);

#ifdef	__cplusplus
}
#endif

#endif	/* __FUNC_DISPLAY_H_ */

