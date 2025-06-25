#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>

#include "../LibsLocal/magic_enum/magic_enum.hpp"
#include "../LibsLocal/rang.hpp"

#include "../utils/Radix.h"

#include "Lexer.h"

//TODO bazi kisimlarda regex kullanilabilir

namespace asmc
{

enum class LabelStatus
{
	/*
	
	//USED
		LOOP:
		JGZ LOOP 

	//NOT_USED(warning)
		START: //not used
		LOOP:
		JGZ LOOP

		JGZ END => jumpTable
		END //not used

	//UNDEFINED(error)
		LOOP:
		JGZ END_MOD//UNDEFINED LABEL

	//Multiple defined labels gives error at first pass
	//no need to check them at second pass
	*/

	NotUsed,
	Used,
	Undefined
};


struct SymbolInfo
{
	int m_ramIndex = -1;
	LabelStatus m_status = LabelStatus::NotUsed;
};


struct MemoryLayout
{
	int m_ramIndex = 0;

	int m_opcode = 0;
	int m_firstByte = 0;
	int m_secondByte = 0;
	int m_byteAmount = 0;
};

struct OpcodePair
{
	int regRegister;//ADD rx,ry
	int regImmediate;//ADD rx,0xfa
};

class Parser
{
public:
	
	Parser(asmc::Lexer* lexer);

	//parser
	void program();
	
	std::vector<int> getBinaryData();

	bool checkError();

private:	

	void secondPass();

	bool checkToken(asmc::TokenType type);
	bool checkPeek(asmc::TokenType type);

	void nextToken();	
	void statement();

	void generateBinaryArr();


	void DEBUG_printMessage(std::string message);

	
	void printWarning(std::string message);
	void printError(std::string message);
	void printOutput();

	void createMemoryLayout(int byteAmount, int opcode);
	bool expect(asmc::Token token, asmc::TokenType expectedIdent);


	bool f_errorParser;
	int m_lineNumber;

	int m_ramLocation;

	asmc::Token m_currentToken;
	asmc::Token m_peekToken;

	std::unordered_map<asmc::TokenType, OpcodePair> m_variantTable;
	std::unordered_map<std::string, SymbolInfo> m_symbolTable;
	std::unordered_map<std::string, MemoryLayout> m_jumpTable;

	std::vector<MemoryLayout> m_output;
	std::vector<int> m_binaryProgram;

	asmc::Lexer* m_lexer;

	void parseALUcommands();
	void parseJumpCommands();

	void parseOUT();
	void parseMOV();
	void parseLOAD();
	void parseADD();
	void parseSUB();
	void parseJGZ();
	void parseSTR();
	void parseNOT();
};

}