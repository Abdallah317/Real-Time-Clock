/*
 * HELPER.h
 *
 *  Created on: Aug 10, 2025
 *      Author: PC
 */

#ifndef APP_HELPER_H_
#define APP_HELPER_H_

#include "../HAL/rtc/rtc.h"
#include "../HAL/lcd/lcd.h"
#include "../HAL/keypad/keypad.h"
#include "../MCAL/gpio/gpio.h"
#include "../LIB/stdutils.h"
#include <avr/interrupt.h>
#include <util/delay.h>
// Global variables to exit from one mode to the main menu

extern uint8_t exit_flag;
extern uint8_t function;
uint16_t getThreeDigitDecimalFromKeypad();
uint8_t getTwoDigitBCDFromKeypad();
uint8_t Alarm(rtc_t *rtc, uint8_t *Alarm_hr, uint8_t *Alarm_min);
void Stop_Watch_Fun(void);
void Down_Counter_Fun(void);
void View_Fun(rtc_t *rtc);
#endif /* APP_HELPER_H_ */
