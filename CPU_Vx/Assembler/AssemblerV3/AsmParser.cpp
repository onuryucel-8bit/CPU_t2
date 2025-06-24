#include "AsmParser.h"

namespace asmc
{

Parser::Parser(asmc::Lexer* lexer)
{
	m_lexer = lexer;

	errorFlagParser = false;
	peekToken = m_lexer->getToken();
	nextToken();

}

bool Parser::checkToken(asmc::TokenType type)
{
	return type == currentToken.m_type;
}

bool Parser::checkPeek(asmc::TokenType type)
{
	return type == peekToken.m_type;
}

void Parser::nextToken()
{
	currentToken = peekToken;
	peekToken = m_lexer->getToken();//lexer
}

void Parser::program()
{
	while (!checkToken(asmc::TokenType::ENDOFLINE) && !errorFlagParser)
	{
		statement();
	}
}

void Parser::statement()
{
	switch (currentToken.m_type)
	{
	case asmc::TokenType::LOAD:
		nextToken();
		if (!expect(asmc::TokenType::REGISTER))
		{
			return;
		}

		//is LOAD rx,0xff type?
		if (checkPeek(asmc::TokenType::HEXNUMBER))
		{
			//m_output.push_back(0);

			std::cout << currentToken.m_text << "::" << peekToken.m_text << "\n";

			nextToken();//currentToken => 0xff
			nextToken();//currentToken => nextNewToken
		}
		break;

	case asmc::TokenType::ADD:
		nextToken();
		if (!checkToken(asmc::TokenType::REGISTER))
		{
			std::cout << "ERROR expected register found\n";
			errorFlagParser = true;
			return;
		}

		if (checkPeek(asmc::TokenType::REGISTER))
		{
			//m_output.push_back(0);

			std::cout << currentToken.m_text << "::" << peekToken.m_text << "\n";

			nextToken();//currentToken => 0xff
			nextToken();//currentToken => nextNewToken
		}
		break;
	default:
		break;
	}

}

void Parser::printError(std::string message)
{

}

bool Parser::expect(asmc::TokenType expectedIdent)
{
	if (!checkToken(expectedIdent))
	{
		std::cout << "ERROR expected["<< magic_enum::enum_name(expectedIdent)<<"]\n";
		errorFlagParser = true;
		return false;
	}
	
	return true;


}

}