#pragma once

#include <string>
#include <iostream>//TEST

/*
*   BNFv0.1
*	<program> = <opcode>
*   <opcode> = <byte1> <byte2> ?<byte3>
* 
*   <byte1> = LOAD | SUB | STR | JMP ...
*   <byte2> = rx ?ry
*   <byte3> = rx | <sayi>
* 
*   <sayi> = <rakam>*
*   <rakam> = 1 | 2 | 3 ....
*
*/

class Lexer
{
public:
	Lexer(std::string& program);
	~Lexer();

	std::string getNextToken();
	char checkNextCharIsEqual();
	void reset();

	int getCurrentPosition();
private:
	void advance();
	void skipWhiteSpace();
	void skipComments();

	
	
	int m_position;

	char m_currentChar;
	std::string m_program;

};

