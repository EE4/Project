/* 
 * File:   func_rand.h
 * Author: jelle
 *
 * Created on April 29, 2016, 9:46 PM
 */

#include "func_rand.h"

//===----------------------------------------------------------------------===//
//  GLOBAL VARIABLES
//===----------------------------------------------------------------------===//

static unsigned char seed = 0;
static bool seeded = 0;

//===----------------------------------------------------------------------===//
//  PRIVATE FUNCTIONS
//===----------------------------------------------------------------------===//

static unsigned char xorshift() {
    seed ^= (seed << 7);
    seed ^= (seed >> 5);
    return seed ^= (seed << 3);
}

//===----------------------------------------------------------------------===//
//  API FUNCTIONS
//===----------------------------------------------------------------------===//

void RAND_seed(unsigned char time)
{
    /* If player 1 tapped any finger and generator isn't seeded yet */
    if (p1_pressed && !seeded) {
        /* Seed generator */
        seed = time;
        
        /* Only seed generator once */
        seeded = TRUE;
    }
}

unsigned char RAND_gen(void)
{
    return xorshift();
}