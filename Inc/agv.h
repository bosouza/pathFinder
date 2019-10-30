#ifndef AGV_H_DEFINED
#define AGV_H_DEFINED

#ifdef __cplusplus
extern "C"
{
#endif

#include "stm32f4xx_hal.h"

    void Error_Handler(void);

#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define H_BRIDGE_IN_2_Pin GPIO_PIN_1
#define H_BRIDGE_IN_2_GPIO_Port GPIOB
#define H_BRIDGE_IN_1_Pin GPIO_PIN_2
#define H_BRIDGE_IN_1_GPIO_Port GPIOB
#define H_BRIDGE_IN_4_Pin GPIO_PIN_14
#define H_BRIDGE_IN_4_GPIO_Port GPIOB
#define H_BRIDGE_IN_3_Pin GPIO_PIN_15
#define H_BRIDGE_IN_3_GPIO_Port GPIOB
#define LINE_LEFT_DETECTED_Pin GPIO_PIN_6
#define LINE_LEFT_DETECTED_GPIO_Port GPIOC
#define LINE_RIGHT_DETECTED_Pin GPIO_PIN_8
#define LINE_RIGHT_DETECTED_GPIO_Port GPIOC
#define FOLLOWING_LINE_Pin GPIO_PIN_9
#define FOLLOWING_LINE_GPIO_Port GPIOC
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define TURNING_Pin GPIO_PIN_8
#define TURNING_GPIO_Port GPIOB
#define ERROR_Pin GPIO_PIN_9
#define ERROR_GPIO_Port GPIOB

#ifdef __cplusplus
}
#endif

typedef enum
{
    AGV_OK = 0,
    AGV_ERROR,
} AVG_StatusTypedef;

//initAVG initializes all peripherals and low level stuff
void InitAVG(void);
//turnAngle will simply turn the avg the specified angle (in radians). A positive angle means a left
//turn (counter-clockwise, looking from above), while a negative angle means a right turn (clockwise,
//looking from above).
void TurnAngle(float radians);
//turnAngleGrap will turn the avg and look for a guide line inside the interval specified by
//(´radians´ - ´searchAngle´, ´radians´ + ´searchAngle´). Returns false if no guide line was found
//inside the interval, in which case the avg will make sure to turn exactly radians before returning
AVG_StatusTypedef TurnAngleGrab(float radians, float searchAngle);
//turn lines will spin while counting how many lines crossed the sensors, stoping once ´lines´ is
//reached. Returns false once a full rotation is completed but couldn't find sufficient guide lines,
//in which case the avg makes sure to stop at the initial angle
AVG_StatusTypedef TurnLines(int lines);
//followLine will attempt to follow the line that's currently between the sensors, returning true once
//a crossing has been reached. Return false means it was not possible to follow the path to its end,
//meaning the path is blocked. By default once a crossing is reached the agv will attempt to position
//itself right above the crossing by advancing the distance specified in `alignDistance`
AVG_StatusTypedef FollowLine(float alignDistance);
//hardForward will attempt to advance the specified distance. If the path is block it'll wait until
//it's unblocked to be able to clear the specified distance
void HardForward(float distance);
void SignalError();

void TurnLeft(uint32_t);
void TurnRight(uint32_t);

void LeftForwards(uint32_t);
void LeftBackwards(uint32_t);
void LeftStop();

void RightForwards(uint32_t);
void RightBackwards(uint32_t);
void RightStop();

#endif