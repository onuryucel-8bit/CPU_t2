#include <iostream>
#include <string>
#include <cctype>

//#include "Assembler/AssemblerCPU_t2.h"

//#include "LogisimRamPrinter.h"
//#include "LibsLocal/magic_enum/magic_enum.hpp"



//-------------------parser-----------------------//

#include "Fileio/FileReader.h"

#include "AssemblerV3/AsmLexer.h"
#include "AssemblerV3/AsmParser.h"

//asmc asm compiler
void run()
{
	FileReader fr;
	std::string program = fr.read("compiler.txt");

	asmc::Lexer lexer(program);
	asmc::Parser p(&lexer);
	p.program();
}

int main()
{
	

	run();

	//asmp::AssemblerCPU_t2 assembler(file);
	//std::vector<int> output = assembler.run();
	
	

	//LogisimRamPrinter printer;
	//printer.run(output);
}


/*
<program> ::= <statement>
<statement> ::= <opcode> <register>

<opcode> ::= "LOAD" | "ADD"
<register> ::= "r" <hex1>
<hex1> ::= [0-9]

*/