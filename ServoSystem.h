#include "stm32f30x.h"

#define SERVO_DUTY_100PCNT  10000U
#define SERVO_CNT_GAIN      5U
#define SERVO_CNT_OFFSET    300U
#define SRVO_INIT_SUCCESS   0
#define SRVO_TIM_INIT_ERR   -1
#define SRVO_TIM_CFG_ERR    -2
#define SRVO_TIM_STRT_ERR   -3

int16_t ServoInit();
void Servo(uint32_t degree);
void PWMPulseFinish();