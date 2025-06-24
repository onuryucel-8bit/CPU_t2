#pragma once

#include <iostream>
#include <vector>

#include "../LibsLocal/magic_enum/magic_enum.hpp"

#include "AsmLexer.h"

//TODO bazi kisimlarda regex kullanilabilir

namespace asmc
{


class Parser
{
public:
	
	Parser(asmc::Lexer* lexer);

	//parser
	void program();


private:	

	bool checkToken(asmc::TokenType type);
	bool checkPeek(asmc::TokenType type);

	void nextToken();	
	void statement();

	void printError(std::string message);

	bool expect(asmc::TokenType expectedIdent);

	bool errorFlagParser;

	asmc::Token currentToken;
	asmc::Token peekToken;

	std::vector<int> m_output;

	asmc::Lexer* m_lexer;

};

}