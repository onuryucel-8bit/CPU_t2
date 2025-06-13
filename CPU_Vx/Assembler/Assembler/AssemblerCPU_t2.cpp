#include "AssemblerCPU_t2.h"

namespace asmp
{

AssemblerCPU_t2::AssemblerCPU_t2(std::string& program)
	:lexer(program)
{

	m_currentRamIndex = 0;
	m_lineNumber = 1;
	m_error = false;

	m_program = program;
	
}

AssemblerCPU_t2::~AssemblerCPU_t2()
{

}

//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//

void AssemblerCPU_t2::printError(std::string message)
{
	std::cout << "ERROR:: " << message << " \nline number = "<< m_lineNumber << "\n";
}

void AssemblerCPU_t2::printError()
{
	std::cout << "ERROR:: invalid token currentToken = [" << m_currentToken << "] line number = " << m_lineNumber << "\n";
}

//------------------------------------------------------------------------------------//

void AssemblerCPU_t2::readVariable()
{
	//currentType = symbol
	std::cout << "VAR " << m_currentToken << "\n";

	std::string varName = m_currentToken;

	lexer.getNextToken();//skip '='
	std::string number = lexer.getNextToken();//get hex part

	//valid hex syntax?
	if (!removeHexPrefix(number))
	{
		m_error = true;
	}

	//str(hex) => int(hex)
	int decNumber = rdx::hexToDeC(number);

	//Ayni isim kullanilmissa hata ver
	//check is label duplicate 
	if (m_symbolTable.contains(m_currentToken))
	{
		m_error = true;
		printError("Duplicate label [" + m_currentToken + "] line number = [" + std::to_string(m_lineNumber) + "]");
	}
	else
	{
		m_symbolTable[m_currentToken] = { m_currentRamIndex, decNumber, SymbolTypes::VARIABLE, SymbolUsageStatus::UNDEFINED };
		//m_currentRamIndex--;
	}

}

bool AssemblerCPU_t2::removeHexPrefix(std::string& operand)
{
	if (operand[0] == '0' && operand[1] == 'x')
	{
		operand.erase(0, 2);
		return true;
	}
	printError("removeHexPrefix():: invalid hex format currentToken[" + m_currentToken + "] currentOperand[" + operand + "]");
	return false;
}

bool AssemblerCPU_t2::removeRRegisterPrefix(std::string& operand)
{
	if (operand[0] == 'r' || operand[0] == 'R')
	{
		operand.erase(operand.begin());
		return true;
	}
	printError("removeRRegisterPrefix():: invalid register format currentToken[" + m_currentToken + "] currentOperand[" + operand +"]");
	return false;
	
}

//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//


void AssemblerCPU_t2::run()
{
	if (!firstPass())
	{
		return;
	}
	secondPass();
}

bool AssemblerCPU_t2::firstPass()
{
	while (m_error == false)
	{
		RamLayout memLayout;
		std::string rx, ry, adres, sayi;
		int package = 0;

		m_currentToken = lexer.getNextToken();

		//end of file
		if (m_currentToken.empty())
		{
			break;
		}

		TokenType currentType = m_translateTable[m_currentToken].m_type;
		//std::cout << m_currentToken << "\n";

		//Eger currentToken etiket/label ise
		if (m_currentToken[m_currentToken.length() - 1] == ':')
		{
			//currentType = LABEL;
			m_currentToken.erase(m_currentToken.length() - 1);//remove : part

			//Ayni isim kullanilmissa hata ver
			if (m_symbolTable.contains(m_currentToken))
			{
				m_error = true;
				printError("Duplicate label [" + m_currentToken + "] line number = [" + std::to_string(m_lineNumber) + "]");
			}
			else
			{
				m_symbolTable[m_currentToken] = { m_currentRamIndex, 0, SymbolTypes::LABEL };
				//m_currentRamIndex--;
			}
			
			
		}
		//Degisken tanimi
		else if (lexer.peek() == '=')
		{
			readVariable();
		}
		else
		{
			switch (currentType)
			{

			case ORIGIN:
				sayi = lexer.getNextToken();
				//get adr
				if (!removeHexPrefix(sayi))
				{
					m_error = true;
					break;
				}
				m_currentRamIndex = std::stoi(rx);
				break;

			case JGZ:
				adres = lexer.getNextToken();
				
				if (!m_symbolTable.contains(adres))
				{
					m_symbolTable[adres] = {-1, 0, SymbolTypes::LABEL };
				}
				break;
			
			case LABEL:
				break;

			default:
				//m_error = true;
				//printError("default error");
				break;
			}
			m_currentRamIndex++;
		}//else END

		m_lineNumber++;
	}//while END

	
/*
---------------------------------------------------------------
===============================================================
---------------------------------------------------------------
*/

#ifdef _DEBUG
	std::cout << "--------second pass--------\n";
	std::cout << "Printing symbol table...\n";
#endif // _DEBUG

	//cpp17 !!
	for (const auto& [key, value] : m_symbolTable) 
	{
		

#ifdef _DEBUG
		std::cout << "symbol name: " << key << " line number: " << value.m_lineNumber <<" symbol type : " << m_symbolNameTable[value.m_type] << "\n";
#endif // _DEBUG

		if (value.m_lineNumber == -1)
		{
			std::cout << rang::fg::red << "ERROR::firstPass():: Label is not used in program symbol name = " << key << rang::style::reset << "\n";
			return false;
		}
	}
/*
---------------------------------------------------------------
===============================================================
---------------------------------------------------------------
*/
	return true;

	//while (m_error == false)
	//{
	//	RamLayout memLayout;
	//	std::string rx,ry,adres,sayi;
	//	int package = 0;

	//	m_currentToken =  lexer.getNextToken();
	//	if (m_currentToken.empty())
	//	{
	//		break;
	//	}


	//	TokenType currentType = m_translateTable[m_currentToken].m_type;
	//	
	//	//Eger currentToken etiket/label ise
	//	if (m_currentToken[m_currentToken.length() - 1] == ':')
	//	{
	//		currentType = LABEL;
	//		m_currentToken.erase(m_currentToken.length() - 1);//remove : part

	//		m_symbolTable[m_currentToken] = { m_lineNumber, SymbolTypes::LABEL };
	//		m_lineNumber--;
	//	}

	//	memLayout.m_ramIndex = m_currentRamIndex;
	//	memLayout.m_type = currentType;

	//	switch (currentType)
	//	{
	//	case ORIGIN:
	//		break;

	//	//Gecersiz yazim sekli buraya dusmekte
	//	case EMPTY:
	//		printError();
	//		m_error = true;
	//		break;

	//	case LABEL:
	//		std::cout << "Label = " << m_currentToken << " linenumber = " << m_lineNumber << "\n";
	//		break;

	//	case HLT:
	//		break;

	//	case LOAD:

	//		rx = lexer.getNextToken();
	//		sayi = lexer.getNextToken();

	//		if (!removeRRegisterPrefix(rx) || !removeHexPrefix(sayi))
	//		{
	//			m_error = true;
	//			break;
	//		}

	//		memLayout.m_package.rx = std::stoi(rx);
	//		memLayout.m_package.byte = std::stoi(sayi);
	//		break;

	//	case ADD:

	//		rx = lexer.getNextToken();
	//		ry = lexer.getNextToken();

	//		if (!removeRRegisterPrefix(rx) || !removeRRegisterPrefix(ry))
	//		{
	//			m_error = true;
	//			break;
	//		}

	//		memLayout.m_package.rx = std::stoi(rx);
	//		memLayout.m_package.ry = std::stoi(ry);
	//		break;

	//	case SUB:

	//		rx = lexer.getNextToken();
	//		sayi = lexer.getNextToken();

	//		if (!removeRRegisterPrefix(rx) || !removeHexPrefix(sayi))
	//		{
	//			m_error = true;
	//			break;
	//		}

	//		memLayout.m_package.rx = std::stoi(rx);
	//		memLayout.m_package.byte = std::stoi(sayi);
	//		break;

	//	case SHL:
	//		break;
	//	case SHR:
	//		break;
	//	case AND:
	//		break;
	//	case OR:
	//		break;
	//	case NOT:
	//		break;
	//	case XOR:
	//		break;
	//	
	//	case STR:
	//		break;
	//	case MOV:
	//		break;

	//	case JGZ:

	//		//adres / label
	//		sayi = lexer.getNextToken();
	//		 
	//		//check symbol table if label is in
	//		if (m_symbolTable.contains(sayi))
	//		{
	//			//table[label_name] => 5(adres)
	//			rx = std::to_string(m_symbolTable[sayi].m_lineNumber);
	//		}
	//		memLayout.m_package.byte = std::stoi(rx);
	//		break;
	//	}
	//	m_output.push_back(memLayout);
	//	m_lineNumber++;
	//}
}

void AssemblerCPU_t2::secondPass()
{

}
}