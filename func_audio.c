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
     * 
     */
    
    T2CON = 0b00000000;     /* Timer 2 control register
                             *  bit   7: Unimplemented
                             *  bit 6-3: Output postscale select (0000 -> 1:1)
                             *  bit   2: Timer on/off (1 -> On)
                             *  bit 1-0: Clock prescale select (01 -> 1/4)
                             */
    
    PIR1bits.TMR2IF = 0;    /* Clear Timer 2 interrupt flag */
    PIE1bits.TMR2IE = 0;    /* Disable interrupts for Timer 2 */

    PR2 = C_PWM;            /* Overflow value for PWM CLK */
    
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
    
    TMR0L = P_SAMPLE;       /* Overflow after C_SAMPLE counts */
    
    INTCON = 0x60;          /* Interrupt Control Register
                             *  bit7 "0": Global interrupt Enable
                             *  bit6 "1": Peripheral Interrupt Enable
                             *  bit5 "1": Enables the TMR0 overflow interrupt
                             *  bit4 "0": Diables the INT0 external interrupt
                             *  bit3 "0": Disables the RB port change interrupt
                             *  bit2 "0": TMR0 Overflow Interrupt Flag bit
                             *  bit1 "0": INT0 External Interrupt Flag bit
                             *  bit0 "0": RB Port Change Interrupt Flag bit
                             */
    
    T0CONbits.TMR0ON = 1;   /* Enable Timer 0 */
    INTCONbits.GIE = 1;     /* Enable interrupt */
}

static void AUDIO_update_sample(const unsigned char a)
{
    CCPR1L = a >> 2;
    CCP1CONbits.DC1B1 = (a & 0x02) >> 1;
    CCP1CONbits.DC1B0 = (a & 0x01);
}

static void AUDIO_tick(void)
{
    if (sound && sample < sound_length)
        AUDIO_update_sample(sound[sample]);
}

//===----------------------------------------------------------------------===//
//  PUBLIC DECLARATIONS
//===----------------------------------------------------------------------===//

void AUDIO_ISR(void)
{
    if (INTCONbits.TMR0IF) {
        /* Interrupt is coming from Timer 0 */
        
        AUDIO_tick();
        
        TMR0L = P_SAMPLE; /* Overflow after C_SAMPLE counts */
        
        INTCONbits.TMR0IF = 0; /* Reset interrupt flag */
    }
}

void AUDIO_playSound(int to_play)
{
    /* Set the sound buffer */
    switch (to_play) {
        case SOUND_TAP:
            sound = tap;
            break;
        case SOUND_SELECT:
            sound = select;
            break;
        default:
            sound = 0;
    }
    
    /* Reset sample count */
    sample = 0;
}

void AUDIO_init(void)
{
    AUDIO_setupPWM();
    AUDIO_setupSampler();
}

//===----------------------------------------------------------------------===//
//===----------------------------------------------------------------------===//

