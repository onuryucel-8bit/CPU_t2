#pragma once

#include <iostream>
#include <optional>
#include <string>
#include <cctype>

#include "../LibsLocal/magic_enum/magic_enum.hpp"

namespace asmc
{

using uchar = unsigned char;

enum TokenType
{

	ENDOFLINE,
	NEWLINE,
	NUMBER,
	IDENT,
	STRING,

	//Operands
	REGISTER,
	HEXNUMBER,
	ADDRESS,

	//Opcodes
	LOAD,
	STR,
	OUT,
	MOV,

	ADD,
	SUB,
	SHL,
	SHR,

	AND,
	OR,
	XOR,
	NOT,

	JMP,
	JZ,
	JLZ,
	JGZ,
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
	int m_position;
	std::string m_program;
	char m_currentChar;
	bool f_error;

	Lexer(std::string program);

	void nextChar();


	char peek();

	//0x4f peeks over 'x' returns 4
	char peekOverX();	

	void skipWhiteSpace();

	void skipComments();

	//TODO std icinde bunun tanimi olmasi lazim onu kullan


	bool checkIfKeyword(std::string token);

	//is 0xfa
	bool isNumberHex();

	bool isOperand();

	//skip ',' '\n'
	void skipNonEssential();

	Token getToken();


private:

};

}