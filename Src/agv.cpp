#include "agv.h"
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
//warning: ugly hardware code hastily put together, don't stare at it for too long

//how many radians does the robot turn in one second (max voltage on both wheels, turning
//to opposite sides).
#define RADIANS_PER_SECOND 2.45
//how many meters does the robot moves in one second (max voltage on both wheels)
#define METERS_PER_SECOND 0.5
//the higher this number is the more amortized the sensor readings are
#define SENSOR_MEAN_NUM 10
//threshold for the left if-sensor (compared to the 12-bit adc output)
#define LEFT_THRESHOLD 900
//threshold for the right if-sensor (compared to the 12-bit adc output)
#define RIGHT_THRESHOLD 600
#define DEFAULT_PWM 800
#define LEFT_MOTOR_CHANNEL TIM_CHANNEL_1
#define RIGHT_MOTOR_CHANNEL TIM_CHANNEL_2
#define ECHO_CHANNEL_1 TIM_CHANNEL_1
#define ECHO_CHANNEL_2 TIM_CHANNEL_2
#define TRIGGER_CHANNEL TIM_CHANNEL_1

void TurnAngle(float radians);
AVG_StatusTypedef TurnAngleGrab(float radians, float searchAngle);
AVG_StatusTypedef TurnLines(int lines);
AVG_StatusTypedef FollowLine();
void HardForward(float distance);
void SystemClock_Config(void);
void BusyWait(uint32_t ms);
void TurnRight(uint32_t duty_cycle);
void TurnLeft(uint32_t duty_cycle);
void Stop();
void LeftForwards(uint32_t duty_cycle);
void LeftBackwards(uint32_t duty_cycle);
void LeftStop();
void RightForwards(uint32_t duty_cycle);
void RightBackwards(uint32_t duty_cycle);
void RightStop();

uint16_t ADC_Data[2] = {0};
uint32_t echo_period;

void InitAVG(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART2_UART_Init();
  MX_ADC1_Init();
  MX_TIM3_Init();
  MX_TIM1_Init();
  MX_TIM11_Init();
  MX_TIM4_Init();

  HAL_ADC_Start_DMA(&hadc1, (uint32_t *)ADC_Data, 2);

  HAL_TIM_PWM_Start(&htim3, LEFT_MOTOR_CHANNEL);
  HAL_TIM_PWM_Start(&htim3, RIGHT_MOTOR_CHANNEL);
  HAL_TIM_PWM_Start(&htim4, TRIGGER_CHANNEL);
  HAL_TIM_PWM_Start_DMA(&htim11, ECHO_CHANNEL_2, &echo_period, 1);
  HAL_TIM_PWM_Start(&htim11, ECHO_CHANNEL_1);
  Stop();
  HAL_TIM_Base_Start(&htim3);

  HAL_GPIO_WritePin(LINE_LEFT_DETECTED_GPIO_Port, LINE_LEFT_DETECTED_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LINE_RIGHT_DETECTED_GPIO_Port, LINE_RIGHT_DETECTED_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(TURNING_GPIO_Port, TURNING_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(FOLLOWING_LINE_GPIO_Port, FOLLOWING_LINE_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(ERROR_GPIO_Port, ERROR_Pin, GPIO_PIN_RESET);
}

void TurnAngle(float radians)
{
  HAL_GPIO_WritePin(TURNING_GPIO_Port, TURNING_Pin, GPIO_PIN_SET);
  if (radians > 0)
    TurnLeft(DEFAULT_PWM);
  else
    TurnRight(DEFAULT_PWM);
  BusyWait(1000 * radians / RADIANS_PER_SECOND);
  Stop();
  HAL_GPIO_WritePin(TURNING_GPIO_Port, TURNING_Pin, GPIO_PIN_RESET);
}

void TurnLeftFor(float time)
{
  HAL_GPIO_WritePin(TURNING_GPIO_Port, TURNING_Pin, GPIO_PIN_SET);
  TurnLeft(DEFAULT_PWM);
  BusyWait(time);
  Stop();
  HAL_GPIO_WritePin(TURNING_GPIO_Port, TURNING_Pin, GPIO_PIN_RESET);
}
void TurnRightFor(float time)
{
  HAL_GPIO_WritePin(TURNING_GPIO_Port, TURNING_Pin, GPIO_PIN_SET);
  TurnRight(DEFAULT_PWM);
  BusyWait(time);
  Stop();
  HAL_GPIO_WritePin(TURNING_GPIO_Port, TURNING_Pin, GPIO_PIN_RESET);
}

AVG_StatusTypedef FollowLine(float allignDistance)
{
  HAL_GPIO_WritePin(FOLLOWING_LINE_GPIO_Port, FOLLOWING_LINE_Pin, GPIO_PIN_SET);
  uint32_t leftSensor = ADC_Data[0];
  uint32_t rightSensor = ADC_Data[1];
  for (int i = 0; i < SENSOR_MEAN_NUM; i++)
  {
    leftSensor = ((SENSOR_MEAN_NUM - 1) * leftSensor + ADC_Data[0]) / SENSOR_MEAN_NUM;
    rightSensor = ((SENSOR_MEAN_NUM - 1) * rightSensor + ADC_Data[1]) / SENSOR_MEAN_NUM;
  }
  while (leftSensor < LEFT_THRESHOLD || rightSensor < RIGHT_THRESHOLD)
  {
    if (leftSensor >= LEFT_THRESHOLD)
    {
      HAL_GPIO_WritePin(LINE_LEFT_DETECTED_GPIO_Port, LINE_LEFT_DETECTED_Pin, GPIO_PIN_SET);
      LeftStop();
    }
    else
    {
      HAL_GPIO_WritePin(LINE_LEFT_DETECTED_GPIO_Port, LINE_LEFT_DETECTED_Pin, GPIO_PIN_RESET);
      LeftForwards(DEFAULT_PWM);
    }
    if (rightSensor >= RIGHT_THRESHOLD)
    {
      HAL_GPIO_WritePin(LINE_RIGHT_DETECTED_GPIO_Port, LINE_RIGHT_DETECTED_Pin, GPIO_PIN_SET);
      RightStop();
    }
    else
    {
      HAL_GPIO_WritePin(LINE_RIGHT_DETECTED_GPIO_Port, LINE_RIGHT_DETECTED_Pin, GPIO_PIN_RESET);
      RightForwards(DEFAULT_PWM);
    }
    BusyWait(5);
    leftSensor = ((SENSOR_MEAN_NUM - 1) * leftSensor + ADC_Data[0]) / SENSOR_MEAN_NUM;
    rightSensor = ((SENSOR_MEAN_NUM - 1) * rightSensor + ADC_Data[1]) / SENSOR_MEAN_NUM;
  }
  HAL_GPIO_WritePin(LINE_LEFT_DETECTED_GPIO_Port, LINE_LEFT_DETECTED_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LINE_RIGHT_DETECTED_GPIO_Port, LINE_RIGHT_DETECTED_Pin, GPIO_PIN_RESET);
  RightForwards(DEFAULT_PWM);
  LeftForwards(DEFAULT_PWM);
  BusyWait(1000 * allignDistance / METERS_PER_SECOND);
  Stop();
  HAL_GPIO_WritePin(FOLLOWING_LINE_GPIO_Port, FOLLOWING_LINE_Pin, GPIO_PIN_RESET);
  return AGV_OK;
}

void SignalError()
{
  HAL_GPIO_WritePin(ERROR_GPIO_Port, ERROR_Pin, GPIO_PIN_SET);
  while (1)
    ;
}

void TurnRight(uint32_t duty_cycle)
{
  LeftForwards(duty_cycle);
  RightBackwards(duty_cycle);
}

void TurnLeft(uint32_t duty_cycle)
{
  RightForwards(duty_cycle);
  LeftBackwards(duty_cycle);
}

void Stop()
{
  RightStop();
  LeftStop();
}

void LeftForwards(uint32_t duty_cycle)
{
  __HAL_TIM_SET_COMPARE(&htim3, LEFT_MOTOR_CHANNEL, duty_cycle);
  HAL_GPIO_WritePin(H_BRIDGE_IN_3_GPIO_Port, H_BRIDGE_IN_3_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(H_BRIDGE_IN_4_GPIO_Port, H_BRIDGE_IN_4_Pin, GPIO_PIN_SET);
}

void LeftStop()
{
  __HAL_TIM_SET_COMPARE(&htim3, LEFT_MOTOR_CHANNEL, 1000);
  HAL_GPIO_WritePin(H_BRIDGE_IN_3_GPIO_Port, H_BRIDGE_IN_3_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(H_BRIDGE_IN_4_GPIO_Port, H_BRIDGE_IN_4_Pin, GPIO_PIN_RESET);
}

void LeftBackwards(uint32_t duty_cycle)
{
  __HAL_TIM_SET_COMPARE(&htim3, LEFT_MOTOR_CHANNEL, duty_cycle);
  HAL_GPIO_WritePin(H_BRIDGE_IN_3_GPIO_Port, H_BRIDGE_IN_3_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(H_BRIDGE_IN_4_GPIO_Port, H_BRIDGE_IN_4_Pin, GPIO_PIN_RESET);
}

void RightForwards(uint32_t duty_cycle)
{
  __HAL_TIM_SET_COMPARE(&htim3, RIGHT_MOTOR_CHANNEL, duty_cycle);
  HAL_GPIO_WritePin(H_BRIDGE_IN_1_GPIO_Port, H_BRIDGE_IN_1_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(H_BRIDGE_IN_2_GPIO_Port, H_BRIDGE_IN_2_Pin, GPIO_PIN_SET);
}

void RightStop()
{
  __HAL_TIM_SET_COMPARE(&htim3, RIGHT_MOTOR_CHANNEL, 1000);
  HAL_GPIO_WritePin(H_BRIDGE_IN_1_GPIO_Port, H_BRIDGE_IN_1_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(H_BRIDGE_IN_2_GPIO_Port, H_BRIDGE_IN_2_Pin, GPIO_PIN_RESET);
}

void RightBackwards(uint32_t duty_cycle)
{
  __HAL_TIM_SET_COMPARE(&htim3, RIGHT_MOTOR_CHANNEL, duty_cycle);
  HAL_GPIO_WritePin(H_BRIDGE_IN_1_GPIO_Port, H_BRIDGE_IN_1_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(H_BRIDGE_IN_2_GPIO_Port, H_BRIDGE_IN_2_Pin, GPIO_PIN_RESET);
}

uint32_t waitMs = 0;

void BusyWait(uint32_t ms)
{
  waitMs = ms;
  HAL_TIM_Base_Start_IT(&htim11);
  while (waitMs > 0)
    ;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM11)
  {
    waitMs--;
    if (waitMs > 0)
    {
      HAL_TIM_Base_Start_IT(&htim11);
    }
  }
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */