/**
 **************************************************
 *
 * @file        dice.c
 * @brief       Dice specific STM32 code for LED Dice Soldering Kit from Soldered.
 *
 * @note        In order to successfully run this code, make sure to use STM32 cube programmer
 * 				And set Option Bytes -> User Configuration -> NRST_MODE 2
 *
 * @authors     Robert Soric for soldered.com
 ***************************************************/
#include "dice.h"

/**
 * @brief			Calculate a random number from 1 until 6
 *
 * @params			ADC_HandleTypeDef *hadc: Pointer to ADC object to read noise from ADC to generate a random number
 *
 * @returns			A uint8_t generated randomly
 */
uint8_t getNewRandomValue(ADC_HandleTypeDef *hadc) {
	uint16_t seed;
	uint16_t temp = 1;
	int i;

	HAL_Delay(1);
	// Make 25 readings of noise on ADC pin and shuffle their bits
	for (i = 0; i < 25; i++) {
		// Randomly delay last couple of measurements by 3 ms max
		if(i > 22) HAL_Delay(temp & 0b00000011);
		seed ^= HAL_ADC_GetValue(hadc);
		//Shuffle the seed
		seed = 2053 * seed + 12491;
		//XOR two bytes
		temp = seed ^ (seed >> 2);
		//XOR two nibbles
		temp ^= (temp >> 2);
	}

	// Return remainder of division by 6, add 1 so 0 isn't included and 6 is
	return (uint8_t) ((temp % 6) + 1 );
}

/**
 * @brief			Show a die number on the LEDs
 *
 * @params			uint8_t _n: number to show
 *
 * @returns			None
 */
void showNumber(uint8_t _n) {

	// Turn all LEDs off
	HAL_GPIO_WritePin(LED_1_7_GPIO_Port, LED_1_7_Pin, RESET);
	HAL_GPIO_WritePin(LED_2_6_GPIO_Port, LED_2_6_Pin, RESET);
	HAL_GPIO_WritePin(LED_3_5_GPIO_Port, LED_3_5_Pin, RESET);
	HAL_GPIO_WritePin(LED_4_GPIO_Port, LED_4_Pin, RESET);

	// Turn on the according combination of LEDs
	switch (_n) {
	case 0:
		break;
	case 1:
		HAL_GPIO_WritePin(LED_4_GPIO_Port, LED_4_Pin, SET);
		break;
	case 2:
		HAL_GPIO_WritePin(LED_1_7_GPIO_Port, LED_1_7_Pin, SET);
		break;
	case 3:
		HAL_GPIO_WritePin(LED_3_5_GPIO_Port, LED_3_5_Pin, SET);
		HAL_GPIO_WritePin(LED_4_GPIO_Port, LED_4_Pin, SET);
		break;
	case 4:
		HAL_GPIO_WritePin(LED_1_7_GPIO_Port, LED_1_7_Pin, SET);
		HAL_GPIO_WritePin(LED_3_5_GPIO_Port, LED_3_5_Pin, SET);
		break;
	case 5:
		HAL_GPIO_WritePin(LED_1_7_GPIO_Port, LED_1_7_Pin, SET);
		HAL_GPIO_WritePin(LED_3_5_GPIO_Port, LED_3_5_Pin, SET);
		HAL_GPIO_WritePin(LED_4_GPIO_Port, LED_4_Pin, SET);
		break;
	case 6:
		HAL_GPIO_WritePin(LED_1_7_GPIO_Port, LED_1_7_Pin, SET);
		HAL_GPIO_WritePin(LED_3_5_GPIO_Port, LED_3_5_Pin, SET);
		HAL_GPIO_WritePin(LED_2_6_GPIO_Port, LED_2_6_Pin, SET);
		break;
	default:
		break;
	}
}
