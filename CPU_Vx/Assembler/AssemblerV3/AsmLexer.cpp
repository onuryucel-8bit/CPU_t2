#include "AsmLexer.h"
namespace asmc 
{


Lexer::Lexer(std::string program)
{
	m_program = program;
	m_position = -1;
	m_currentChar = 0;
	f_error = false;
	nextChar();
}

char Lexer::peek()
{
	if (m_position + 1 >= m_program.length())
	{
		return ENDOFLINE;
	}
	return m_program[m_position + 1];
}

bool Lexer::checkIfKeyword(std::string token)
{
	std::optional<TokenType> colorName = magic_enum::enum_cast<TokenType>(token);
	if (colorName.has_value())
	{
		//std::cout << colorName.value() << "\n";
		return true;
	}

	return false;
}

//is 0xfa
bool Lexer::isNumberHex()
{
	if (m_currentChar == '0' && peek() == 'x' && std::isxdigit(static_cast<uchar>(peekOverX())))
	{
		return true;
	}

	return false;
}

bool Lexer::isOperand()
{
	//isxdigit() checks next char is it hex?
	//register ?
	if (m_currentChar == 'r' && std::isxdigit(static_cast<uchar>(peek())))
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

//skip ',' '\n'
void Lexer::skipNonEssential()
{
	if (m_currentChar == ',' || m_currentChar == '\n')
	{
		nextChar();
	}
}

Token Lexer::getToken()
{
	skipWhiteSpace();
	skipComments();
	skipNonEssential();

	Token token;


	//ident,keyword or operand check
	if (std::isalpha(m_currentChar))
	{
		if (isOperand())
		{
			nextChar();
			if (isxdigit(static_cast<uchar>(peek())))
			{
				std::cout << "ERROR invalid reg operand it should be 4bit\n";
				return token;
			}
			token = { std::string(1,m_currentChar), TokenType::REGISTER };
		}
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
				token = { tokenStr, TokenType::IDENT };
			}
		}
	}
	//number check
	else if (std::isdigit(m_currentChar))
	{

		if (isOperand())
		{
			nextChar();//skip 0
			nextChar();//skip x

			if (!isxdigit(static_cast<uchar>(peek())))
			{
				std::cout << "ERROR invalid number operand it should be 4bit\n";
				return token;
			}

			//get next hex number part
			std::string number(1, m_currentChar);
			nextChar();
			number += m_currentChar;

			token = { number, TokenType::HEXNUMBER };
		}
		else
		{
			int startPos = m_position;
			int length = 1;
			while (std::isdigit(peek()))
			{
				nextChar();
				length++;
			}

			std::string tokenStr = m_program.substr(startPos, length);
			token = { tokenStr, TokenType::NUMBER };
		}

	}
	else
	{
		switch (m_currentChar)
		{
		case '+':
			token = { std::string(1,m_currentChar), TokenType::PLUS };
			break;
		case '-':
			token = { std::string(1,m_currentChar), TokenType::MINUS };
			break;
		case '/':
			token = { std::string(1,m_currentChar), TokenType::SLASH };
			break;
		case '*':
			token = { std::string(1,m_currentChar), TokenType::ASTERISK };
			break;
		case '\n':
			//token = { std::string(1,m_currentChar), TokenType::NEWLINE };
			break;
		case ENDOFLINE:
			token = { std::string(1,m_currentChar), TokenType::ENDOFLINE };
			break;
		default:
			f_error = true;
			std::cout << "LEXER Error current char =" << m_currentChar << "\n";
			break;
		}
	}


	nextChar();

	return token;
}

void Lexer::skipComments()
{
	if (m_currentChar == ';')
	{
		while (m_currentChar != '\n' && m_currentChar != ENDOFLINE)
		{
			nextChar();
		}
	}
}

void Lexer::skipWhiteSpace()
{
	while (m_currentChar == ' ' || m_currentChar == '\t')
	{
		nextChar();
	}
}

char Lexer::peekOverX()
{
	if (m_position + 2 >= m_program.length())
	{
		return ENDOFLINE;
	}
	return m_program[m_position + 2];
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

}