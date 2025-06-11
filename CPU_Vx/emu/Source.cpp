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

const int RAMSIZE = 256;

int ram[RAMSIZE] = 
{ 
	LOAD,0x00,0x05,
	LOAD,0x01,0x00,

	ADD,0x08,
	SUBs,0x00,0x01,

	JGZ,0x06,0x00
};


int programCounter = 0;
int regs[8];

int rx, ry;
int sayi;
int opcode;

int ACC = 0;

void clearRAM()
{
	for (size_t i = 0; i < RAMSIZE; i++)
	{
		ram[i] = 0;
	}
}

void clearRegs()
{
	for (size_t i = 0; i < 8; i++)
	{
		regs[i] = 0;
	}
}
/*
HLT	0000_0000	0x0

LOAD 0000_0001	0x01
LOAD 0000_0010	0x02
STR	0000_0011	0x03
MOV	0000_0100	0x04

OUT	0000_0101	0x05
OUT	0000_0110	0x06


ADD	0000_1000	0x08
SUB	0000_1001	0x09
SHL	0000_1010	0x0a
SHR	0000_1011	0x0b

AND	0000_1100	0x0c
OR	0000_1101	0x0d
NOT	0000_1110	0x0e
XOR	0000_1111	0x0f


ADD	0001_0000	0x10
SUB	0001_0001	0x11
AND	0001_0100	0x14
OR	0001_0101	0x15
XOR	0001_0111	0x17


JMP	0010_0000	0x20
JZ	0010_0001	0x21
JLZ	0010_0010	0x22
JGZ	0010_0011	0x23

JSC	0010_0101	0x25
JUC	0010_0110	0x26
*/


void LOAD0x1()
{
	//2.byte
	programCounter++;
	opcode = ram[programCounter];
	rx = opcode & 0x07;

	//3.byte
	programCounter++;
	sayi = ram[programCounter];

	regs[rx] = sayi;
}

void MOV0x4()
{
	programCounter++;
	opcode = ram[programCounter];

	rx = opcode & 0x38;
	rx >>= 3;
	ry = opcode & 0x07;

	regs[rx] = regs[ry];
}

void ADD0x8()
{
	programCounter++;
	opcode = ram[programCounter];

	rx = opcode & 0x38;
	rx >>= 3;
	ry = opcode & 0x07;

	regs[rx] = regs[rx] + regs[ry];

	ACC = regs[rx];
}

void SUBs0x11()
{
	programCounter++;
	opcode = ram[programCounter];

	rx = opcode & 0x07;

	programCounter++;
	sayi = ram[programCounter];
	
	regs[rx] = regs[rx] - sayi;

	ACC = regs[rx];
}

void JGZ0x23()
{
	//if acc > 0
	if (ACC > 0)
	{
		programCounter++;
		//adresi al
		int adres = ram[programCounter];

		programCounter = adres;
	}
	else
	{
		//ziplama komutunu atla
		programCounter += 2;
	}
}

void printRegs()
{
	std::cout << "--------------Regs--------------\n";
	for (size_t i = 0; i < 8; i++)
	{
		std::cout << "r :" << i  <<" | " << regs[i] << "\n";
	}
}

int main()
{
	//clearRAM();
	clearRegs();

	

	

	bool running = true;
	while (running)
	{
		
		opcode = ram[programCounter];
		
		switch (opcode)
		{
		case OPCODE::HLT:
			running = false;
			break;

		case OPCODE::LOAD:
			LOAD0x1();
			programCounter++;
			break;

		case OPCODE::MOV:						
			MOV0x4();
			programCounter++;
			break;

		case OPCODE::SUBs:
			SUBs0x11();
			programCounter++;
			break;

		case OPCODE::ADD:
			ADD0x8();
			programCounter++;
			break;

		case OPCODE::JGZ:
			JGZ0x23();
			break;
		}
		
	}
	printRegs();
}