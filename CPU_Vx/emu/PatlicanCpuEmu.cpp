#include "PatlicanCpuEmu.h"

PatlicanCpuEmu::PatlicanCpuEmu(int* _ram, size_t ramSize)
{
	std::copy(_ram, _ram + ramSize, ram);
	outReg = 0;
}

PatlicanCpuEmu::~PatlicanCpuEmu()
{
}

void PatlicanCpuEmu::printRegs()
{
	std::cout << "--------------Regs--------------\n";
	for (size_t i = 0; i < 8; i++)
	{
		std::cout << "r :" << i << " | " << regs[i] << "\n";
	}

	std::cout << "-------------Out Reg--------------\n";
	std::cout << outReg << "\n";
}

void PatlicanCpuEmu::printOutReg()
{

}

void PatlicanCpuEmu::clearRegs()
{
	/*
	for (size_t i = 0; i < 8; i++)
	{
		regs[i] = 0;
	}
	*/
	std::fill(regs, regs + 8, 0);
}

void PatlicanCpuEmu::run()
{
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

		case OPCODE::OUT:
			OUT0x5();
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



void PatlicanCpuEmu::LOAD0x1()
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


void PatlicanCpuEmu::LOAD0x2()
{
	
	//2.byte
	programCounter++;
	int secondByte = ram[programCounter];
	rx = secondByte & 0x07;

	//3.byte	
	programCounter++;
	int adres = ram[programCounter];

	regs[rx] = ram[adres];
}

void PatlicanCpuEmu::STR0x3()
{
	programCounter++;
	int regByte = ram[programCounter];

	int rx = regByte & 0x07;
	
	programCounter++;
	int adres = ram[programCounter];

	ram[adres] = regs[rx];
}

void PatlicanCpuEmu::MOV0x4()
{
	programCounter++;
	opcode = ram[programCounter];

	rx = opcode & 0x38;
	rx >>= 3;
	ry = opcode & 0x07;

	regs[rx] = regs[ry];
}

void PatlicanCpuEmu::OUT0x5()
{
	programCounter++;
	int regByte = ram[programCounter];

	int rx = regByte & 0x07;

	outReg = regs[rx];
}

void PatlicanCpuEmu::ADD0x8()
{
	programCounter++;
	opcode = ram[programCounter];

	rx = opcode & 0x38;
	rx >>= 3;
	ry = opcode & 0x07;

	regs[rx] = regs[rx] + regs[ry];

	ACC = regs[rx];
}

void PatlicanCpuEmu::SUBs0x11()
{
	programCounter++;
	opcode = ram[programCounter];

	rx = opcode & 0x07;

	programCounter++;
	sayi = ram[programCounter];

	regs[rx] = regs[rx] - sayi;

	ACC = regs[rx];
}

void PatlicanCpuEmu::JGZ0x23()
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
