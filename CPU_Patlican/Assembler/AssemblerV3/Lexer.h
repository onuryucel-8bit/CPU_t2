#pragma once

#include <iostream>
#include <optional>
#include <string>
#include <cctype>
#include <cstdint>

#include "../LibsLocal/magic_enum/magic_enum.hpp"
#include "../LibsLocal/rang.hpp"

#undef OUT //isim cakismasi var windows.h la galiba?

namespace asmc
{

enum TokenType
{

	ENDOFLINE = -1,

	ORIGIN = 0x52,	
	DB,	//define byte
	NEWLINE,	

	LABEL ,
	JUMPLOC ,

	//Operands
	REGISTER,
	HEXNUMBER,
	ADDRESS,
	REGADR,

	//Opcodes
	LOAD = 0x01,
	STR = 0x03,
	MOV = 0x04,
	OUT = 0x05,

	ADD = 0x08,
	SUB = 0x09,
	SHL = 0x0a,
	SHR = 0x0b,

	AND = 0x0c,
	OR = 0x0d,
	NOT = 0x0e,
	XOR = 0x0f,

	ADC = 0x18,

	JMP = 0x20,
	JZ  = 0x21,
	JLZ = 0x22,
	JGZ = 0x23,
	JSC = 0x25,
	JUC = 0x26,
	
	EMPTY
};

struct Token
{
	std::string m_text = "";
	TokenType m_type = TokenType::EMPTY;
};

#define EMPTY_TOKEN { "", asmc::TokenType::EMPTY }

class Lexer
{
public:
	
	Lexer(std::string program);

	//sonraki karakteri isaret eder
	void nextChar();

	char peek();
	//0x4f peeks over 'x' returns 4
	char peekOverX();
	
	[[nodiscard]] Token getToken();

	bool getErrorFlag();
	

private:
	//is 0xfa
	bool isNumberHex();
	bool isOperand();

	void printError(std::string message);

	//bosluklari es gecer
	void skipWhiteSpace();
	//aciklama satiri ; sonrasindaki karakterleri es gecer
	void skipComments();
	//skip ',' '\n'
	void skipNonEssential();

	//str token enum icerisinde tanimlimi
	bool checkIfKeyword(std::string token);

	//.ORIGIN .DB
	
	[[nodiscard]] asmc::Token lexDotPart();
	//rx	
	[[nodiscard]] asmc::Token lexRegPart();
	//0xff
	[[nodiscard]] asmc::Token lexHexNumberPart();
	//+,-,@,\n,EOF.... single char
	[[nodiscard]] asmc::Token lexSingleChar();


	int m_position;
	std::string m_program;
	char m_currentChar;

	bool f_error;
	bool f_newline;
};

}