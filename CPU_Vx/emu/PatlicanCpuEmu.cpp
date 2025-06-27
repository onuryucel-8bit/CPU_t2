#include "PatlicanCpuEmu.h"

PatlicanCpuEmu::PatlicanCpuEmu(uint8_t* _ram, size_t ramSize)
{
	std::copy(_ram, _ram + ramSize, ram);
	
	reset();
}

PatlicanCpuEmu::~PatlicanCpuEmu()
{
}

void PatlicanCpuEmu::reset()
{
	clearRegs();

	outReg = 0;

	rx = 0;
	ry = 0;

	currentOpcode = 0;
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

		currentOpcode = ram[programCounter];
		
		switch (currentOpcode)
		{
		case OPCODE::HLT:
			running = false;
			break;

		case OPCODE::LOAD:
			LOAD0x1();
			programCounter++;
			break;
		case OPCODE::LOADa:
			LOAD0x2();			
			break;
		case OPCODE::STR:
			STR0x3();
			break;
		case OPCODE::MOV:
			MOV0x4();
			break;
		case OPCODE::OUT:
			OUT0x5();
			break;
		case OPCODE::OUTa:
			OUT0x6();			
			break; 
		

		case OPCODE::ADD:
			ADD0x8();			
			break;
		case OPCODE::SUB:
			SUB0x9();
			break;
		case OPCODE::SHL:
			SHL0xa();
			break;
		case OPCODE::SHR:
			SHR0xb();
			break;
		case OPCODE::AND:
			AND0xc();
			break;
		case OPCODE::OR:
			OR0xd();
			break;
		case OPCODE::NOT:
			NOT0xe();
			break;
		case OPCODE::XOR:
			XOR0xf();
			break;

		case OPCODE::ADDs:
			ADD0x10();
			break;
		case OPCODE::SUBs:
			SUB0x11();
			break;
		case OPCODE::ORs:
			OR0x15();
			break;
		case OPCODE::XORs:
			XOR0x17();
			break;

		case OPCODE::JMP:
			JMP0x20();
			break;
		case OPCODE::JZ:
			JZ0x21();
			break;
		case OPCODE::JLZ:
			JLZ0x22();
			break;
		case OPCODE::JGZ:
			JGZ0x23();
			break;
		case OPCODE::JSC:
			JSC0x25();
			break;
		case OPCODE::JUC:
			JUC0x26();
			break;

		}

	}
	printRegs();
}


//-----------------------------------------------------------------------------------------------------//
//---------------------------------OPCODES-------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------//

void PatlicanCpuEmu::LOAD0x1()
{
	//2.byte
	programCounter++;
	int byte = ram[programCounter];
	rx = byte & 0x07;

	//3.byte
	programCounter++;
	int sayi = ram[programCounter];

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
	int byte = ram[programCounter];

	rx = byte & 0x38;
	rx >>= 3;
	ry = byte & 0x07;

	regs[rx] = regs[ry];
}

void PatlicanCpuEmu::OUT0x5()
{
	programCounter++;
	int regByte = ram[programCounter];

	int rx = regByte & 0x07;

	outReg = regs[rx];
}

void PatlicanCpuEmu::OUT0x6()
{
	programCounter++;
	int adres = ram[programCounter];

	outReg = ram[adres];
}

void PatlicanCpuEmu::ADD0x8()
{
	programCounter++;
	int regByte = ram[programCounter];

	int rx = regByte & 0x38;
	rx >>= 3;
	int ry = regByte & 0x07;

	regs[rx] = regs[rx] + regs[ry];

	ACC = regs[rx];
}

void PatlicanCpuEmu::SUB0x9()
{
	programCounter++;
	int regByte = ram[programCounter];

	int rx = regByte & 0x38;
	rx >>= 3;
	int ry = regByte & 0x07;

	regs[rx] = regs[rx] - regs[ry];

	ACC = regs[rx];
}

void PatlicanCpuEmu::SHL0xa()
{
	programCounter++;
	int regByte = ram[programCounter];

	int rx = regByte & 0x38;
	rx >>= 3;
	int shiftAmount = regByte & 0x07;

	regs[rx] = regs[rx] << regs[ry];

	ACC = regs[rx];
}

void PatlicanCpuEmu::SHR0xb()
{
	programCounter++;
	int regByte = ram[programCounter];

	int rx = regByte & 0x38;
	rx >>= 3;
	int shiftAmount = regByte & 0x07;

	regs[rx] = regs[rx] >> regs[ry];

	ACC = regs[rx];
}

void PatlicanCpuEmu::AND0xc()
{
	programCounter++;
	int regByte = ram[programCounter];

	int rx = regByte & 0x38;
	rx >>= 3;
	int ry = regByte & 0x07;

	regs[rx] = regs[rx] & regs[ry];

	ACC = regs[rx];
	
}


void PatlicanCpuEmu::OR0xd()
{
	programCounter++;
	int regByte = ram[programCounter];

	int rx = regByte & 0x38;
	rx >>= 3;
	int ry = regByte & 0x07;

	regs[rx] = regs[rx] | regs[ry];

	ACC = regs[rx];
}

void PatlicanCpuEmu::NOT0xe()
{
	programCounter++;
	int rx = ram[programCounter];
	
	regs[rx] = ~regs[rx];

	ACC = regs[rx];
}

void PatlicanCpuEmu::XOR0xf()
{
	programCounter++;
	int regByte = ram[programCounter];

	int rx = regByte & 0x38;
	rx >>= 3;
	int ry = regByte & 0x07;

	regs[rx] = regs[rx] ^ regs[ry];

	ACC = regs[rx];
}

void PatlicanCpuEmu::ADD0x10()
{
	programCounter++;
	int rx = ram[programCounter];

	programCounter++;
	int number = ram[programCounter];

	regs[rx] = regs[rx] + number;

	ACC = regs[rx];
}

void PatlicanCpuEmu::SUB0x11()
{
	programCounter++;
	int rx = ram[programCounter];

	programCounter++;
	int number = ram[programCounter];

	regs[rx] = regs[rx] - number;

	ACC = regs[rx];
}

void PatlicanCpuEmu::AND0x14()
{
	programCounter++;
	int rx = ram[programCounter];

	programCounter++;
	int number = ram[programCounter];

	regs[rx] = regs[rx] & number;

	ACC = regs[rx];
}

void PatlicanCpuEmu::OR0x15()
{
	programCounter++;
	int rx = ram[programCounter];

	programCounter++;
	int number = ram[programCounter];

	regs[rx] = regs[rx] | number;

	ACC = regs[rx];
}

void PatlicanCpuEmu::XOR0x17()
{
	programCounter++;
	int rx = ram[programCounter];

	programCounter++;
	int number = ram[programCounter];

	regs[rx] = regs[rx] ^ number;

	ACC = regs[rx];
}

void PatlicanCpuEmu::JMP0x20()
{
	programCounter++;
	int address = ram[programCounter];
	
	programCounter = address;
}

void PatlicanCpuEmu::JZ0x21()
{
			
	if (ACC == 0)
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

void PatlicanCpuEmu::JLZ0x22()
{
	if (ACC < 0)
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

void PatlicanCpuEmu::JSC0x25()
{
	if (underflow == 1)
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

void PatlicanCpuEmu::JUC0x26()
{
	if (sumCarry == 1)
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
