//===----------------------------------------------------------------------===//
//  
//  EE4 Project - Finger Tapping Game
//
//  Author: Jelle De Vleeschouwer
//  Updated: 16/04/2016
//
//===----------------------------------------------------------------------===//

#ifndef __FUNC_SCORE_H_
#define	__FUNC_SCORE_H_

#include "config.h"

//===----------------------------------------------------------------------===//
//  PUBLIC PROTOTYPES
//===----------------------------------------------------------------------===//

/* Negative score indicated a win of player 1, a positive score indicates
 * a win of player 2 */
int SCORE_getScore(void);

/* Set the absolute score immediatelly */
void SCORE_setScore(int absolute);

/* Update the absolute score with a relative score */
void SCORE_updateScore(int relative);

#endif

