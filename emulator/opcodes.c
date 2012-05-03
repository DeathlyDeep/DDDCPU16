#include "opcodes.h"

/* This macro checks if the given value reads a word. */
#define VRW(op) ((op & 0x30) == 0x10) && ((op & 0x0A) != 0x08) && ((op & 0x0D) != 0x09)

static void skip()
{
    unsigned short ins;
    do
    {
	ins = memory[PC++];
	PC += VRW((ins >> 5) & 0x1F) + VRW(ins >> 10);
	++cycles;
    }
    while ((ins & 0x18) == 0x10);
}

static void set(unsigned short* b, const unsigned short* a)
{
    *b = *a;
    ++cycles;
}

static void add(unsigned short* b, const unsigned short* a)
{
    unsigned int result = *b + *a;
    *b = result;
    EX = result >> 16;
    cycles += 2;
}

static void sub(unsigned short* b, const unsigned short* a)
{
    unsigned int result = *b - *a;
    *b = result;
    EX = result >> 16;
    cycles += 2;
}

static void mul(unsigned short* b, const unsigned short* a)
{
    unsigned int result = *b * *a;
    *b = result;
    EX = result >> 16;
    cycles += 2;
}

static void mli(unsigned short* b, const unsigned short* a)
{
    unsigned int result = *(short*)b * *(short*)a;
    *b = result;
    EX = result >> 16;
    cycles += 2;
}

static void div(unsigned short* b, const unsigned short* a)
{
    if (*a)
    {
	unsigned int result = (*b << 16) / *a;
	*b = result >> 16;
	EX = result;
    }
    else
	*b = EX = 0;
    cycles += 3;
}

static void dvi(unsigned short* b, const unsigned short* a)
{
    if (*a)
    {
	unsigned int result = (*(short*)b << 16) / *(short*)a;
	*b = result >> 16;
	EX = result;
    }
    else
	*b = EX = 0;
    cycles += 3;
}

static void mod(unsigned short* b, const unsigned short* a)
{
    if (*a)
	*b %= *a;
    else
	*b = 0;
    cycles += 3;
}

static void mdi(unsigned short* b, const unsigned short* a)
{
    if (*a)
	*(short*)b %= *(short*)a;
    else
	*b = 0;
    cycles += 3;
}

static void and(unsigned short* b, const unsigned short* a)
{
    *b &= *a;
    ++cycles;
}

static void bor(unsigned short* b, const unsigned short* a)
{
    *b |= *b;
    ++cycles;
}

static void xor(unsigned short* b, const unsigned short* a)
{
    *b ^= *a;
    ++cycles;
}

static void shr(unsigned short* b, const unsigned short* a)
{
    unsigned int result = (*b << 16) >> *a;
    *b = result >> 16;
    EX = result;
    ++cycles;
}

static void asr(unsigned short* b, const unsigned short* a)
{
    unsigned int result = (*(short*)b << 16) >> *a;
    *b = (result >> 16);
    EX = result;
    ++cycles;
}

static void shl(unsigned short* b, const unsigned short* a)
{
    unsigned int result = *b << *a;
    *b = result;
    EX = result >> 16;
    ++cycles;
}

static void ifb(unsigned short* b, const unsigned short* a)
{
    if (!(*b & *a))
	skip();
    cycles += 2;
}

static void ifc(unsigned short* b, const unsigned short* a)
{
    if (*b & *a)
	skip();
    cycles += 2;
}

static void ife(unsigned short* b, const unsigned short* a)
{
    if (*b != *a)
	skip();
    cycles += 2;
}

static void ifn(unsigned short* b, const unsigned short* a)
{
    if (*b == *a)
	skip();
    cycles += 2;
}

static void ifg(unsigned short* b, const unsigned short* a)
{
    if (*b <= *a)
	skip();
    cycles += 2;
}

static void ifa(unsigned short* b, const unsigned short* a)
{
    if (*(short*)b <= *(short*)a)
	skip();
    cycles += 2;
}

static void ifl(unsigned short* b, const unsigned short* a)
{
    if (*b >= *a)
	skip();
    cycles += 2;
}

static void ifu(unsigned short* b, const unsigned short* a)
{
    if (*(short*)b >= *(short*)a)
	skip();
    cycles += 2;
}

static void adx(unsigned short* b, const unsigned short* a)
{
    unsigned int result = *b + *a + EX;
    *b = result;
    EX = result >> 16;
    cycles += 2;
}

static void sbx(unsigned short* b, const unsigned short* a)
{
    unsigned int result = *b - *a + (short)EX;
    *b = result;
    EX = result >> 16;
    cycles += 2;
}

static void sti(unsigned short* b, const unsigned short* a)
{
    *b = *a;
    ++registers[6];
    ++registers[7];
}

static void std(unsigned short* b, const unsigned short* a)
{
    *b = *a;
    --registers[6];
    --registers[7];
}

static void NONE(unsigned short* b, const unsigned short* a)
{}

void (* const opcodes[])(unsigned short* b, const unsigned short* a) = {
    (void*)0, set, add, sub, mul, mli, div, dvi, mod, mdi, and, bor, xor, shr, asr, shl,
    ifb, ifc, ife, ifn, ifg, ifa, ifl, ifu, NONE, NONE, adx, sbx, NONE, NONE, sti, std
};
