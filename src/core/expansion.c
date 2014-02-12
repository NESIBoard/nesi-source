/**
* NESI Expansion Driver License
* Copyright (C) 2013 Dakotah Karrer
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "system.h"
#include "expansion.h"

void initialize(void)
{
   /* no Initialization needed*/
}

void setDigitalOut(int pin, int setOutputState)
{
/**
    * Channel Pin Configuration
    *
    * The pins are configured as digital outputss.
    */

/*Converting setOutputState to either digital high or low */
 setOutputState = !!setOutputState;

   switch(pin)
   {
       case(3):
          _TRISG9 = 0;              /*Set direction as output*/
          _PCFG9  = 1;              /*Pin is in digital mode*/
          _RG9 = setOutputState;    /*Voltage level is either high (1) or low (0)*/
           break;
       case(4):
          _TRISB5 = 0;
          _PCFG5  = 1;
          _RB5 = setOutputState;
           break;
       case(5):
          _TRISB10 = 0;
          _PCFG10  = 1;
          _RB10 = setOutputState;
           break;
       case(6):
          _TRISB11 = 0;
          _PCFG11  = 1;
          _RB11 = setOutputState;
           break;
       case (7):
          _TRISB12 = 0;
          _PCFG12  = 1;
          _RB12 = setOutputState;
           break;
       case(8):
          _TRISB13 = 0;
          _PCFG13  = 1;
          _RB13 = setOutputState;
           break;
       case(9):
          _TRISF3 = 0;
          _RF3 = setOutputState;
           break;
       case(10):
          _TRISD6 = 0;
          _RD6 = setOutputState;
           break;
       default:         /*All other pins cannot be configured as analog inputs*/
           break;
    }
}



const Expansion expansion = {init:initialize, digitalOut:setDigitalOut};
