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
#include <pic18f2550.h>

/** LOW LEVEL FUNCTIONS **/
#include "func_score.h"
#include "func_audio.h"
#include "func_pwm.h"
#include "func_display.h"
#include "func_rand.h"
#include "feedback_fsm.h"
#include "p1_tapping_fsm.h"
#include "p2_tapping_fsm.h"


/** GAME LEVEL LOGIC **/
#include "general_fsm.h"
//#include "mode1_fsm.h"
//#include "mode2_fsm.h"
#include "mode3_fsm.h"

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
#define PLAYER_1        (0u)
#define PLAYER_2        (1u)

/* Possible patterns to display */
#define PATTERN_NONE    (0u)
#define PATTERN_INDEX   (1u)
#define PATTERN_MIDDLE  (2u)
#define PATTERN_RING    (3u)
#define PATTERN_PINKY   (4u)
#define PATTERN_ALL     (5u)

/* Possible sounds to play */
#define SOUND_NONE      (0u)
#define SOUND_TAP       (1u)
#define SOUND_SELECT    (2u)
#define SOUND_READY     (4u)
#define SOUND_SET       (5u)
#define SOUND_GO        (6u)
#define SOUND_WON       (7u)
#define SOUND_LOST      (8u)

typedef char sound_t;

/* Possible player states */
#define STATE_NONE      (0u)
#define STATE_GO        (1u)
#define STATE_SET       (2u)
#define STATE_READY     (3u)

/* Possible game modes */
#define MODE_1          (1u)
#define MODE_2          (2u)
#define MODE_3          (3u)

//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
//  GLOBAL VARIABLES
//===----------------------------------------------------------------------===//
// when a variable is declared 'extern' it also has to be declared in
// the corresponding .c file without the 'extern' keyword

//===----------------------------------------------------------------------===//
//  GLOBAL GAME VARIABLES
//===----------------------------------------------------------------------===//
extern unsigned char round;
extern bool game_ended;

//===----------------------------------------------------------------------===//
//  RANDOM NUMBER GENERATOR
//===----------------------------------------------------------------------===//
#define MAX_RAND (255u)

//===----------------------------------------------------------------------===//
//  MODE DISPLAY
//===----------------------------------------------------------------------===//
extern bool mode_idle;

//===----------------------------------------------------------------------===//
//  PWM
//===----------------------------------------------------------------------===//
#define PWM_CHANNELS    2
#define SERVO_CHANNEL   0
#define MODE_CHANNEL    1

extern unsigned short PWM_duty[PWM_CHANNELS];

//===----------------------------------------------------------------------===//
//  PATTERN DISPLAY
//===----------------------------------------------------------------------===//

#define MAX_PATTERNS (10u)

extern bool pattern_done;

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

//===----------------------------------------------------------------------===//
//  API
//===----------------------------------------------------------------------===//
void hardware_init(void);
unsigned char timed_to_1ms(void);
void interrupt interrupt_handler(void);

#endif
//EOF----------------------------------------------------------------
