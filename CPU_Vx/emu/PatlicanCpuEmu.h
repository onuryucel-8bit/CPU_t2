#pragma once

#include <iostream>

enum OPCODE
{
	HLT = 0x00,

	LOAD = 0x01,
	LOADa = 0x02,
	STR = 0x03,
	MOV = 0x04,

	OUT = 0x05,
	OUTa = 0x06,

	ADD = 0x08,
	SUB = 0x09,
	SHL = 0x0A,
	SHR = 0x0B,

	AND = 0x0C,
	OR = 0x0D,
	NOT = 0x0E,
	XOR = 0x0F,

	ADDs = 0x10,
	SUBs = 0x11,
	ANDs = 0x14,
	ORs = 0x15,
	XORs = 0x17,

	JMP = 0x20,
	JZ = 0x21,
	JLZ = 0x22,
	JGZ = 0x23,

	JSC = 0x25,
	JUC = 0x26
};
 
class PatlicanCpuEmu
{
public:
	PatlicanCpuEmu(int* _ram, size_t ramSize);
	~PatlicanCpuEmu();
	
	void run();

private:

	static const int RAMSIZE = 256;	
	int ram[RAMSIZE];


	int programCounter = 0;
	int regs[8];

	int rx, ry;
	int sayi;
	int opcode;

	int ACC = 0;

	void printRegs();
	void clearRegs();

	void LOAD0x1();
	void MOV0x4();
	void ADD0x8();
	void SUBs0x11();
	void JGZ0x23();
	void LOAD0x2();
};



