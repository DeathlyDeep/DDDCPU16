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

#ifndef EVENTS_H_INCLUDED
#define EVENTS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "globals.h"

#define MAX_EVENTS 1024 // TODO : add option to modify this value.

struct event
{
    unsigned long trigger;
    unsigned long event_ID;
    void (* callback)(void*);
    void* arguments;
};

unsigned long schedule_event(unsigned long trigger, void (* callback)(void*),
                             void* arguments);
void cancel_event(unsigned long event_ID, void (* callback)(void*));
void trigger_events(void);

#endif // EVENTS_H_INCLUDED
