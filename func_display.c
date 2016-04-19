/* 
 * File:   func_display.h
 * Author: jelle
 *
 * Created on April 18, 2016, 9:45 PM
 */

#include "func_display.h"

//===----------------------------------------------------------------------===//
//  CONSTANTS
//===----------------------------------------------------------------------===//
#define LIVES_MASK  (0x70)
#define STATE_MASK  (0x8F)

//===----------------------------------------------------------------------===//
//  PRIVATE  VARIABLES
//===----------------------------------------------------------------------===//

/*
 *  Q1 - INDEX  : shift 0
 *  Q2 - MIDDLE : shift 1
 *  Q3 - RING   : shift 2
 *  Q4 - PINKY  : shift 3
 *  Q5 - N/C
 *  Q6 - N/C
 *  Q7 - N/C
 *  Q8 - N/C
 */
static uint8_t pattern_p1;
static uint8_t pattern_p2;

/*
 *  Q1 - LIVE 2
 *  Q2 - LIVE 3
 *  Q3 - LIVE 4
 *  Q4 - LIVE 5
 *  Q5 - GREEN
 *  Q6 - YELLOW
 *  Q7 - RED
 *  Q8 - LIVE 1
 */
static uint8_t lives_p1;
static uint8_t lives_p2;

//===----------------------------------------------------------------------===//
//  PRIVATE PROTOTYPES
//===----------------------------------------------------------------------===//

static unsigned char PATTERN_translate(pattern_t pattern)
{
    if (PATTERN_NONE == pattern)
        return (unsigned char)0;
    
    /* Shift according to pattern */
    return (0x01 << ((int)pattern - 1));
}

static unsigned char LIVES_translate(unsigned char lives)
{
    /* 5 leds on -> shifted to the right how many lives there are left:
     * f.e. lives = 0 means shifting to the right 5 times, nothing left 
     *      lives = 5 means shifting to the right 0 times, 5 lives left 
     */
    unsigned char gauge_lives = 0x1F >> (5 - lives);
    
    /* Now format bits like they are connected to the SIPO */
    return (unsigned char)((gauge_lives >> 1) | (gauge_lives << 7));
}

static unsigned char LIVE_inv_translate(unsigned char lives)
{
    /* Mask out state bits from live display storage */
    unsigned char masked = lives & ~LIVES_MASK;
    
    /* Inverse formatign of how leds are connected to SIPO */
    unsigned char gauge_lives = (masked >> 7) | (masked << 1);
    
    unsigned char lives_left = 0; 
    
    /* Count while the value of the gauge lives is still true */
    while (masked) {
        /* Shift out one live and increment lives_left */
        masked = masked >> 1;
        lives_left++;
    }
    
    return lives_left;
}

static unsigned char STATE_translate(player_state_t state)
{
    unsigned char _state = 0x01 << (state - 1);
    
    if (STATE_NONE == state)
        return (unsigned char)0;
    
    /* Now format bits like they are connected to the SIPO */
    return (unsigned char)(_state << 4);
}


static void SIPO_do_write_byte(char a)
{
    /* Shift out 8 bits on rising clock edge */
    for (int i = 0; i < 8; i++) {
        /* Put bit at index on data pin */
        D_SERIAL_O = (a >> i) & 0x01;

        /* Give a rising edge to sample data-pin */
        D_SERIAL_CK = 1;

        /* Give certain amount of time to sample */
        NOP();

        /* Turn off CLK */
        D_SERIAL_CK = 0;
    }
}

//===----------------------------------------------------------------------===//
//  PUBLIC PROTOTYPES
//===----------------------------------------------------------------------===//

void PATTERN_setPattern(player_t player, pattern_t pattern)
{   
    if (PLAYER_1 == player) {
        pattern_p1 = PATTERN_translate(pattern);
    } else if (PLAYER_2 == player) {
        pattern_p2 = PATTERN_translate(pattern);
    } else {
        /* Do nothing */
    }
}

void STATE_setState(player_t player, player_state_t state)
{
    if (PLAYER_1 == player) {
        lives_p1 = (lives_p1 & STATE_MASK) | STATE_translate(state);
    } else if (PLAYER_2 == player) {
        lives_p2 = (lives_p2 & STATE_MASK) | STATE_translate(state);
    } else {
        /* Do nothing */
    }
}

void LIVES_setLives(player_t player, unsigned char lives)
{   
    if (PLAYER_1 == player) {
        lives_p1 = (lives_p1 & LIVES_MASK) | STATE_translate(lives);
    } else if (PLAYER_2 == player) {
        lives_p2 = (lives_p2 & LIVES_MASK) | STATE_translate(lives);
    } else {
        /* Do nothing */
    }
}

unsigned char LIVES_getLives(player_t player)
{
    if (PLAYER_1 == player) {
        return LIVE_inv_translate(lives_p1);
    } else if (PLAYER_2 == player) {
        return LIVE_inv_translate(lives_p2);
    } else {
        /* Do nothing */
        return 0;
    }
}

void LEDS_update(void)
{
    /* Output enable on */
    D_SERIAL_OE = 1;
    
    /*
     *  UPDATE THIS ORDER ACCORDING TO HOW
     *  THE DIFFERENT SIPO'S ARE CONNECTED IN
     *  THE CHAIN.
     */
    SIPO_do_write_byte(lives_p1);
    SIPO_do_write_byte(pattern_p1);
    SIPO_do_write_byte(pattern_p2);
    SIPO_do_write_byte(lives_p2);
    
    /* Strobe high to put serial data on 
     * parallel outputs */
    D_STROBE = 1;

    /* Give the SIPO the time to sample strobe and
     * to put data on outputs */
    NOP();

    /* Strobe low */
    D_STROBE = 0;
}