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
//  PUBLIC PROTOTYPES
//===----------------------------------------------------------------------===//

void PATTERN_setPattern(player_t player, pattern_t pattern);
void STATE_setState(player_t player, player_state_t state);
void LIVES_setLives(player_t player, unsigned char lives);
unsigned char LIVES_getLives(player_t player);

#ifdef	__cplusplus
}
#endif

#endif	/* __FUNC_DISPLAY_H_ */

