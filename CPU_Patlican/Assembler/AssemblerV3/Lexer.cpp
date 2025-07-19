#include "Lexer.h"
namespace asmc 
{

Lexer::Lexer(std::string program)
{
	m_program = program;
	m_position = -1;
	m_currentChar = 0;

	f_error = false;
	f_newline = true;

	nextChar();
}

Token Lexer::getToken()
{
	skipWhiteSpace();
	skipComments();
	skipNonEssential();
	
	Token token;

	//.origin .db
	if (m_currentChar == '.')
	{
		token = lexDotPart();

		f_newline = false;
	}
	//Register,[keyword,label,jumploc] check
	else if (std::isalpha(m_currentChar))
	{
		//Register?
		if (isOperand())
		{
			token = lexRegPart();
		}
		//keyword,label,jumploc
		else
		{
			int startPos = m_position;
			int length = 1;
			while (std::isalpha(peek()))
			{
				nextChar();
				length++;
			}

			std::string tokenStr = m_program.substr(startPos, length);

			if (checkIfKeyword(tokenStr))
			{
				std::optional<TokenType> enumVal = magic_enum::enum_cast<TokenType>(tokenStr);
				token = { tokenStr, enumVal.value() };
			}
			else
			{
				//label
				if (f_newline == true && peek() != ':')
				{
					printError("LEXER:: label must end with ':' ");
					f_error = true;
				}
				else if (peek() == ':')
				{
					token = { tokenStr, TokenType::LABEL };
					nextChar();
					
				}
				//variable
				else
				{
					token = { tokenStr, TokenType::JUMPLOC };
					//nextChar();
					
				}
				
			}
		}
		f_newline = false;
	}
	//number check
	else if (std::isdigit(m_currentChar))
	{
		//HEX NUMBER
		if (isOperand())
		{			
			token = lexHexNumberPart();
		}			
		else
		{			
			f_error = true;

		}
		f_newline = false;

	}
	else
	{
		token = lexSingleChar();
		f_newline = false;
	}

	nextChar();

	return token;
}

char Lexer::peek()
{
	if (m_position + 1 >= m_program.length())
	{
		return ENDOFLINE;
	}
	return m_program[m_position + 1];
}

char Lexer::peekOverX()
{
	if (m_position + 2 >= m_program.length())
	{
		return ENDOFLINE;
	}
	return m_program[m_position + 2];
}

//--------------------------------------------//
//--------------------------------------------//
//--------------------------------------------//

bool Lexer::checkIfKeyword(std::string token)
{
	std::optional<TokenType> tempToken = magic_enum::enum_cast<TokenType>(token);
	if (tempToken.has_value())
	{		
		return true;
	}

	return false;
}

asmc::Token Lexer::lexDotPart()
{
	//get token str
	int startPos = m_position + 1;
	int length = 0;
	while (std::isalpha(peek()))
	{
		nextChar();
		length++;
	}

	std::string tokenStr = m_program.substr(startPos, length);

	if (checkIfKeyword(tokenStr))
	{
		//returns ORIGIN or DB token
		std::optional<TokenType> enumVal = magic_enum::enum_cast<TokenType>(tokenStr);
		return { tokenStr, enumVal.value() };
	}
	
	f_error = true;

	return EMPTY_TOKEN;
}

asmc::Token Lexer::lexRegPart()
{	
	nextChar();
	if (isxdigit(static_cast<uint8_t>(peek())))
	{
		std::cout << "ERROR invalid reg operand it should be r[0-7]\n";
		return EMPTY_TOKEN;
	}

	
	return { std::string(1,m_currentChar), TokenType::REGISTER };
}

asmc::Token Lexer::lexHexNumberPart()
{
	nextChar();//skip 0
	nextChar();//skip x

	if (!isxdigit(static_cast<uint8_t>(peek())))
	{
		std::cout << "ERROR invalid number operand it should be 4bit\n";
		return EMPTY_TOKEN;
	}

	//get next hex number part
	std::string number(1, m_currentChar);
	nextChar();
	number += m_currentChar;

	return {number, asmc::TokenType::HEXNUMBER};
}

asmc::Token Lexer::lexSingleChar()
{
	asmc::Token token;

	int startPos;
	int length;
	std::string tokenStr;

	switch (m_currentChar)
	{
	
	case '\n':
		//std::cout << "LEXER newline detected\n";
		token = { std::string(1,m_currentChar), TokenType::NEWLINE };
		f_newline = true;		
		break;

		//ADDRESS
	case '@':
		//OUT rx, OUT @fa, OUT @rx

		nextChar();//move cursor to number OR r

		//OUT @rx
		if (m_currentChar == 'r')
		{
			nextChar();
			token = { std::string(1, m_currentChar), asmc::TokenType::REGADR };
		}
		else
		{
			startPos = m_position;
			length = 1;
			while (std::isxdigit(peek()))
			{
				nextChar();
				length++;
			}

			tokenStr = m_program.substr(startPos, length);
			token = { tokenStr, asmc::TokenType::ADDRESS };
		}



		break;

	case ENDOFLINE:
		token = { std::string(1,m_currentChar), TokenType::ENDOFLINE };
		break;
	default:
		printError("LEXER Error current char = [" + std::to_string(m_currentChar) + "] current pos[" + std::to_string(m_position) + "]");
		f_error = true;
		return EMPTY_TOKEN;	
	}

	return token;
}

//is 0xfa
bool Lexer::isNumberHex()
{
	if (m_currentChar == '0' && peek() == 'x' && std::isxdigit(static_cast<uint8_t>(peekOverX())))
	{
		return true;
	}

	return false;
}

bool Lexer::isOperand()
{
	//isxdigit() checks next char is it hex?
	//register ?
	if (m_currentChar == 'r' && std::isdigit(static_cast<uint8_t>(peek())))
	{
		return true;
	}
	//is 0xfa
	else if (isNumberHex())
	{
		return true;
	}

	return false;
}

//--------------------------------------------//
//--------------------------------------------//
//--------------------------------------------//
void Lexer::skipComments()
{
	if (m_currentChar == ';')
	{
		while (m_currentChar != '\n' && m_currentChar != ENDOFLINE)
		{
			nextChar();
		}
		f_newline = true;
	}
}

void Lexer::skipWhiteSpace()
{
	while (m_currentChar == ' ' || m_currentChar == '\t')
	{
		nextChar();
	}
}

//skip ',' '\n'
void Lexer::skipNonEssential()
{
	while (m_currentChar == ',' || m_currentChar == '\t')
	{
		if (m_currentChar == '\n')
		{
			f_newline = true;
		}

		nextChar();
	}
}

void Lexer::nextChar()
{
	m_position++;
	if (m_position >= m_program.length())
	{
		m_currentChar = ENDOFLINE;
	}
	else
	{
		m_currentChar = m_program[m_position];
	}
}

//--------------------------------------------//
//--------------------------------------------//
//--------------------------------------------//

void Lexer::printError(std::string message)
{
	std::cout << rang::fg::red
		<< "ERROR::" << message
		//<< " line number = " << m_lineNumber
		//<< " next and peek Token [" << m_currentToken.m_text << " " << m_peekToken.m_text
		<< rang::style::reset << "\n";
}

bool Lexer::getErrorFlag()
{
	return f_error;
}

}