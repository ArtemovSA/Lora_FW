
#ifndef __DEV_CTRL_H__
#define __DEV_CTRL_H__

#include "stdint.h"
#include "stm32f1xx_hal.h"

//Default settings
#define DC_MAGICKEY_SETTINGS            0x01
#define DC_MAGICKEY_PARAMS              0x02

#define DC_DEFAULT_SET_MODBUSADDR       0x06

//Status led
#define DC_INFINITE 0xFFFF
#define LED_ON HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET)
#define LED_OFF HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET)

//Flash adresses
#define FADR_SETTINGS           0x0000
#define FADR_PARAMETRS          0x1000
#define FADR_SCRIPT_START       0x2000

typedef struct{
  uint8_t modbusAddr;
  
}DC_settings_t;

typedef struct{
  uint8_t PWM1_OUT;
  uint8_t PWM2_OUT;
}DC_params_t;



extern DC_settings_t DC_settings;
extern DC_params_t DC_params;

void DC_init(); //Device init
void DC_debugOut(char *str, ...); //Out debug data
void DC_ledBlink(uint16_t periodic, uint16_t count); //Led blink


#endif