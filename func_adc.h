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

#ifndef _FUNC_ADC_
#define _FUNC_ADC_

/** I N C L U D E S *************************************************/
#include "config.h"

/** D E F I N E S ***************************************************/
#define ADC_CHANNELS 3

/** P U B L I C   V A R I A B L E S *********************************/
// when a variable is declared 'extern' it also has to be declared in
// the corresponding .c file without the 'extern' keyword
extern int ADC_value[ADC_CHANNELS];

/** P U B L I C   P R O T O T Y P E S *******************************/
void ADC_init(void);
void ADC_ISR(void);

#endif
//EOF----------------------------------------------------------------
