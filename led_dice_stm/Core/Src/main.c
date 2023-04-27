/* USER CODE BEGIN Header */
/**
 **************************************************
 *
 * @file        main.c
 * @brief       STM32 code for LED Dice Soldering Kit from Soldered.
 *
 * @note        In order to successfully run this code, make sure to use STM32 cube programmer
 * 				And set Option Bytes -> User Configuration -> NRST_MODE 2
 *
 * @authors     Robert Soric for soldered.com
 ***************************************************/

/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "dice.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

/* USER CODE BEGIN PV */

volatile uint8_t flag = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
/* USER CODE BEGIN PFP */
void setNumber(uint8_t _n);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */
	// COMMENT THIS DELAY IN THE FINAL CODE (for debug purpose only)
	//HAL_Delay(8000);
	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_ADC1_Init();
	/* USER CODE BEGIN 2 */
	// Start measuring with ADC to get random values
	HAL_ADC_Start(&hadc1);

	// Show random value on startup

	HAL_Delay(1);
	uint8_t randomDiceRolledValue = getNewRandomValue(&hadc1);
	showNumber(randomDiceRolledValue);

	// Remember it
	uint8_t lastRandomDiceRolledValue = randomDiceRolledValue;
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */

	// Main loop
	while (1) {

		if (flag) {
			uint8_t rollCounter = 0;
			uint8_t delayTime = 30;
			int i = 0;

			// Add a random value of rolls
			do {
				rollCounter += getNewRandomValue(&hadc1);
				i++;
			} while (i < getNewRandomValue(&hadc1));

			// Each next display of rolls happens a bit later for dramatic effect
			// Calculate how much longer each roll will take
			uint8_t delayTimeIncrement = (200 / rollCounter);

			while (rollCounter) {
				if (flag) {
					flag = 0;

					// If still rolling, add new rolls but no more than 50
					rollCounter += getNewRandomValue(&hadc1);
					if (rollCounter > 50)
						rollCounter = 50;
				}

				// Make sure the number that is being displayed is not the same as the last one
				// To simulate real dice
				do {
					randomDiceRolledValue = getNewRandomValue(&hadc1);
				} while (randomDiceRolledValue == lastRandomDiceRolledValue);

				// Number is good, show the roll and remember it
				showNumber(randomDiceRolledValue);
				lastRandomDiceRolledValue = randomDiceRolledValue;
				rollCounter--;

				// Wait the expected length of time and increment it the correct amount
				HAL_Delay(delayTime);
				delayTime += delayTimeIncrement;
			}
		}
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV32;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
	RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief ADC1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_ADC1_Init(void) {

	/* USER CODE BEGIN ADC1_Init 0 */

	/* USER CODE END ADC1_Init 0 */

	ADC_ChannelConfTypeDef sConfig = { 0 };

	/* USER CODE BEGIN ADC1_Init 1 */
	GPIO_InitTypeDef gpioInit;
	__GPIOA_CLK_ENABLE()
	;
	gpioInit.Pin = RNG_Pin;
	gpioInit.Mode = GPIO_MODE_ANALOG;
	gpioInit.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(RNG_GPIO_Port, &gpioInit);
	/* USER CODE END ADC1_Init 1 */

	/** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
	 */
	hadc1.Instance = ADC1;
	hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
	hadc1.Init.Resolution = ADC_RESOLUTION_12B;
	hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
	hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	hadc1.Init.LowPowerAutoWait = DISABLE;
	hadc1.Init.LowPowerAutoPowerOff = DISABLE;
	hadc1.Init.ContinuousConvMode = ENABLE;
	hadc1.Init.NbrOfConversion = 1;
	hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	hadc1.Init.DMAContinuousRequests = DISABLE;
	hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
	hadc1.Init.SamplingTimeCommon1 = ADC_SAMPLETIME_1CYCLE_5;
	hadc1.Init.SamplingTimeCommon2 = ADC_SAMPLETIME_1CYCLE_5;
	hadc1.Init.OversamplingMode = DISABLE;
	hadc1.Init.TriggerFrequencyMode = ADC_TRIGGER_FREQ_HIGH;
	if (HAL_ADC_Init(&hadc1) != HAL_OK) {
		Error_Handler();
	}

	/** Configure Regular Channel
	 */
	sConfig.Channel = ADC_CHANNEL_14;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLINGTIME_COMMON_1;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN ADC1_Init 2 */

	/* USER CODE END ADC1_Init 2 */

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(LED_1_7_GPIO_Port, LED_1_7_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(LED_3_5_GPIO_Port, LED_3_5_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOA, LED_4_Pin | LED_2_6_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin : LED_1_7_Pin */
	GPIO_InitStruct.Pin = LED_1_7_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LED_1_7_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : LED_3_5_Pin */
	GPIO_InitStruct.Pin = LED_3_5_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LED_3_5_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pins : LED_4_Pin LED_2_6_Pin */
	GPIO_InitStruct.Pin = LED_4_Pin | LED_2_6_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pin : SWITCH_Pin */
	GPIO_InitStruct.Pin = SWITCH_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(SWITCH_GPIO_Port, &GPIO_InitStruct);

	/**/
	HAL_SYSCFG_SetPinBinding(
	HAL_BIND_SO8_PIN1_PB7 | HAL_BIND_SO8_PIN4_PF2 | HAL_BIND_SO8_PIN5_PA8);

	/* EXTI interrupt init*/
	HAL_NVIC_SetPriority(EXTI4_15_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);

}

/* USER CODE BEGIN 4 */

void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin) {
	flag = 1;
}

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
	}
	/* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
