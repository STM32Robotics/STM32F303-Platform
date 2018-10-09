#ifndef STEPPER_H_
#define STEPPER_H_

#define HALF_STEP     0x01U
#define FULL_STEP     0x02U
#define DIR_LEFT      -1
#define DIR_RIGHT     0
#define LMT_SW_RIGHT  0x400
#define LMT_SW_LEFT   0x004
#define LMT_SW_MASK  (LMT_SW_LEFT | LMT_SW_RIGHT)
#define STEPPER_MASK  0x3C0
#define STEP_MAX      390U

void StepperInit();
void Stepper(unsigned dir, unsigned char step_size, unsigned int step_count);
void StepperPosInit();
void Timer4Handler();
#endif