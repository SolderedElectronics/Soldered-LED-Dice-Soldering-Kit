/* USER CODE BEGIN Header */
/**
 **************************************************
 *
 * @file        main.h
 * @brief       Header code for LED Dice Soldering Kit from Soldered.
 *
 * @note        In order to successfully run this code, make sure to use STM32 cube programmer
 * 				And set Option Bytes -> User Configuration -> NRST_MODE 2
 *
 * @authors     Robert Soric for soldered.com
 ***************************************************/
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32c0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED_1_7_Pin GPIO_PIN_7
#define LED_1_7_GPIO_Port GPIOB
#define LED_3_5_Pin GPIO_PIN_2
#define LED_3_5_GPIO_Port GPIOF
#define LED_4_Pin GPIO_PIN_8
#define LED_4_GPIO_Port GPIOA
#define LED_2_6_Pin GPIO_PIN_12
#define LED_2_6_GPIO_Port GPIOA
#define SWITCH_Pin GPIO_PIN_13
#define SWITCH_GPIO_Port GPIOA
#define SWITCH_EXTI_IRQn EXTI4_15_IRQn
#define RNG_Pin GPIO_PIN_14
#define RNG_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
