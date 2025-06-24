#pragma once

#include <iostream>
#include <optional>
#include <string>
#include <cctype>

#include "../LibsLocal/magic_enum/magic_enum.hpp"
#include "../LibsLocal/rang.hpp"

namespace asmc
{

//TODO std icinde bunun tanimi olmasi lazim onu kullan
using uchar = unsigned char;

enum TokenType
{

	ENDOFLINE,
	NEWLINE,

	NUMBER,
	//IDENT,
	//STRING,

	LABEL,
	JUMPLOC,

	//Operands
	REGISTER,
	HEXNUMBER,
	ADDRESS,

	//Opcodes
	LOAD,
	 LOADadr,
	STR,
	asmcOUT,
	MOV,

	ADD,
	SUB = 0x11,
	SHL,
	SHR,

	AND,
	OR,
	XOR,
	NOT,

	JMP,
	JZ,
	JLZ,
	JGZ = 0x23,
	JSC,
	JUC,

	//math for macro and variables
	PLUS,
	MINUS,
	SLASH,
	ASTERISK,



	EMPTY
};

struct Token
{
	std::string m_text = "";
	TokenType m_type = TokenType::EMPTY;
};

class Lexer
{
public:
	

	Lexer(std::string program);

	void nextChar();

	char peek();
	//0x4f peeks over 'x' returns 4
	char peekOverX();	
	
	Token getToken();

	bool getErrorFlag();
	

private:
	//is 0xfa
	bool isNumberHex();
	bool isOperand();

	void printError(std::string message);

	void skipWhiteSpace();
	void skipComments();
	//skip ',' '\n'
	void skipNonEssential();

	
	bool checkIfKeyword(std::string token);

	int m_position;
	std::string m_program;
	char m_currentChar;

	bool f_error;
	bool f_newline;
};

}