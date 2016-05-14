/*
 * File:   func_display.h
 * Author: jelle
 *
 * Created on April 18, 2016, 9:45 PM
 */

#include "func_display.h"
#include "config.h"

//===----------------------------------------------------------------------===//
//  CONSTANTS
//===----------------------------------------------------------------------===//
#define LIVES_MASK  (0x0E)
#define STATE_MASK  (0xF1)

//===----------------------------------------------------------------------===//
//  PUBLIC  VARIABLES
//===----------------------------------------------------------------------===//
bool mode_idle;

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
 *  Q8 - LIVE 2
 *  Q7 - LIVE 3
 *  Q6 - LIVE 4
 *  Q5 - LIVE 5
 *  Q4 - GO
 *  Q3 - SET
 *  Q2 - READY
 *  Q1 - LIVE 1
 */
static uint8_t lives_p1;
static uint8_t lives_p2;

static bool leds_update;

static enum {
    PRE = 0,
    LIVES_P2,
    PATTERN_P2,
    PATTERN_P1,
    LIVES_P1,
    POST  
} state;


//===----------------------------------------------------------------------===//
//  PRIVATE PROTOTYPES
//===----------------------------------------------------------------------===//

static unsigned char PATTERN_translate(unsigned char pattern)
{
    if (PATTERN_NONE == pattern)
        return 0;
    else if (PATTERN_ALL == pattern)
        return 0xFF;

    /* Shift according to pattern */
    return (uint8_t)(0x80 >> (uint8_t)((int)pattern) - 1);
}

static unsigned char LIVES_translate(unsigned char lives)
{
    /* 5 leds on -> shifted to the left how many lives there are left:
     * f.e. lives = 0 means shifting to the right 5 times, nothing left
     *      lives = 5 means shifting to the right 0 times, 5 lives left
     * 
     * 1111 1000
     *  |
     *  V (lives = 3)
     * 1110 0000
     *  |
     *  V (gauge_lives >> 7)    | (gauge_lives << 1)
     * 0000 0001                | 1100 0000
     *  |
     *  V
     * 1100 0001
     * 
     */
    
    unsigned char gauge_lives = 0xF8 << (5 - lives);
    
    /* Now format bits like they are connected to the SIPO */
    return (unsigned char)((gauge_lives >> 7) | (gauge_lives << 1));
}

static unsigned char LIVE_inv_translate(unsigned char lives)
{
    /* Mask out state bits from live display storage */
    unsigned char masked = lives & STATE_MASK;

    /* Inverse formating of how LEDs are connected to SIPO */
    unsigned char gauge_lives = (masked << 7) | (masked >> 1);

    unsigned char lives_left = 0;

    /* Count while the value of the gauge lives is still true */
    while (gauge_lives) {
        /* Shift out one live and increment lives_left */
        gauge_lives = gauge_lives << 1;
        lives_left++;
    }

    return lives_left;
}


static unsigned char STATE_translate(unsigned char state)
{
    unsigned char _state = 0x01 << (state - 1);

    if (STATE_NONE == state)
        return (unsigned char)0;
    if (STATE_ALL == state)
        return (unsigned char)0xFF;

    /* Now format bits like they are connected to the SIPO */
    return (unsigned char)(_state << 1);
}

static void MODE_doSet(unsigned char mode, bool value)
{
    /* Don't put PWM on mode display */
    mode_idle = FALSE;
    
    switch (mode) {
        case 0:
            break;
        case MODE_1:
            D_MODE1 = value;
            D_MODE2 = FALSE;
            D_MODE3 = FALSE;
            break;
        case MODE_2:
            D_MODE2 = value;
            D_MODE1 = FALSE;
            D_MODE3 = FALSE;
            break;
        case MODE_3:
            D_MODE3 = value;
            D_MODE1 = FALSE;
            D_MODE2 = FALSE;
            break;
        default:
            if (MODE_ALL == mode) {
                D_MODE3 = value;
                D_MODE1 = value;
                D_MODE2 = value;
            }
                    
            /* Do nothing */
            NOP();
    }
}

static void SIPO_do_write_bit(char a, int index)
{
    unsigned short i = 0;
    /* Put bit at index on data pin */
    D_SERIAL_O = ((a >> index) & 0x01);
    
    /* Clock pulse */
    D_SERIAL_CK = 1;
    D_SERIAL_CK = 0;
}

//===----------------------------------------------------------------------===//
//  PUBLIC PROTOTYPES
//===----------------------------------------------------------------------===//

void PATTERN_setPattern(unsigned char player, unsigned char pattern)
{
    if (PLAYER_1 == player) {
        pattern_p1 = PATTERN_translate(pattern);
    } else if (PLAYER_2 == player) {
        pattern_p2 = PATTERN_translate(pattern);
    } else {
        /* Do nothing */
    }
}

void STATE_setState(unsigned char player, unsigned char state)
{
    if (PLAYER_1 == player) {
        lives_p1 = (lives_p1 & STATE_MASK) | STATE_translate(state);
    } else if (PLAYER_2 == player) {
        lives_p2 = (lives_p2 & STATE_MASK) | STATE_translate(state);
    } else {
        /* Do nothing */
    }
    
}

void LIVES_setLives(unsigned char player, unsigned char lives)
{
    if (PLAYER_1 == player) {
        lives_p1 = (lives_p1 & LIVES_MASK) | LIVES_translate(lives);
    } else if (PLAYER_2 == player) {
        lives_p2 = (lives_p2 & LIVES_MASK) | LIVES_translate(lives);
    } else {
        /* Do nothing */
    }
}

unsigned char LIVES_getLives(unsigned char player)
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

void MODE_setMode(unsigned char mode)
{
    mode_idle = FALSE;
    MODE_doSet(mode, TRUE);
}

void MODE_clrMode(unsigned char mode)
{
    mode_idle = FALSE;
    MODE_doSet(mode, FALSE);
}

static unsigned char bit_count;

void LEDS_init(void)
{
    D_SERIAL_OE = FALSE;
    D_SERIAL_CK = FALSE;
    D_SERIAL_O = FALSE;
    D_STROBE = FALSE;
    
    state = PRE;
    bit_count = 0;
}


void LEDS_tick(void)
{
    D_SCORE = 1;
    
    switch (state) {
        case PRE:

            /* Output enable on */
            D_SERIAL_OE = 1;
            
            if (leds_update)
                state = LIVES_P2;
            else 
                state = PRE;
            
            /* Make sure the updating can be triggered again during updating */
            leds_update = 0;

            break;
        case LIVES_P2:
            
            SIPO_do_write_bit(lives_p2, bit_count);
            
            if (++bit_count == 8) {
                bit_count = 0;
                state = PATTERN_P2;
            } else {
                state = LIVES_P2;
            }
            
            break;
        case PATTERN_P2:
            
            SIPO_do_write_bit(pattern_p2, bit_count);
            
            if (++bit_count == 8) {
                bit_count = 0;
                state = PATTERN_P1;
            } else {
                state = PATTERN_P2;
            }
            
            break;
        case PATTERN_P1:
            
            SIPO_do_write_bit(pattern_p1, bit_count);
            
            if (++bit_count == 8) {
                bit_count = 0;
                state = LIVES_P1;
            } else {
                state = PATTERN_P1;
            }
            
            break;
        case LIVES_P1:
            
            SIPO_do_write_bit(lives_p1, bit_count);
            
            if (++bit_count == 8) {
                bit_count = 0;
                state = POST;
            } else {
                state = LIVES_P1;
            }
            
            break;
        case POST:

            /* Strobe pulse to put serial data on
             * parallel outputs */
            D_STROBE = 1;

            /* Strobe low */
            D_STROBE = 0;

            /* Output enable high */
            D_SERIAL_OE = 1;
    
            /* Loop */
            state = PRE;
            
            break;
    }
}

void LEDS_update(void)
{
    leds_update = 1;
}
