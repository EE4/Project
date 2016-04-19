//===----------------------------------------------------------------------===//
//  
//  EE4 Project - Finger Tapping Game
//
//  Author: Jelle De Vleeschouwer
//  Created on April 14, 2016, 22:37 PM
//
//===----------------------------------------------------------------------===//

#ifndef __FUNC_AUDIO_H_
#define	__FUNC_AUDIO_H_

#ifdef	__cplusplus
extern "C" {
#endif

#include "config.h"
    
void AUDIO_ISR(void);
void AUDIO_init(void);
void AUDIO_playSound(int to_play);

#ifdef	__cplusplus
}
#endif

#endif	/* __FUNC_AUDIO_H_ */