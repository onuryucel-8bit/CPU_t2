#pragma once

#include <string>
#include <iostream>//TEST

class Lexer
{
public:
	Lexer(std::string& program);
	~Lexer();

	std::string getNextToken();
	char peek();
	void reset();

	int getCurrentPosition();
private:
	void advance();
	void skipWhiteSpace();
	void skipComments();

	
	
	int m_position;

	char m_currentChar;
	std::string m_program;

};

