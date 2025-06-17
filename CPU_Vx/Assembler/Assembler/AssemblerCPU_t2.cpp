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

std::string AssemblerCPU_t2::getErrorLine()
{
	int pos = lexer.getCurrentPosition();

	//get next token
	std::string errorLine = lexer.getNextToken();
	
	//POT_ERROR
	//hata yaziminda hata cikarsa pos - 2 den dolayi olabilir

	//get previous token
	pos--;
	char currentChar = m_program[pos];
	
	//FIXME pos hata var
	std::string tempLine;
	char emptySpaceCounter = 0;
	while (currentChar != '\n' && currentChar != '\t' && pos > -1)
	{
		currentChar = m_program[pos];
		tempLine += currentChar;
		if (currentChar == ' ')
		{
			emptySpaceCounter++;
		}

		if (emptySpaceCounter == 5)
		{
			break;
		}

		pos--;		
	}

	std::reverse(tempLine.begin(), tempLine.end());

	errorLine = tempLine + errorLine;

	return errorLine;
}

void AssemblerCPU_t2::printError(std::string message)
{
	std::cout << rang::fg::red << "ERROR:: " << message << " error line = [" << getErrorLine() <<"]"<< rang::style::reset << "\n";
}

void AssemblerCPU_t2::printWarning(std::string message)
{
	std::cout << rang::fg::yellow << "WARNING::" << message << rang::style::reset << "\n";
}

//------------------------------------------------------------------------------------//
//================================UTILS===============================================//
//------------------------------------------------------------------------------------//


std::string AssemblerCPU_t2::toString(asmp::TokenType type)
{
	switch (type)
	{
	case asmp::EMPTY:
		break;
	case asmp::ORIGIN:
		break;
	case asmp::LABEL:
		break;
	case asmp::HLT:
		break;
	case asmp::ADD: return "ADD";

	case asmp::SUBs: return "SUBs";
	case asmp::SUB: return "SUB";
	
	case asmp::SHL:
		break;
	case asmp::SHR:
		break;
	case asmp::AND:
		break;
	case asmp::OR:
		break;
	case asmp::NOT:
		break;
	case asmp::XOR:
		break;
	case asmp::LOAD: return "LOAD";
	case asmp::STR:
		break;
	case asmp::MOV:
		break;
	case asmp::JGZ: return "JGZ";
	case asmp::JMP:
		break;
	default: return "?default toString(TokenType)";
	}
}

std::string AssemblerCPU_t2::toString(asmp::SymbolUsageStatus status)
{	
	switch (status)
	{
	case SymbolUsageStatus::DEFINED:
		return "DEFINED";
			
	case SymbolUsageStatus::UNDEFINED:
		return "UNDEFINED";
		
	case SymbolUsageStatus::NOT_USED:
		return "NOT_USED";
	}

	
}

std::string AssemblerCPU_t2::toString(asmp::SymbolTypes type)
{
	switch (type)
	{
	case SymbolTypes::LABEL:
		return "LABEL";

	case SymbolTypes::MACRO:
		return "MACRO";

	case SymbolTypes::VARIABLE:
		return "VARIABLE";
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

bool AssemblerCPU_t2::isRegister(std::string& operand)
{
	return operand.length() == 2 && std::isdigit(operand[1]) && (operand[0] == 'r' || operand[0] == 'R');
}

bool AssemblerCPU_t2::removeRRegisterPrefix(std::string& operand)
{

	if (isRegister(operand))
	{
		operand.erase(operand.begin());
		return true;
	}
	printError("removeRRegisterPrefix():: invalid register format currentToken[" + m_currentToken + "] currentOperand[" + operand +"]");
	return false;
	
}

//====================================================================================//
//====================================================================================//
//====================================================================================//

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
		m_symbolTable[m_currentToken] = { m_currentRamIndex, decNumber, SymbolTypes::MACRO, SymbolUsageStatus::NOT_USED };
		//m_currentRamIndex--;
	}

}

//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//


std::vector<int> AssemblerCPU_t2::run()
{
	std::cout << rang::bg::blue << "Running first pass...\n" << rang::style::reset;

	firstPass();

	if (m_error == true)
	{
		return std::vector<int>{};
	}

	std::cout << rang::bg::blue << "Running second pass...\n" << rang::style::reset;
	secondPass();
	if (m_error == true)
	{
		return std::vector<int>{};
	}

#ifdef _DEBUG
	printOutput();
#endif // _DEBUG

	return generateOutput();
	//return std::vector<int> {};
}



std::vector<int> AssemblerCPU_t2::generateOutput()
{
	std::vector<int> output(256,0);

	for (size_t i = 0; i < m_memoryMap.size(); i++)
	{
		std::cout << m_memoryMap[i].m_type
			<< " " << std::hex << m_memoryMap[i].m_package.partx
			<< " " << m_memoryMap[i].m_package.party
			<< " " << m_memoryMap[i].m_ramIndex
			<< "\n";

		//push first byte [opcode part]
		int index = m_memoryMap[i].m_ramIndex;
		
		output[index] = m_memoryMap[i].m_type;
		
		//push operand part
		//2.byte
		if (m_memoryMap[i].m_byteAmount == 2)
		{
			index++;
			output[index] = m_memoryMap[i].m_package.partx;
		}
		//3.byte LOAD,STR 3bayt commands
		else
		{
			index++;
			output[index] = m_memoryMap[i].m_package.partx;
			index++;
			output[index] = m_memoryMap[i].m_package.party;
		}
		

	}

	return output;
}

void AssemblerCPU_t2::firstPass()
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

		TokenType currentType = m_commandInfoTable[m_currentToken].m_type;
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
				printError("Duplicate label [" + m_currentToken + "] \nline number = [" + std::to_string(m_lineNumber) + "]");
			}
			else
			{
				m_symbolTable[m_currentToken] = { m_currentRamIndex, 0, SymbolTypes::LABEL, SymbolUsageStatus::NOT_USED};
				//m_currentRamIndex--;
			}
			
			
		}
		//Degisken tanimi
		else if (lexer.checkNextCharIsEqual() == '=')
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

			case JMP:
			case JGZ:
				adres = lexer.getNextToken();
				
				if (!m_symbolTable.contains(adres))
				{
					m_symbolTable[adres] = {-1, 0, SymbolTypes::LABEL, SymbolUsageStatus::UNDEFINED};
				}
				//eger adres tablodaysa tanimli hale getir
				else if(m_symbolTable[adres].m_type == SymbolTypes::LABEL)
				{
					m_symbolTable[adres].m_status = SymbolUsageStatus::DEFINED;
				}
//TODO
				//aaaaa kafa agrisi karsilastirma lazim buraya LABEL ile VAR arasinda
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
	std::cout << rang::bg::blue << "Printing symbol table...\n" << rang::style::reset;
#endif // _DEBUG

	//cpp17 !!
	for (const auto& [key, value] : m_symbolTable) 
	{
		

#ifdef _DEBUG
		std::cout << "symbol name: " << key <<
			" line number: " << value.m_lineNumber <<
			" symbol type : " << m_symbolNameTable[value.m_type] <<
			" status: "<< toString(value.m_status) << "\n";

#endif // _DEBUG

		if (value.m_status == SymbolUsageStatus::NOT_USED)
		{
			printWarning("firstPass()::NOT_USED| type [" + toString(value.m_type) + "] not used name: [" + key + "] value: [" + std::to_string(value.m_value) + "]\nline number = " + std::to_string(value.m_lineNumber));
		}

		if (value.m_lineNumber == -1)
		{
			printError("firstPass()::Undefine label is used in program label name = [" + key + "]\nline number = " + std::to_string(value.m_lineNumber));
			m_error = true;
			break;
		}
	}
/*
---------------------------------------------------------------
===============================================================
---------------------------------------------------------------
*/
	

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


	//	TokenType currentType = m_commandInfoTable[m_currentToken].m_type;
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
	//	m_memoryMap.push_back(memLayout);
	//	m_lineNumber++;
	//}
}

void AssemblerCPU_t2::secondPass()
{
	//dosyanin basina geri don
	lexer.reset();
	m_currentRamIndex = 0;
	m_lineNumber = 0;

	while (m_error == false)
	{
		m_error = true;
	}
	
}

#ifdef _DEBUG
void AssemblerCPU_t2::printOutput()
{
	std::cout << rang::bg::green <<"Printing results..." << rang::style::reset << "\n";
	for (size_t i = 0; i < m_memoryMap.size(); i++)
	{
//FIXME toString() ?
		std::cout //<< toString(m_memoryMap[i].m_type)
			<< std::hex << m_memoryMap[i].m_type
			<< " " << m_memoryMap[i].m_package.partx
			<< " " << m_memoryMap[i].m_package.party
			<< std::dec << "\n";
	}
}
#endif // _DEBUG





}//namespace END