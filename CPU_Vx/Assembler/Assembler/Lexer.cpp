#include "Lexer.h"

Lexer::Lexer(std::string& program)
{
	m_program = program;

	m_currentChar = m_program[0];
	m_position = 0;
}

Lexer::~Lexer()
{

}

void Lexer::advance()
{
	m_position++;

	if (m_position >= m_program.length())
	{
		m_currentChar = EOF;
	}
	else
	{
		m_currentChar = m_program[m_position];
	}
}

void Lexer::skipWhiteSpace()
{
	while (std::isspace(m_currentChar) && m_currentChar != EOF)
	{
		advance();
	}
}

void Lexer::skipComments()
{
	if (m_currentChar == ';')
	{
		while (m_currentChar != '\n' && m_currentChar != EOF)
		{
			advance();
		}
		
		if (m_currentChar == '\n')
		{
			advance();
		}
	}
}

char Lexer::peek()
{
	char tempChar = m_currentChar;
	int tempPos = m_position;

	while (tempChar != ';' && tempChar != '\t' && tempChar != '\n' && tempChar != EOF)
	{
		if (tempChar == '=')
		{
			return tempChar;
		}

		tempPos++;
		if (tempPos >= m_program.length())
		{
			tempChar = EOF;
		}
		else
		{
			tempChar = m_program[tempPos];
		}

		
	}

	return 0;
}




std::string Lexer::getNextToken()
{
	std::string currentToken;

	//Komut aralarindaki bosluklarini ve aciklama satirlarini atlamak icin
	while (true)
	{
		skipWhiteSpace();
		skipComments();
		
		if (m_currentChar == EOF)
		{
			currentToken = "";
			return currentToken;
		}
					
		if (!std::isspace(m_currentChar) && m_currentChar != ';')
		{
			break;
		}			
	}
	

	while (m_currentChar != ',' && m_currentChar != ' ' && m_currentChar != '\t' && m_currentChar != '\n' && m_currentChar != EOF)
	{
		currentToken += m_currentChar;
		advance();
	}
	
	//, ' ' vb. seyleri es gecmesi icin advance lazim
	advance();

	return currentToken;
}