/*
    DDDCPU16 - DeathlyDeep's DCPU16, a DCPU16 emulator.
    Copyright (C) 2012-2013 Sylvain Boilard <boilard@crans.org>

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
*/

#ifndef CLOCK_H_INCLUDED
#define CLOCK_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>

#include <dddcpu16.h>

int init(const struct dddcpu16_context* dddcpu16_context,
         int argc, char* argv[]);
void term(void);

struct clock_context
{
    unsigned long last_set;
    unsigned long trigger_tick;
    unsigned long event_ID;
    unsigned long cycles_per_tick;
    unsigned char drift_per_tick;
    unsigned char current_drift;
    unsigned short interrupt;
};

#endif // CLOCK_H_INCLUDED
