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

void RAND_seed(unsigned short time)
{
    /* If player 1 tapped any finger */
    if (p1_pressed) {
        /* Seed generator */
        seed = (unsigned char)((unsigned char)(time >> 3) ^ (unsigned char)time) | p1_pressed;
    }
}

unsigned char RAND_gen(void)
{
    return xorshift();
}