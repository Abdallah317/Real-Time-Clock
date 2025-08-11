/*
 * HELPER.c
 *
 *  Created on: Aug 10, 2025
 *      Author: PC
 */

#include "HELPER.h"
const char *days[] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

void Down_Counter_Fun(void)
{
    uint16_t Down_Counter = 0;
    function = 1;

    LCD_Clear();
    LCD_Printf("Enter seconds:");
    Down_Counter = getThreeDigitDecimalFromKeypad();
    _delay_ms(500);

    LCD_Clear();
    while (1)
    {
        if (exit_flag == 1)
        {
            exit_flag = 0;
            break;
        }

        LCD_GoToLine(0);
        LCD_Printf("%3d", Down_Counter);

        if (Down_Counter == 0)
        {
            LCD_Clear();
            LCD_Printf("TIME'S UP!");
            GPIO_PinWrite(PA_3, HIGH);
            while (GPIO_PinRead(PA_4) == HIGH);
            GPIO_PinWrite(PA_3, LOW);
            break;
        }

        _delay_ms(900);
        Down_Counter--;
    }
}

void Stop_Watch_Fun(void)
{
    function = 1;
    LCD_Clear();
    uint8_t Stop_Watch_secs = 0;
    uint8_t Stop_Watch_mins = 0;
    uint8_t is_paused = 1;

    while (1)
    {
        if (exit_flag == 1)
        {
            exit_flag = 0;
            break;
        }

        if (GPIO_PinRead(PA_2) == LOW)
        {
            _delay_ms(50);
            Stop_Watch_secs = 0;
            Stop_Watch_mins = 0;
            while (GPIO_PinRead(PA_2) == LOW);
            is_paused = 1;
            LCD_Clear();
        }

        if (GPIO_PinRead(PA_1) == LOW)
        {
            _delay_ms(50);
            is_paused = !is_paused;
            while (GPIO_PinRead(PA_1) == LOW);
        }

        if (!is_paused)
        {
            LCD_GoToLine(0);
            LCD_Printf("%2d:%2d", Stop_Watch_mins, Stop_Watch_secs);
            _delay_ms(900);
            Stop_Watch_secs++;

            if (Stop_Watch_secs == 60)
            {
                Stop_Watch_mins++;
                Stop_Watch_secs = 0;
            }
        }
    }
}

void View_Fun(rtc_t *rtc)
{
    RTC_GetDateTime(rtc);
    LCD_GoToLine(0);
    LCD_Printf("time:%2x:%2x:%2x  \nDate:%2x/%2x/%2x%s",
               (uint16_t)rtc->hour, (uint16_t)rtc->min, (uint16_t)rtc->sec,
               (uint16_t)rtc->date, (uint16_t)rtc->month, (uint16_t)rtc->year,days[rtc->weekDay-1]);
}

uint8_t getTwoDigitBCDFromKeypad()
{
    uint8_t digit1 = 0x00, digit2 = 0x00;

    while (1)
    {
        digit1 = KEYPAD_GetKey();
        if (digit1 >= '0' && digit1 <= '9')
        {
            LCD_DisplayChar(digit1); // Display the digit
            break;
        }
    }

    while (1)
    {
        digit2 = KEYPAD_GetKey();
        if (digit2 >= '0' && digit2 <= '9')
        {
            LCD_DisplayChar(digit2); // Display the digit
            break;
        }
    }

    return ((digit1 - '0') << 4) | (digit2 - '0'); // BCD conversion
}
uint16_t getThreeDigitDecimalFromKeypad()
{
    uint8_t digit1 = 0x00, digit2 = 0x00, digit3 = 0x00;

    while (1)
    {
        digit1 = KEYPAD_GetKey();
        if (digit1 >= '0' && digit1 <= '9')
        {
            LCD_DisplayChar(digit1);
            break;
        }
    }

    while (1)
    {
        digit2 = KEYPAD_GetKey();
        if (digit2 >= '0' && digit2 <= '9')
        {
            LCD_DisplayChar(digit2);
            break;
        }
    }

    while (1)
    {
        digit3 = KEYPAD_GetKey();
        if (digit3 >= '0' && digit3 <= '9')
        {
            LCD_DisplayChar(digit3);
            break;
        }
    }

    // Convert to decimal (not BCD)
    return (((digit1-'0')*100) + ((digit2-'0')*10) + (digit3-'0'));
}
