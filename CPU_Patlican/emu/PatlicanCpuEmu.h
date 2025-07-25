#pragma once

#include <iostream>
#include <cstdint>

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

	ADC = 0x18,

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
	PatlicanCpuEmu(uint8_t* _ram, size_t ramSize);
	~PatlicanCpuEmu();
	
	void run();

	void reset();
	void printRegs();

private:

	static const int RAMSIZE = 256;	
	uint8_t ram[RAMSIZE];


	int programCounter = 0;
	uint8_t regs[8];
	uint8_t outReg;

	uint8_t currentOpcode;

	int sumCarry = 0;
	int underflow = 0;
	uint8_t ACC = 0;
	
	void clearRegs();

	//--------------------------//
	void LOAD0x1();
	void LOAD0x2();
	void STR0x3();
	void MOV0x4();
	void OUT0x5();
	void OUT0x6();

	//--------------------------//
	void ADD0x8();
	void SUB0x9();
	void SHL0xa();
	void SHR0xb();

	void AND0xc();
	void OR0xd();
	void NOT0xe();
	void XOR0xf();

	void ADD0x10();
	void SUB0x11();
	void AND0x14();
	void OR0x15();
	void XOR0x17();

	void ADC0x18();
	//--------------------------//

	void JMP0x20();
	void JZ0x21();
	void JLZ0x22();
	void JGZ0x23();
	
	void JSC0x25();
	void JUC0x26();
	//--------------------------//
};



