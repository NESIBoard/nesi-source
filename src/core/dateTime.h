/**
 * NESI DateTime Module License
 * Copyright (C) 2013 Mickie Byrd
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

#ifndef DATETIME_H
#define DATETIME_H

/**
 * Microcontroller Resource Allocation
 *
 * The following microcontroller resources are used in this module.
 *   - Pins: C12 (Clock Input), C15 (Clock Out)
 *   - Peripherals: RTCC (Real Time Clock and Calendar),
 *                  Timer 4 (millisecond count- currently unused)
 */

/**
 * Change Log
 *
 * 12/19/2013 - Mickie Byrd
 *   Added .toStamp() and .parseStamp() to handle time stamp string processing.
 *   Added .new() member to be used for initializing DateAndTime variables.
 * 12/24/2013 - Mickie Byrd
 *   Added .compare() to be used for comparing two DateAndTime objects.
 */

/**
 * TODO
 *
 * Things left to do:
 *   comments - functions and code
 *   refactoring - simplification and condensing
 */

#include "system.h"

typedef struct {
    signed second;
    signed minute;
    signed hour;
} Time;

typedef struct {
    signed weekday;
    signed day;
    signed month;
    signed year;
} Date;

typedef struct {
    union {
        Time time;
        struct {
            signed second;
            signed minute;
            signed hour;
        };
    };
    union {
        Date date;
        struct {
            signed weekday;
            signed day;
            signed month;
            signed year;
        };
    };
} DateAndTime;

typedef enum {
    SUNDAY, MONDAY, TUESDAY, WEDNESDAY, THURSDAY, FRIDAY, SATURDAY
} Weekday;

typedef enum {
    JANUARY = 1, FEBUARY, MARCH, APRIL, MAY, JUNE,
    JULY, AUGUST, SEPTEMBER, OCTOBER, NOVEMBER, DECEMBER
} Month;


typedef struct {
    void (*init)(void);
    DateAndTime (*get)(void);
    void (*set)(DateAndTime);
    DateAndTime (*new)(void);
    String (*toStamp)(DateAndTime);
    String (*getStamp)(void);
    DateAndTime (*parseStamp)(String);
    /**
     * Checks if time a is after time b
     * @return negative if b is after a, zero if same, positive if a is after b
     */
    Sint (*compare)(DateAndTime a, DateAndTime b);
} DateTime;

extern const DateTime dateTime;

#endif  /* DATETIME_H */
