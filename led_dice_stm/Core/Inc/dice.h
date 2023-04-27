/*
 * game.h
 *
 *  Created on: Apr 26, 2023
 *      Author: Robert for Soldered.com
 */

#ifndef INC_DICE_H_
#define INC_DICE_H_

#include "stm32c0xx_hal.h"
#include "stdint.h"
#include "main.h"

// Function prototypes

uint8_t getNewRandomValue(ADC_HandleTypeDef *hadc);
void showNumber(uint8_t _n);

#endif /* INC_GAME_H_ */
