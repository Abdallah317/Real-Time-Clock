/***************************************************************************************************
                                   ExploreEmbedded	
 ****************************************************************************************************
 * File:   keypad.c
 * Version: 16.0
 * Author: ExploreEmbedded
 * Website: http://www.exploreembedded.com/wiki
 * Description: Contains the library routines for 4x4 Hex-Keypad

The libraries have been tested on ExploreEmbedded development boards. We strongly believe that the 
library works on any of development boards for respective controllers. However, ExploreEmbedded 
disclaims any kind of hardware failure resulting out of usage of libraries, directly or indirectly.
Files may be subject to change without prior notice. The revision history contains the information 
related to updates. 


GNU GENERAL PUBLIC LICENSE: 
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

Errors and omissions should be reported to codelibraries@exploreembedded.com
 **************************************************************************************************/



/***************************************************************************************************
                             Revision History
 ****************************************************************************************************
15.0: Initial version 
16.0: KEYPAD_Init function is updated to specify the row/col connections as parameters.
      Included the gpio functions to read/write the individual row/col.
 ***************************************************************************************************/


/***************************************************************************************************
                             PORT configurations/Connections
 ****************************************************************************************************
            ___________________
           |    |    |    |    |
           | 0  | 1  | 2  | 3  |--------- R0
           |____|____|____|____|
           |    |    |    |    |
		   | 4  | 5  | 6  | 7  |--------- R1
           |____|____|____|____|
           |    |    |    |    |
		   | 8  | 9  | A  | B  |--------- R2
		   |____|____|____|____|
		   |    |    |    |    |
		   | C  | D  | E  | F  |--------- R3
           |____|____|____|____|
             |    |    |    |
             |    |    |    |____________ C3
             |    |    |
             |    |    |_________________ C2
             |    |
             |    |______________________ C1
             |
             |___________________________ C0

 ****************************************************************************************************/


/***************************************************************************************************
                                   ExploreEmbedded
 ****************************************************************************************************
 * File:   keypad.c
 * Version: 16.0 (Modified for AVR)
 * Description: Contains the library routines for 4x4 Hex-Keypad with GPIO fixes and pull-ups
 ***************************************************************************************************/

#include "keypad.h"


gpioPins_et A_RowsPins_U8[C_MaxRows_U8];
gpioPins_et A_ColsPins_U8[C_MaxCols_U8];         

uint8_t A_KeyLookUptable_U8[C_MaxRows_U8][C_MaxCols_U8]= 
{
    { '0','1','2','3' },
    { '4','5','6','7' },
    { '8','9','A','B' },
    { 'C','D','E','F' }
};

static void keypad_WaitForKeyRelease(void);
static void keypad_WaitForKeyPress(void);


/***************************************************************************************************
                   void KEYPAD_Init()
 ***************************************************************************************************/
void KEYPAD_Init(
        gpioPins_et row_0, 
        gpioPins_et row_1, 
        gpioPins_et row_2,
        gpioPins_et row_3, 
        gpioPins_et col_0,
        gpioPins_et col_1, 
        gpioPins_et col_2,
        gpioPins_et col_3 )
{
    uint8_t i;

    A_RowsPins_U8[0] = row_0;
    A_RowsPins_U8[1] = row_1;
    A_RowsPins_U8[2] = row_2;
    A_RowsPins_U8[3] = row_3;

    A_ColsPins_U8[0] = col_0;
    A_ColsPins_U8[1] = col_1;
    A_ColsPins_U8[2] = col_2;
    A_ColsPins_U8[3] = col_3;

    for(i = 0; i < C_MaxRows_U8; i++)
    {
        GPIO_PinDirection(A_RowsPins_U8[i], OUTPUT);
        GPIO_PinWrite(A_RowsPins_U8[i], HIGH);  // initialize HIGH
    }

    for(i = 0; i < C_MaxCols_U8; i++)
    {
        GPIO_PinDirection(A_ColsPins_U8[i], INPUT);
        GPIO_PinWrite(A_ColsPins_U8[i], HIGH);  // Enable internal pull-up
    }
}


/***************************************************************************************************
                  uint8_t KEYPAD_GetKey()
 ***************************************************************************************************/
uint8_t KEYPAD_GetKey(void)
{
    uint8_t i,j,v_KeyPressed_u8 = 0;


    keypad_WaitForKeyRelease();
    keypad_WaitForKeyPress();

    for (i=0;i<C_MaxRows_U8;i++)
    {
        GPIO_PinWrite(A_RowsPins_U8[i],HIGH);
    }

    for (i=0;(i<C_MaxRows_U8);i++)
    {
        GPIO_PinWrite(A_RowsPins_U8[i],LOW);

        for(j=0; (j<C_MaxCols_U8); j++)
        {
            if(GPIO_PinRead(A_ColsPins_U8[j]) == 0)
            {
                v_KeyPressed_u8 = 1;
                break;
            }
        }

        if(v_KeyPressed_u8 ==1)
        {
            break;
        }

        GPIO_PinWrite(A_RowsPins_U8[i],HIGH);
    }

    if(i<C_MaxRows_U8)
        v_KeyPressed_u8 = A_KeyLookUptable_U8[i][j];
    else
        v_KeyPressed_u8 = C_DefaultKey_U8;


    return v_KeyPressed_u8;
}



/***************************************************************************************************
                   static void keypad_WaitForKeyRelease(void)
 ***************************************************************************************************/
static void keypad_WaitForKeyRelease(void)
{
    uint8_t i, v_keyStatus_u8;

    for (i = 0; i < C_MaxRows_U8; i++)
    {
        GPIO_PinWrite(A_RowsPins_U8[i], LOW);
    }

    do
    {
        do
        {
            v_keyStatus_u8 = 1;
            for(i = 0; i < C_MaxCols_U8; i++)
            {
                v_keyStatus_u8 &= GPIO_PinRead(A_ColsPins_U8[i]);
            } 
        } while(v_keyStatus_u8 == 0);

        DELAY_us(C_DebounceTimeInMicroSecond_U16);

        v_keyStatus_u8 = 1;
        for(i = 0; i < C_MaxCols_U8; i++)
        {
            v_keyStatus_u8 &= GPIO_PinRead(A_ColsPins_U8[i]);
        } 
    } while(v_keyStatus_u8 == 0);
}


/***************************************************************************************************
                   static void keypad_WaitForKeyPress(void)
 ***************************************************************************************************/
static void keypad_WaitForKeyPress(void)
{
    uint8_t i, v_keyStatus_u8;

    for (i = 0; i < C_MaxRows_U8; i++)
    {
        GPIO_PinWrite(A_RowsPins_U8[i], LOW);
    }

    do
    {
        do
        {
            v_keyStatus_u8 = 1;
            for(i = 0; i < C_MaxCols_U8; i++)
            {
                v_keyStatus_u8 &= GPIO_PinRead(A_ColsPins_U8[i]);
            } 
        } while(v_keyStatus_u8 != 0);

        DELAY_us(C_DebounceTimeInMicroSecond_U16);

        v_keyStatus_u8 = 1;
        for(i = 0; i < C_MaxCols_U8; i++)
        {
            v_keyStatus_u8 &= GPIO_PinRead(A_ColsPins_U8[i]);
        } 
    } while(v_keyStatus_u8 != 0);
}

