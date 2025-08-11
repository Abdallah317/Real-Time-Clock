/*#include "HELPER.h"
 // Global variables to exit from one mode to the main menu
uint8_t exit_flag = 0,
  function = 0;

ISR(INT2_vect) {
  if (function == 1) {
    exit_flag = 1;
  }
}

int main() {
  // Reset stopwatch
  GPIO_PinDirection(PA_2, INPUT);
  GPIO_PinWrite(PA_2, HIGH); // Pull-up

  // Pause stopwatch
  GPIO_PinDirection(PA_1, INPUT);
  GPIO_PinWrite(PA_1, HIGH); // Pull-up

  // Exit button configuration on INT2 >> PB_2
  GPIO_PinDirection(PB_2, INPUT);
  GPIO_PinWrite(PB_2, HIGH); // Pull-up
  MCUCSR &= ~(1 << ISC2); // Falling edge on INT2
  GICR |= (1 << INT2); // Enable INT2 interrupt
  sei(); // Enable global interrupt

  rtc_t rtc;
  uint8_t Alarm_hrs = 0;
  uint8_t Alarm_mins = 0;
  uint8_t i = 0;
  // Connect RS->PB0, RW->PB1, EN->PB3 and data bus to PORTB.4 to PORTB.7
  LCD_SetUp(PB_0, PB_1, PB_3, P_NC, P_NC, P_NC, P_NC, PB_4, PB_5, PB_6, PB_7);
  LCD_Init(2, 16);

  // Connect SCL->PC0, SDA->PC1
  RTC_Init();

  // Keypad connection
  KEYPAD_Init(PD_0, PD_1, PD_2, PD_3, PD_4, PD_5, PD_6, PD_7);

  // Buzzer output initialization (OFF by default)
  GPIO_PinDirection(PA_3, OUTPUT);
  GPIO_PinWrite(PA_3, LOW);

  // Buzzer control button
  GPIO_PinDirection(PA_4, INPUT);
  GPIO_PinWrite(PA_4, HIGH); // Pull-up

  while (1) {
    function = 0;
    LCD_Clear();

    LCD_Printf("A-Set C-View\nD-Stop E-Count");
    switch (KEYPAD_GetKey()) {
    case 'A':
      do {
        LCD_Clear();
        LCD_Printf("Set Hour:");
        rtc.hour = getTwoDigitBCDFromKeypad();
        _delay_ms(500);
      } while (rtc.hour > 0x23);

      do {
        LCD_Clear();
        LCD_Printf("Set Minute:");
        rtc.min = getTwoDigitBCDFromKeypad();
        _delay_ms(500);
      } while (rtc.min > 0x59);

      do {
        LCD_Clear();
        LCD_Printf("Set Second:");
        rtc.sec = getTwoDigitBCDFromKeypad();
        _delay_ms(500);
      } while (rtc.sec > 0x59);

      do {
        LCD_Clear();
        LCD_Printf("Set Date:");
        rtc.date = getTwoDigitBCDFromKeypad();
        _delay_ms(500);
      } while (rtc.date > 0x31);

      do {
        LCD_Clear();
        LCD_Printf("Set Month:");
        rtc.month = getTwoDigitBCDFromKeypad();
        _delay_ms(500);
      } while (rtc.month > 0x12);

      do {
        LCD_Clear();
        LCD_Printf("Set Year:");
        rtc.year = getTwoDigitBCDFromKeypad();
        _delay_ms(500);
      } while (rtc.year > 0x99);

      do {
        LCD_Clear();
        LCD_Printf("WeekDay[1-7]:");
        rtc.weekDay = KEYPAD_GetKey() - '0'; // Single digit
        LCD_DisplayChar(rtc.weekDay + '0');
        _delay_ms(500);
      } while (rtc.weekDay > 7);

      function = 1;
      RTC_SetDateTime( & rtc);
      View_Fun( & rtc);
      _delay_ms(1000);

      LCD_Clear();
      LCD_Printf("Alarm ? B");

      if (KEYPAD_GetKey() == 'B') {
        do {
          LCD_Clear();
          LCD_Printf("Alarm Hour:");
          Alarm_hrs = getTwoDigitBCDFromKeypad();
          _delay_ms(500);
        } while (Alarm_hrs > 0x23);

        do {
          LCD_Clear();
          LCD_Printf("Alarm Min:");
          Alarm_mins = getTwoDigitBCDFromKeypad();
          _delay_ms(500);
        } while (Alarm_mins > 0x59);

        LCD_Clear();
        while (!exit_flag) {
          View_Fun( & rtc);
          if (Alarm_hrs == rtc.hour && Alarm_mins == rtc.min) {
            //turn on buzzer
            GPIO_PinWrite(PA_3, HIGH);

            while (1) {
              LCD_Clear();
              LCD_Printf("Nap? B");
              if (KEYPAD_GetKey() == 'B') {
                GPIO_PinWrite(PA_3, LOW);
                LCD_Clear();
                LCD_Printf("Napp..");
                for (i = 0; i < 20; i++) {
                  _delay_ms(1000);
                }
                GPIO_PinWrite(PA_3, HIGH);
              } else {
                break;
              }

            }
            LCD_Clear();
            LCD_Printf("Turn OFF Buzzer");
            while (GPIO_PinRead(PA_4));
            GPIO_PinWrite(PA_3, LOW);
          }

        }
      } else {
        while (!exit_flag) {
          View_Fun( & rtc);
        }
      }
      break;

    case 'C':
      View_Fun( & rtc);
      _delay_ms(500);
      LCD_Clear();
      LCD_Printf("Alarm? B");

      function = 1;
      if (KEYPAD_GetKey() == 'B') {
        do {
          LCD_Clear();
          LCD_Printf("Alarm Hour:");
          Alarm_hrs = getTwoDigitBCDFromKeypad();
          _delay_ms(500);
        } while (Alarm_hrs > 0x23);

        do {
          LCD_Clear();
          LCD_Printf("Alarm Min:");
          Alarm_mins = getTwoDigitBCDFromKeypad();
          _delay_ms(500);
        } while (Alarm_mins > 0x59);

        LCD_Clear();
        while (!exit_flag) {
          View_Fun( & rtc);
          if (Alarm_hrs == rtc.hour && Alarm_mins == rtc.min) {
            //turn on buzzer
            GPIO_PinWrite(PA_3, HIGH);

            while (1) {
              LCD_Clear();
              LCD_Printf("Nap? B");
              if (KEYPAD_GetKey() == 'B') {
                GPIO_PinWrite(PA_3, LOW);
                LCD_Clear();
                LCD_Printf("Napp..");
                for (i = 0; i < 20; i++) {
                  _delay_ms(1000);
                }
                GPIO_PinWrite(PA_3, HIGH);
              } else {
                break;
              }

            }
            LCD_Clear();
            LCD_Printf("Turn OFF Buzzer");
            while (GPIO_PinRead(PA_4));
            GPIO_PinWrite(PA_3, LOW);
          }
        }
      } else {
        function = 1;
        while (!exit_flag) {
          View_Fun( & rtc);
        }
      }
      break;

    case 'D':
      Stop_Watch_Fun();
      break;

    case 'E':
      Down_Counter_Fun();
      break;

    default:
      LCD_Clear();
      LCD_Printf("Invalid Choice!");
      _delay_ms(200);
      break;
    }
  }

  return 0;
}*/

#include "HELPER.h"

// Global variables to exit from one mode to the main menu
uint8_t exit_flag = 0, function = 0;

ISR(INT2_vect)
{
    if (function == 1)
    {
        exit_flag = 1;
    }
}

int main()
{
    // Reset stopwatch
    GPIO_PinDirection(PA_2, INPUT);
    GPIO_PinWrite(PA_2, HIGH); // Pull-up

    // Pause stopwatch
    GPIO_PinDirection(PA_1, INPUT);
    GPIO_PinWrite(PA_1, HIGH); // Pull-up

    // Exit button configuration on INT2 >> PB_2
    GPIO_PinDirection(PB_2, INPUT);
    GPIO_PinWrite(PB_2, HIGH); // Pull-up
    MCUCSR &= ~(1 << ISC2);    // Falling edge on INT2
    GICR |= (1 << INT2);       // Enable INT2 interrupt
    sei();                     // Enable global interrupt

    rtc_t rtc;
    uint8_t Alarm_hrs = 0;
    uint8_t Alarm_mins = 0;
    uint8_t i = 0;

    // Connect RS->PB0, RW->PB1, EN->PB3 and data bus to PORTB.4 to PORTB.7
    LCD_SetUp(PB_0, PB_1, PB_3, P_NC, P_NC, P_NC, P_NC, PB_4, PB_5, PB_6, PB_7);
    LCD_Init(2, 16);

    // Connect SCL->PC0, SDA->PC1
    RTC_Init();

    // Keypad connection
    KEYPAD_Init(PD_0, PD_1, PD_2, PD_3, PD_4, PD_5, PD_6, PD_7);

    // Buzzer output initialization (OFF by default)
    GPIO_PinDirection(PA_3, OUTPUT);
    GPIO_PinWrite(PA_3, LOW);

    // Buzzer control button
    GPIO_PinDirection(PA_4, INPUT);
    GPIO_PinWrite(PA_4, HIGH); // Pull-up

    while (1)
    {
        function = 0;
        LCD_Clear();
        LCD_Printf("A-Set C-View\nD-Stop E-Count");

        switch (KEYPAD_GetKey())
        {
            case 'A':
                do
                {
                    LCD_Clear();
                    LCD_Printf("Set Hour:");
                    rtc.hour = getTwoDigitBCDFromKeypad();
                    _delay_ms(500);
                } while (rtc.hour > 0x23);

                do
                {
                    LCD_Clear();
                    LCD_Printf("Set Minute:");
                    rtc.min = getTwoDigitBCDFromKeypad();
                    _delay_ms(500);
                } while (rtc.min > 0x59);

                do
                {
                    LCD_Clear();
                    LCD_Printf("Set Second:");
                    rtc.sec = getTwoDigitBCDFromKeypad();
                    _delay_ms(500);
                } while (rtc.sec > 0x59);

                do
                {
                    LCD_Clear();
                    LCD_Printf("Set Date:");
                    rtc.date = getTwoDigitBCDFromKeypad();
                    _delay_ms(500);
                } while (rtc.date > 0x31);

                do
                {
                    LCD_Clear();
                    LCD_Printf("Set Month:");
                    rtc.month = getTwoDigitBCDFromKeypad();
                    _delay_ms(500);
                } while (rtc.month > 0x12);

                do
                {
                    LCD_Clear();
                    LCD_Printf("Set Year:");
                    rtc.year = getTwoDigitBCDFromKeypad();
                    _delay_ms(500);
                } while (rtc.year > 0x99);

                do
                {
                    LCD_Clear();
                    LCD_Printf("WeekDay[1-7]:");
                    rtc.weekDay = KEYPAD_GetKey() - '0'; // Single digit
                    LCD_DisplayChar(rtc.weekDay + '0');
                    _delay_ms(500);
                } while (rtc.weekDay > 7);

                function = 1;
                RTC_SetDateTime(&rtc);
                View_Fun(&rtc);
                _delay_ms(1000);

                LCD_Clear();
                LCD_Printf("Alarm ? B");

                if (KEYPAD_GetKey() == 'B')
                {
                    do
                    {
                        LCD_Clear();
                        LCD_Printf("Alarm Hour:");
                        Alarm_hrs = getTwoDigitBCDFromKeypad();
                        _delay_ms(500);
                    } while (Alarm_hrs > 0x23);

                    do
                    {
                        LCD_Clear();
                        LCD_Printf("Alarm Min:");
                        Alarm_mins = getTwoDigitBCDFromKeypad();
                        _delay_ms(500);
                    } while (Alarm_mins > 0x59);

                    LCD_Clear();
                    while (!exit_flag)
                    {
                        View_Fun(&rtc);
                        if (Alarm_hrs == rtc.hour && Alarm_mins == rtc.min)
                        {
                            // Turn on buzzer
                            GPIO_PinWrite(PA_3, HIGH);

                            while (1)
                            {
                                LCD_Clear();
                                LCD_Printf("Nap? B");
                                if (KEYPAD_GetKey() == 'B')
                                {
                                    GPIO_PinWrite(PA_3, LOW);
                                    LCD_Clear();
                                    LCD_Printf("Napp..");
                                    for (i = 0; i < 20; i++)
                                    {
                                        _delay_ms(1000);
                                    }
                                    GPIO_PinWrite(PA_3, HIGH);
                                }
                                else
                                {
                                	Alarm_mins=0x61; //to fail the if condition!
                                    break;
                                }
                            }
                            exit_flag=0;
                            LCD_Clear();
                            LCD_Printf("Turn OFF Buzzer");
                            while (GPIO_PinRead(PA_4));
                            GPIO_PinWrite(PA_3, LOW);
                            LCD_Clear();
                        }
                    }
                }
                else
                {
                    while (!exit_flag)
                    {
                        View_Fun(&rtc);
                    }
                    exit_flag=0;
                }
                break;

            case 'C':
                View_Fun(&rtc);
                _delay_ms(500);
                LCD_Clear();
                LCD_Printf("Alarm? B");
                function = 1;

                if (KEYPAD_GetKey() == 'B')
                {
                    do
                    {
                        LCD_Clear();
                        LCD_Printf("Alarm Hour:");
                        Alarm_hrs = getTwoDigitBCDFromKeypad();
                        _delay_ms(500);
                    } while (Alarm_hrs > 0x23);

                    do
                    {
                        LCD_Clear();
                        LCD_Printf("Alarm Min:");
                        Alarm_mins = getTwoDigitBCDFromKeypad();
                        _delay_ms(500);
                    } while (Alarm_mins > 0x59);

                    LCD_Clear();
                    while (!exit_flag)
                    {
                        View_Fun(&rtc);
                        if (Alarm_hrs == rtc.hour && Alarm_mins == rtc.min)
                        {
                            // Turn on buzzer
                            GPIO_PinWrite(PA_3, HIGH);

                            while (1)
                            {
                                LCD_Clear();
                                LCD_Printf("Nap? B");
                                if (KEYPAD_GetKey() == 'B')
                                {
                                    GPIO_PinWrite(PA_3, LOW);
                                    LCD_Clear();
                                    LCD_Printf("Napp..");
                                    for (i = 0; i < 20; i++)
                                    {
                                        _delay_ms(1000);
                                    }
                                    GPIO_PinWrite(PA_3, HIGH);
                                }
                                else
                                {
                                	Alarm_mins=0x61; //to fail the if condition!
                                    break;
                                }
                            }
                            LCD_Clear();
                            LCD_Printf("Turn OFF Buzzer");
                            while (GPIO_PinRead(PA_4));
                            GPIO_PinWrite(PA_3, LOW);
                            LCD_Clear();
                        }
                    }
                    exit_flag=0;
                }
                else
                {
                    function = 1;
                    while (!exit_flag)
                    {
                        View_Fun(&rtc);
                    }
                    exit_flag=0;
                }
                break;

            case 'D':
                Stop_Watch_Fun();
                break;

            case 'E':
                Down_Counter_Fun();
                break;

            default:
                LCD_Clear();
                LCD_Printf("Invalid Choice!");
                _delay_ms(200);
                break;
        }
    }

    return 0;
}
