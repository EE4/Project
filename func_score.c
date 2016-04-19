//===----------------------------------------------------------------------===//
//  
//  EE4 Project - Finger Tapping Game
//
//  Author: Jelle De Vleeschouwer
//  Updated: 16/04/2016
//
//===----------------------------------------------------------------------===//

#include "func_score.h"

#define MIN_DUTY   (13)
#define MAX_DUTY   (42)
#define MAX_SCORE  ((MAX_DUTY - MIN_DUTY - 1) / 2)
#define MIN_SCORE  (-((MAX_DUTY - MIN_DUTY - 1) / 2))
#define MID_DUTY   (MIN_DUTY + ((MAX_DUTY - MIN_DUTY - 1) / 2))

static short score = 0;

static void DUTY_update(void)
{
    short duty = score + MID_DUTY;
    
    /* Condition */
    duty = ((duty > MAX_DUTY) ? MAX_DUTY : (duty < MIN_DUTY ? MIN_DUTY : duty));
    
    /* Update */
    PWM_duty[SERVO_CHANNEL] = duty;
}

static void SCORE_condition(void)
{
    if (score > MAX_SCORE) 
        score = MAX_SCORE;
    if (score < MIN_SCORE)
        score = MIN_SCORE;
}

/* Negative score indicated a win of player 1, a positive score indicates
 * a win of player 2 */
int SCORE_getScore(void)
{
    return score;
}

/* Set the absolute score immediatelly */
void SCORE_setScore(int absolute)
{
    score = absolute;
    SCORE_condition();
    DUTY_update();
}

/* Update the absolute score with a relative score */
void SCORE_updateScore(int relative)
{
    score = score + relative;
    SCORE_condition();
    DUTY_update();
}

//EOF-----------------------------------------------------------------
