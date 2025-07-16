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

#include <fstream>

#include "PatlicanCpuEmu.h"

uint8_t ram[256];

bool readFile(const std::string path)
{
	std::ifstream file(path, std::ios::binary);

	if (!file)
	{
		std::cerr << "ERROR :: readFile() path is invalid\n";
		return false;
	}

	file.read((char*)ram, 256);
	
	file.close();

	return true;
}

int main(int argc, char* argv[])
{
	std::string inputFile;
	
	if (argc == 1)
	{
		std::cout << "Running with default bin folder name[program.bin]... \n";
		inputFile = "program.bin";
		
	}

	if (argc >= 2)
	{
		inputFile = argv[1];
	}


	if (!readFile(inputFile))
	{
		return -1;
	}

	PatlicanCpuEmu emu(ram, 256);
	emu.run();

	return 0;
}