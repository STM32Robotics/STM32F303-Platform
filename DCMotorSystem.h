#ifndef DCMOTORSYSTEM_H_
#define DCMOTORSYSTEM_H_

#define DC_M1_BW        0x02U
#define DC_M1_FW        0x01U
#define DC_M2_BW        0x01U
#define DC_M2_FW        0x02U
#define DCMOTOR1_MASK   (DC_M1_BW | DC_M1_FW)
#define DCMOTOR2_MASK   (DC_M2_BW | DC_M2_FW)
#define MAXSPEED        100U

#define TIM1_CNT_CLOCK_MOTOR 2000000
#define PRESCALE_MOTOR (uint32_t)((SystemCoreClock / TIM1_CNT_CLOCK_MOTOR) - 1)
#define PERIOD_MOTOR 99

void DCMotorInit();

typedef struct _dcmotor_attr {
    unsigned char speed;
    unsigned char direction;   
} dcmotor_attr;


typedef enum _motor_sel{
    M_ONE,
    M_TWO  
}motor_sel;


typedef enum _motor_dir{
    FORWARD,
    BACKWARD,
}motor_dir;

void DCMotor(motor_sel which_motor, unsigned char const speed, motor_dir direction);

#endif