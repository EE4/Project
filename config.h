/*********************************************************************
 *
 *                  EE 4 Project - Code Template
 *
 *********************************************************************
 * Processor:       PIC18F2550
 * Compiler:        XC8 1.35+
 * Author:          Jeroen Van Aken
 * Updated:         17/02/2016
 ********************************************************************/

#ifndef _CONFIG_H
#define _CONFIG_H

/** I N C L U D E S *************************************************/
#include <xc.h>

/** LOW LEVEL FUNCTIONS **/
#include "func_score.h"
#include "func_audio.h"
#include "func_pwm.h"
#include "func_display.h"
#include "feedback_fsm.h"
#include "p1_tapping_fsm.h"
#include "p2_tapping_fsm.h"

/** GAME LEVEL LOGIC **/
//#include "mode1_fsm.h"
//#include "mode2_fsm.h"
//#include "mode3_fsm.h"

//===----------------------------------------------------------------------===//
//  DEFINES
//===----------------------------------------------------------------------===//
#define TRUE        (1)
#define FALSE       (0)

#define OUTPUT      ((unsigned)0)
#define INPUT       ((unsigned)1)

#define NONE        (0)
#define INDEX       (1)
#define MIDDLE      (2)
#define RING        (3)
#define PINKY       (4)

/* LATCH REGISTERS */
#define D_AUDIO     LATCbits.LC2
#define D_MODE1     LATCbits.LC1
#define D_MODE2     LATCbits.LC6
#define D_MODE3     LATCbits.LC7
#define D_HAP_P1    LATBbits.LB0
#define D_HAP_P2    LATBbits.LB1
#define D_CTRL_P1   LATBbits.LB2
#define D_CTRL_P2   LATBbits.LB3
#define D_INDEX     PORTBbits.RB4
#define D_MIDDLE    PORTBbits.RB5
#define D_RING      PORTBbits.RB6
#define D_PINKY     PORTBbits.RB7
#define D_STROBE    LATAbits.LA0
#define D_SERIAL_O  LATAbits.LA1
#define D_SERIAL_CK LATAbits.LA2
#define D_SERIAL_OE LATAbits.LA3
#define D_SCORE     LATAbits.LA4
#define D_SERVO     LATAbits.LA5

/* TRI-STATE REGISTERS */
#define T_AUDIO     TRISCbits.RC2
#define T_MODE1     TRISCbits.RC1
#define T_MODE2     TRISCbits.RC6
#define T_MODE3     TRISCbits.RC7
#define T_HAP_P1    TRISBbits.RB0
#define T_HAP_P2    TRISBbits.RB1
#define T_CTRL_P1   TRISBbits.RB2
#define T_CTRL_P2   TRISBbits.RB3
#define T_INDEX     TRISBbits.RB4
#define T_MIDDLE    TRISBbits.RB5
#define T_RING      TRISBbits.RB6
#define T_PINKY     TRISBbits.RB7
#define T_STROBE    TRISAbits.RA0
#define T_SERIAL_O  TRISAbits.RA1
#define T_SERIAL_CK TRISAbits.RA2
#define T_SERIAL_OE TRISAbits.RA3
#define T_SCORE     TRISAbits.RA4
#define T_SERVO     TRISAbits.RA5

//===----------------------------------------------------------------------===//
//  TYPE DEFINITIONS
//===----------------------------------------------------------------------===//
typedef int bool;
typedef unsigned char uint8_t;

/* Possible players */
typedef enum player {
    PLAYER_1 = 0,
    PLAYER_2,
} player_t;

#define PLAYER_1    (0)
#define PLAYER_2    (1)

/* Possible pattern to display */
typedef enum pattern {
    PATTERN_NONE = 0,
    PATTERN_INDEX,
    PATTERN_MIDDLE,
    PATTERN_RING,
    PATTERN_PINKY
} pattern_t;

/* Possible sounds to play; */
typedef enum sound_e {
    SOUND_NONE = 0,
    SOUND_TAP,
    SOUND_SELECT,
    SOUND_READY,
    SOUND_SET,
    SOUND_GO,
    SOUND_WON,
    SOUND_LOST
} sound_t;

/* Possible player states */
typedef enum player_state {
    STATE_NONE = 0,
    STATE_GO,
    STATE_SET,
    STATE_READY
} player_state_t;
//===----------------------------------------------------------------------===//


//===----------------------------------------------------------------------===//
//  GLOBAL VARIABLES
//===----------------------------------------------------------------------===//
// when a variable is declared 'extern' it also has to be declared in
// the corresponding .c file without the 'extern' keyword

//===----------------------------------------------------------------------===//
//  FINGER TAPPING
//===----------------------------------------------------------------------===//
extern unsigned char p1_pressed;
extern unsigned char p2_pressed;

//===----------------------------------------------------------------------===//
//  HAPTIC FEEDBACK
//===----------------------------------------------------------------------===//
/* Duration of feedback in ms */
#define FEEDBACK_DURATION (200)

extern bool feedback_p1;
extern bool feedback_p2;

//===----------------------------------------------------------------------===//
//  API
//===----------------------------------------------------------------------===//
void hardware_init(void);
unsigned char timed_to_1ms(void);
void interrupt interrupt_handler(void);

#endif
//EOF----------------------------------------------------------------
