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
	EMPTY,
	ORIGIN,
	LABEL,
	HLT,
	ADD,
	SUB,
	SHL,
	SHR,

	AND,
	OR,
	NOT,
	XOR,

	LOAD,
	STR,
	MOV,
	JGZ,
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
	int rx;
	int ry;
	int byte;
};

struct RamLayout
{
	size_t m_ramIndex;
	operandPackage m_package;
	TokenType m_type;
};

enum class SymbolTypes
{
	LABEL,
	VARIABLE
};

enum class SymbolUsageStatus
{
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

	Symbol(int lineNumber, int value, SymbolTypes type, SymbolUsageStatus status)
	{
		m_lineNumber = lineNumber;
		m_value = value;
		m_type = type;
		m_status = status;
	}
};

class AssemblerCPU_t2
{
public:
	AssemblerCPU_t2(std::string& program);
	~AssemblerCPU_t2();
	
	void run();

private:
	bool firstPass();
	void secondPass();


	void readVariable();

	void printError();
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

	std::unordered_map<std::string, OpcodeInfo> m_translateTable =
	{
		{".ORIGIN", OpcodeInfo(TokenType::ORIGIN , 1, "")},
		{"ADD" , OpcodeInfo(TokenType::ADD , 1, "08")},
		{"SUB" , OpcodeInfo(TokenType::SUB , 1, "09")},
		{"SHL" , OpcodeInfo(TokenType::SHL , 1, "0A")},
		{"SHR" , OpcodeInfo(TokenType::SHR , 1, "0B")},

		{"AND" , OpcodeInfo(TokenType::AND , 1, "0C")},
		{"OR"  , OpcodeInfo(TokenType::OR  , 1, "0D")},
		{"NOT" , OpcodeInfo(TokenType::NOT , 1, "0E")},
		{"XOR" , OpcodeInfo(TokenType::XOR , 1, "0F")},

		{"LOAD", OpcodeInfo(TokenType::LOAD, 2, "01")},
		{"STR" , OpcodeInfo(TokenType::STR , 2, "03")},
		{"MOV" , OpcodeInfo(TokenType::MOV , 1, "04")},
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