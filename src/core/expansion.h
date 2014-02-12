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

#ifndef EXPANSION_H
#define	EXPANSION_H

/**
* Microcontroller Resource Allocation
*
* The following microcontroller resources are used in this module.
*   - Pins:
 *          G9  (DO)
 *          G5  (DO)
 *          G10 (DO)
 *          G11 (DO)
 *          G12 (DO)
 *          G13 (DO)
 *          F3  (DO)
 *          D6  (D0)
*/

#include "system.h"

typedef struct {
    /**
    * Initializes the powerDriver software module
    */
   void (*init)(void);
    /**
    * Configures the desired pin on the expansion port as an digital output
    * @param pin the particular pin on the expansion port to map as digital output
    */
   void (*digitalOut)(int pin, int setOutputState);
   
} Expansion;

extern const Expansion expansion;

#endif	/* EXPANSION_H */
