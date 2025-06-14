#pragma once


#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "../utils/Radix.h"
#include "Lexer.h"

#include "../LibsLocal/rang.hpp"

namespace asmp 
{



enum TokenType
{
	EMPTY = -3,
	ORIGIN = -2,
	LABEL = -1,

	HLT = 0x0,

	LOAD = 0x01,
	LOADs = 0x02,
	STR = 0x03,
	MOV = 0x04,

	OUTr = 0x05,
	OUTa = 0x06,


	ADD = 0x8,
	SUB = 0x9,
	SHL = 0xa,
	SHR = 0xb,

	AND = 0x0c,
	OR  = 0x0d,
	NOT = 0x0e,
	XOR = 0x0f,

	ADDs = 0x10,
	SUBs = 0x11,
	ANDs = 0x14,
	ORs = 0x15,
	XORs = 0x17,
	
	JMP = 0x20,
	JZ = 0x21,
	JLZ = 0x22,
	JGZ = 0x23,

	JSC = 0x25,
	JUC = 0x26
};

struct OpcodeInfo
{
	TokenType m_type;
	int m_byteAmount;
	std::string m_hexValue;

	OpcodeInfo()
	{
		m_byteAmount = 0;
		m_type = EMPTY;
		m_hexValue = "";
	}

	OpcodeInfo(TokenType type, int byteAmount, std::string hexValue)
	{
		m_type = type;
		m_byteAmount = byteAmount;
		m_hexValue = hexValue;
	}
};

struct operandPackage
{
	int partx = -1;
	int party = -1;	
};

struct RamLayout
{
	size_t m_ramIndex = 0;
	asmp::operandPackage m_package;
	asmp::TokenType m_type = asmp::TokenType::EMPTY;
};

enum class SymbolTypes
{
	LABEL,   //LOOP:
	VARIABLE,//.var 0x4
	MACRO    //i = 0x5
};

enum class SymbolUsageStatus
{
	/*
	
	START: //not_used
	ADD	r1,r2
	JGZ LOOP: //undefined

	NUMBER = 2
	LOAD r2,NUMBER // defined

	*/
	UNDEFINED,   //used but not defined in program(forward declared label/var)
	DEFINED,     //defined and used in program
	NOT_USED,    //not used in program
};

struct Symbol
{
	int m_lineNumber;
	//For variable type holding the hex value
	int m_value;
	
	SymbolTypes m_type;
	SymbolUsageStatus m_status;

	/*
	Symbol(int lineNumber, int value, SymbolTypes type, SymbolUsageStatus status)
	{
		m_lineNumber = lineNumber;
		m_value = value;
		m_type = type;
		m_status = status;
	}
	*/
};

class AssemblerCPU_t2
{
public:
	AssemblerCPU_t2(std::string& program);
	~AssemblerCPU_t2();
	
	std::vector<RamLayout> run();

private:
	void firstPass();
	void secondPass();

#ifdef _DEBUG
	void printOutput();
#endif // _DEBUG

	std::string toString(asmp::TokenType type);
	std::string toString(asmp::SymbolUsageStatus status);
	std::string toString(asmp::SymbolTypes type);
	
	void readVariable();
	
	//returns error line
	std::string getErrorLine();

	void printWarning(std::string message);
	void printError(std::string message);

	bool removeRRegisterPrefix(std::string& operand);
	bool removeHexPrefix(std::string& operand);
	

	Lexer lexer;

	bool m_error;
	
	size_t m_lineNumber;
	int m_currentRamIndex;
	
	std::string m_program;
	std::string m_currentToken;

	std::vector<RamLayout> m_output;

	//symbol name => symbol line number and type
	//LOOP => (4, LABEL)
	std::unordered_map<std::string, Symbol> m_symbolTable;

	std::unordered_map<std::string, OpcodeInfo> m_commandInfoTable =
	{
		{".ORIGIN", OpcodeInfo(TokenType::ORIGIN , 1, "")},
		{"ADD" , OpcodeInfo(TokenType::ADD , 2, "08")},
		{"SUB" , OpcodeInfo(TokenType::SUB , 2, "09")},
		{"SHL" , OpcodeInfo(TokenType::SHL , 1, "0A")},
		{"SHR" , OpcodeInfo(TokenType::SHR , 1, "0B")},

		{"AND" , OpcodeInfo(TokenType::AND , 1, "0C")},
		{"OR"  , OpcodeInfo(TokenType::OR  , 1, "0D")},
		{"NOT" , OpcodeInfo(TokenType::NOT , 1, "0E")},
		{"XOR" , OpcodeInfo(TokenType::XOR , 1, "0F")},

		{"LOAD", OpcodeInfo(TokenType::LOAD, 2, "01")},
		{"STR" , OpcodeInfo(TokenType::STR , 2, "03")},
		{"MOV" , OpcodeInfo(TokenType::MOV , 1, "04")},
		{"JMP" , OpcodeInfo(TokenType::JMP , 0, "FF")},
		{"JGZ" , OpcodeInfo(TokenType::JGZ , 2, "05")},
	};

#ifdef _DEBUG
	std::unordered_map<SymbolTypes, std::string> m_symbolNameTable =
	{
		{SymbolTypes::LABEL, "LABEL"},
		{SymbolTypes::VARIABLE, "VARIABLE"}
	};
#endif // _DEBUG


};

}