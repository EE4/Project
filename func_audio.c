//===----------------------------------------------------------------------===//
//
//  EE4 Project - Finger Tapping Game
//
//  Author: Jelle De Vleeschouwer
//  Created on April 14, 2016, 22:37 PM
//
//===----------------------------------------------------------------------===//

#include "func_audio.h"
#include "sounds.h"
#include <pic18f2550.h>
#include <xc.h>

//===----------------------------------------------------------------------===//
//  DEFINES
//===----------------------------------------------------------------------===//
#define F_OSC           (12000000u)                         /* Instruction clock */
#define F_PWM           (100000u)                           /* PWM-frequency 100kHz */
#define C_PWM           ((F_OSC / F_PWM) - 1)               /* Overflow value for PWM CLK */

#define F_SAMPLE        (8000u)                             /* Sampling frequency 8kHz */
#define D_SAMPLE        (16u)                               /* Prescaler for sample clk (Divider) */
#define C_SAMPLE        (((F_OSC / D_SAMPLE) / F_SAMPLE))   /* Overflow value for Sample CLK */
#define P_SAMPLE        (256 - C_SAMPLE)                    /* Overflow after C_SAMPLE counts */

//===----------------------------------------------------------------------===//
//  PRIVATE VARIABLES
//===----------------------------------------------------------------------===//
static long sample = 0;

static long sound_length;
static const unsigned char *sound;

//===----------------------------------------------------------------------===//
//  PUBLIC VARIABLES
//===----------------------------------------------------------------------===//

bool sound_done;

//===----------------------------------------------------------------------===//
//  PRIVATE DECLARATIONS
//===----------------------------------------------------------------------===//
void AUDIO_setupPWM(void)
{
    /* PWM frequency:
     *
     *  Instruction freq.       = 12 MHz
     *  Prescaler               = 1
     *  Result freq.            = 12 MHz
     *  Required freq.          = 100 kHz
     *  Counts                  = 120
     *  PR2 (counts - 1)        = 119
     */

    T2CON = 0b00000000;     /* Timer 2 control register
                             *  bit   7: Unimplemented
                             *  bit 6-3: Output postscale select (0000 -> 1:1)
                             *  bit   2: Timer on/off (1 -> On)
                             *  bit 1-0: Clock prescale select (01 -> 1/4)
                             */

    IPR1bits.TMR2IP = 0;
    PIR1bits.TMR2IF = 0;    /* Clear Timer 2 interrupt flag */
    PIE1bits.TMR2IE = 0;    /* Disable interrupts for Timer 2 */

    PR2 = C_PWM;   /* Overflow value for PWM CLK */

    CCPR1L = 0x80;          /* Initial dutycycle of 50% (128) */

    /* Configure CCP-module 2 for PWM */
    CCP1CON = 0b00001100;   /* Capture/Compare/PWM-module 2 config register
                             *  bit 7-6: Unimplemented
                             *  bit 5-4: PWM Duty Cycle LSB's (00 -> 00)
                             *  bit 3-0: Mode select (1100 -> PWM mode)
                             */

    T2CONbits.TMR2ON = 1;   /* Turn on Timer 2 */
}

void AUDIO_setupSampler(void)
{
    T0CON = 0x43;           /* Timer0 Control Register
                             *  bit7 "0": Disable Timer
                             *  bit6 "1": 8-bit timer
                             *  bit5 "0": Internal clock
                             *  bit4 "0": not important in Timer mode
                             *  bit3 "0": Timer0 prescale is assigned
                             *  bit2-0 "011": Prescale 1:16
                             */

    /* Timer frequency:
     *
     *  Instruction freq.       = 12 MHz
     *  Prescaler               = 16
     *  Result freq.            = 75 kHz
     *  Sampling freq.          = 8 kHz
     *  Counts                  = 93
     *  TMR0L (256 - Counts)    = 163
     *
     */

    TMR0L = (char)P_SAMPLE; /* Overflow after C_SAMPLE counts */

    INTCONbits.TMR0IE = 1;
    INTCONbits.TMR0IF = 0;
    
    /* Make sampler high priority */
    INTCON2bits.TMR0IP = 1;
    
    /* Enable Timer 0 */
    T0CONbits.TMR0ON = 1;
}

static void AUDIO_update_sample(const unsigned char a)
{
    CCPR1L = a >> 2;
    CCP1CONbits.DC1B1 = (a & 0x02) >> 1;
    CCP1CONbits.DC1B0 = (a & 0x01);
}

static void AUDIO_tick(void)
{
    if (sample < sound_length) {
        AUDIO_update_sample((const unsigned char)sound[sample++]);
    } else {
        sound_done = 1;
        AUDIO_update_sample(0x80);
    }
}

//===----------------------------------------------------------------------===//
//  PUBLIC DECLARATIONS
//===----------------------------------------------------------------------===//

void AUDIO_ISR(void)
{
    if (INTCONbits.TMR0IF) {
        /* Interrupt is coming from Timer 0 */
        
        AUDIO_tick();

        TMR0L = (uint8_t)P_SAMPLE; /* Overflow after C_SAMPLE counts */

        INTCONbits.TMR0IF = 0; /* Reset interrupt flag */
    }
}

void AUDIO_playSound(int to_play)
{
    /* Set the sound buffer */
    switch (to_play) {
        case SOUND_SELECT:
            sound = do_central;
            sound_length = DO_CENTRAL_LENGTH;
            break;
        case SOUND_WON:
            sound = do_central;
            sound_length = DO_CENTRAL_LENGTH;
            break;
        case SOUND_READY:
            sound = ready;
            sound_length = READY_LENGTH >> 1;
            break;
        case SOUND_SET:
            sound = ready;
            sound_length = READY_LENGTH >> 1;
            break;
        case SOUND_GO: /* Intentional fall-through */
            sound = ready;
            sound_length = READY_LENGTH;
            break;
        default:
            sound_length = 0;
            sound = 0;
    }
    
    /* Reset sample count */
    sample = 0;
    sound_done = 0;
}


void AUDIO_tap(unsigned char pattern)
{
    switch (pattern) {
        case NONE: /* intention */
            return;
        case INDEX:
            sound = do_central;
            sound_length = DO_CENTRAL_LENGTH;
            break;
        case MIDDLE:
            sound = fa_central;
            sound_length = FA_CENTRAL_LENGTH;
            break;
        case RING:
            sound = sol_central;
            sound_length = SOL_CENTRAL_LENGTH;
            break;
        case PINKY:
            sound = do_second;
            sound_length = DO_SECOND_LENGTH;
            break;
        default:
            sound_length = 0;
            sound = 0;
    }
    
    /* Reset sample count */
    sample = 0;
}


void AUDIO_init(void)
{
    sound = 0;
    sample = 0;
    AUDIO_setupPWM();
    AUDIO_setupSampler();
}