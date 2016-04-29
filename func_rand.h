/* 
 * File:   func_rand.h
 * Author: jelle
 *
 * Created on April 29, 2016, 9:46 PM
 */

#ifndef __FUNC_RAND_H_
#define	__FUNC_RAND_H_

#ifdef	__cplusplus
extern "C" {
#endif
    
#include "config.h"

void RAND_seed(unsigned char time);
unsigned char RAND_gen(void);

#ifdef	__cplusplus
}
#endif

#endif	/* __FUNC_RAND_H_ */

