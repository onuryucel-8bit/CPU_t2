#include <iostream>

//-------------------parser-----------------------//

#include "Fileio/FileReader.h"

#include "AssemblerV3/Lexer.h"
#include "AssemblerV3/Parser.h"

#include "Printers/LogisimRamPrinter.h"
#include "Printers/BinaryRamPrinter.h"

//asmc asm compiler
int main(int argc, char* argv[])
{
	std::string inputFile = "program.txt";
	std::string outputFile = "output";

	if (argc >= 2) 
	{
		inputFile = argv[1];
	}
	if (argc >= 3) 
	{
		outputFile = argv[2];
	}

	std::cout << "Running assembler cpu_t2....\n";

	FileReader fr;
	std::string program = fr.read(inputFile.c_str());

	asmc::Lexer lexer(program);
	asmc::Parser p(&lexer);
	p.program();


	if (p.checkError())
	{
		return -1;
	}
	std::cout << std::dec << rang::bg::blue << "Sending the data to logisim printer..." << rang::style::reset << "\n";

	LogisimRamPrinter lrp;
	std::vector<uint8_t> a = p.getBinaryData();
	lrp.run(outputFile + ".txt", a);

	BinaryRamPrinter brp;

	
	brp.run(outputFile + ".bin", a);
}

//TODO .origin .db ve #define anahtarlarini ekle

//HATALAR
//ram adres sapmasi olursa gidip ramLocation i parseX altinda arttirmis olmalisin

/*

SHR
SHL
XOR
OR

AND
NOT
ADD
SUB

STR
LOAD
MOV
SUB
JGZ-jumps


*/


//TODO hata sistemini iyilestir
//TODO m_lineNumber nokta atisi hatanin oldugu satiri gostersin


/*V0.2
<program> ::= <statement>
<statement> ::= <opcode> ((<register> | <address>) | (<register> | <address> | <hex>)?)
<opcode> ::= "LOAD" | "ADD" | "STR" | "JMP" | ....
<hex> ::= 0x[0-f]+
<address> ::= @[0-f]+
<register> ::= r[0-f]
*/


/*V0.1
<program> ::= <statement>
<statement> ::= <opcode> <register>

<opcode> ::= "LOAD" | "ADD"
<register> ::= "r" <hex1>
<hex1> ::= [0-9]
*/


/*


!hatali yazim
LOOP:
START

!LEXER HATASI hatali yazim sekli ':' yok 
start
JGZ start

#uyari verir(kullanilmayan ziplama adresi)
START:
LOOP:
JGZ LOOP

!Hata verir (tanimsiz ziplama adresi) LabelStatus::Undefined
JGZ LOOP

(birden cok ayni isimde zimplama adresi tanimlanmasi)
!Hata verir
LOOP:
JGZ LOOP
LOOP:

!Hata verir
LOOP:
LOOP:
JGZ LOOP

*/

