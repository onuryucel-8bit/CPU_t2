
#include "Fileio/FileReader.h"

std::string program;

int position = -1;
char currentChar = 0;

void advance()
{
	position++;
	if (position >= program.length())
	{
		currentChar = EOF;
	}
	else
	{
		currentChar = program[position];
	}
}

void skipWhiteSpace()
{
	while (std::isspace(currentChar) && currentChar != EOF)
	{
		advance();
	}
}

void skipComments()
{
	if (currentChar == ';')
	{
		while (currentChar != '\n' && currentChar != EOF)
		{
			advance();
		}
	}
}

std::string getNextToken()
{
	std::string token;
	skipWhiteSpace();
	skipComments();

	while (currentChar != ',' && 
		currentChar != ' '  && 
		currentChar != '\t' && 
		currentChar != '\n' && 
		currentChar != EOF)
	{
		token += currentChar;
		advance();
	}
	advance();
	return token;
}

int main()
{
	std::string inputPath = "program.txt";
	FileReader fr;
	program = fr.read(inputPath.c_str());

	std::string line;
	
	advance();
	while (currentChar != EOF)
	{
		line = getNextToken();
		std::cout << line <<"\n";

		switch (line)
		{
		default:
			break;
		}
	}
}