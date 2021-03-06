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

unsigned short memory[0x10000] = {0};
unsigned short registers[8] = {0};
unsigned short PC = 0;
unsigned short SP = 0;
unsigned short EX = 0;

unsigned long cycles_counter = 0;
unsigned long emu_freq = 100000; // 100 kHz
unsigned int emu_speed = 1000; // Exprimed by 10E-3 increments. Default is 1.
unsigned int emu_granularity = 1000000; // 1 ms
