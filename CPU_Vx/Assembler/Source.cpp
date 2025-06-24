#include <iostream>
#include <string>
#include <cctype>

//#include "Assembler/AssemblerCPU_t2.h"

//#include "LogisimRamPrinter.h"
//#include "LibsLocal/magic_enum/magic_enum.hpp"



//-------------------parser-----------------------//

#include "Fileio/FileReader.h"

#include "AssemblerV3/Lexer.h"
#include "AssemblerV3/Parser.h"

#include "LogisimRamPrinter.h"

//asmc asm compiler
void run()
{
	FileReader fr;
	std::string program = fr.read("program.txt");

	asmc::Lexer lexer(program);
	asmc::Parser p(&lexer);
	p.program();


	if (p.checkError())
	{
		return;
	}
	std::cout << std::dec << rang::bg::blue << "Sending the data to logisim printer..." << rang::style::reset << "\n";

	LogisimRamPrinter lrp;
	std::vector<int> a = p.getBinaryData();
	lrp.run(a);

}

int main()
{
	
	run();

	//asmp::AssemblerCPU_t2 assembler(file);
	//std::vector<int> output = assembler.run();
	
	//LogisimRamPrinter printer;
	//printer.run(output);
}

//tekrar eden fonksiyonlari birlestir
//hex sayilari kombinle
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

