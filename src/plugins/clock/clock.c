/*
    DDDCPU16 - DeathlyDeep's DCPU16, a DCPU16 emulator.
    Copyright (C) 2012 Sylvain BOILARD <boilard@crans.org>

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

#include "clock.h"

struct dddcpu16_context context;
unsigned short clock_number = 0;
struct clock_context* clock_array = NULL;

static unsigned long pgcd60(unsigned long n)
{
    unsigned long k = 60;
    while (n %= k)
        if (!(k %= n))
            return n;
    return k;
}

static void info(void)
{
    context.registers[0] = 0xb402;
    context.registers[1] = 0x12d0;
    context.registers[2] = 0;
    context.registers[3] = 0;
    context.registers[4] = 0;
}

static void tick(void* arguments)
{
    struct clock_context* current_clock = (struct clock_context*)arguments;

    if (current_clock->interrupt)
        context.send_int(current_clock->interrupt);
    current_clock->trigger_tick += current_clock->cycles_per_tick;
    current_clock->current_drift += current_clock->drift_per_tick;
    if (current_clock->current_drift >= current_clock->max_drift)
    {
        ++current_clock->trigger_tick;
        current_clock->current_drift -= current_clock->max_drift;
    }
    current_clock->event_ID =
        context.schedule_event(current_clock->trigger_tick, tick, arguments);
}

static unsigned int recv_int(unsigned short PCID)
{
    unsigned short reg_B = context.registers[1];

    switch (context.registers[0])
    {
    case 0:
        clock_array[PCID].last_set = *context.cycles_counter;
        if (clock_array[PCID].event_ID)
            context.cancel_event(clock_array[PCID].event_ID, NULL);
        if (reg_B)
        {
            unsigned long freq_x_reg_B = *context.emu_freq * reg_B;
            clock_array[PCID].max_drift = 60 / pgcd60(freq_x_reg_B);
            clock_array[PCID].drift_per_tick =
                freq_x_reg_B % clock_array[PCID].max_drift;
            clock_array[PCID].current_drift = clock_array[PCID].drift_per_tick;
            clock_array[PCID].cycles_per_tick = freq_x_reg_B / 60;
            clock_array[PCID].trigger_tick =
                *context.cycles_counter + clock_array[PCID].cycles_per_tick;
            clock_array[PCID].event_ID =
                context.schedule_event(clock_array[PCID].trigger_tick, tick,
                                       clock_array + PCID);
        }
        else
            clock_array[PCID].event_ID = 0;

        break;

    case 1:
        context.registers[2] =
            *context.cycles_counter - clock_array[PCID].last_set;
        break;

    case 2:
        clock_array[PCID].interrupt = reg_B;

    default:;
    }

    return 0;
}

int init(const struct dddcpu16_context* dddcpu16_context,
         int argc, char* argv[])
{
    context = *dddcpu16_context;
    ++clock_number;
    return 0;
}

void complete_load(void)
{
    unsigned int i;

    clock_array = (struct clock_context*)
        malloc(sizeof(struct clock_context) * clock_number);
    for (i = 0; i < clock_number; ++i)
    {
        context.add_hard(info, recv_int, i);

        /* Other values are set when needed. */
        clock_array[i].interrupt = 0;
        clock_array[i].event_ID = 0;
    }
}

void term(void)
{
    if (clock_array)
    {
        unsigned int i;

        for (i = 0; i < clock_number; ++i)
            if (clock_array[i].event_ID)
                context.cancel_event(clock_array[i].event_ID, NULL);
        free(clock_array);
    }
}
