#include "Parser.h"

namespace asmc
{

Parser::Parser(asmc::Lexer* lexer)
{
	m_lexer = lexer;

	f_errorParser = false;
	m_lineNumber = 1;
	m_ramLocation = 0;

	m_peekToken = m_lexer->getToken();
	nextToken();

	//{reg-reg,reg-hex}
	m_variantTable[asmc::TokenType::ADD] = { 0x08,0x10 };
	m_variantTable[asmc::TokenType::SUB] = { 0x09,0x11 };
	m_variantTable[asmc::TokenType::AND] = { 0x0c,0x14 };
	m_variantTable[asmc::TokenType::OR] =  { 0x0D,0x15 };
	m_variantTable[asmc::TokenType::XOR] = { 0x0F,0x17 };
	m_variantTable[asmc::TokenType::SHL] = { 0x0A,0x10 };
	m_variantTable[asmc::TokenType::SHR] = { 0x0B,0x10 };

}

void Parser::secondPass()
{
	std::cout << rang::bg::blue << "Starting second pass..." << rang::style::reset << "\n";

	for (const auto& [key, value] : m_symbolTable)
	{
		if (value.m_status == LabelStatus::Undefined)
		{
			printError("Undefined label ["+ key +"] line number [test]");
			f_errorParser = true;
			return;
		}
		else if (value.m_status == LabelStatus::NotUsed)
		{
			//printWarning("Not used label[" + key + "]");
		
			//check jump table if label used
			for (const auto& [labelName, memLayout] : m_jumpTable)
			{
				if (labelName == key)
				{
					//combine label address with jmp instructions
					std::cout << "ramIndex " << value.m_ramIndex << "\n";
					m_jumpTable[labelName].m_firstByte = value.m_ramIndex;
					m_symbolTable[key].m_status = LabelStatus::Used;

					m_output.push_back(m_jumpTable[labelName]);
				}
			}

			//for warning
			if (value.m_status == LabelStatus::NotUsed)
			{
				printWarning("Label not used[" + key + "]");
			}
		}
	}

	
}

bool Parser::checkToken(asmc::TokenType type)
{
	return type == m_currentToken.m_type;
}

bool Parser::checkPeek(asmc::TokenType type)
{
	return type == m_peekToken.m_type;
}

void Parser::nextToken()
{
	m_currentToken = m_peekToken;
	m_peekToken = m_lexer->getToken();//lexer
}

void Parser::program()
{
	std::cout << rang::bg::blue << "Starting first pass..." << rang::style::reset << "\n";

	while (m_currentToken.m_type != asmc::TokenType::ENDOFLINE && !f_errorParser && !m_lexer->getErrorFlag())
	{
		
		statement();
		m_lineNumber++;
	}

	std::cout << std::boolalpha << "lexer error flag "<< m_lexer->getErrorFlag() << "\n";
	
	

	if (m_lexer->getErrorFlag())
	{
		printError("LEXER error occured check");
	}

	if (!f_errorParser && !m_lexer->getErrorFlag())
	{
		std::cout << rang::bg::blue << "Printing m_symbolTable" << rang::style::reset << "\n";

		for (const auto& [key, value] : m_symbolTable)
		{
			std::cout << '[' << key << "] address [" << value.m_ramIndex << "]\n";
		}

		secondPass();
		printOutput();
		generateBinaryArr();
	}
	
}

std::vector<int> Parser::getBinaryData()
{
	return m_binaryProgram;
}

bool Parser::checkError()
{
	return f_errorParser || m_lexer->getErrorFlag(); 
}

void Parser::statement()
{
	switch (m_currentToken.m_type)
	{
	case asmc::TokenType::NEWLINE:
		nextToken();
		break;

	case asmc::TokenType::OUT:
		parseOUT();
		break;

	case asmc::TokenType::LOAD:
		parseLOAD();		
		break;
	case asmc::TokenType::STR:
		parseSTR();		
		break;
	case asmc::TokenType::MOV:
		parseMOV();
		break;

	case asmc::TokenType::SUB:
	case asmc::TokenType::ADD:
	case asmc::TokenType::AND:
	case asmc::TokenType::OR:
	case asmc::TokenType::XOR:
	case asmc::TokenType::SHL:
	case asmc::TokenType::SHR:
		parseALUcommands();		
		break;

	case asmc::TokenType::NOT:
		parseNOT();
		break;

	case asmc::TokenType::JMP:
	case asmc::TokenType::JZ:
	case asmc::TokenType::JLZ:
	case asmc::TokenType::JGZ:
	case asmc::TokenType::JSC:
	case asmc::TokenType::JUC:		
		m_lineNumber--;
		parseJumpCommands();
		break;


	
	//skip
	case asmc::TokenType::LABEL:
		
		//error multiple definition of the label 
		if (m_symbolTable.contains(m_currentToken.m_text) && (m_symbolTable[m_currentToken.m_text].m_status == LabelStatus::NotUsed || m_symbolTable[m_currentToken.m_text].m_status == LabelStatus::Used))
		{
			//m_lineNumber++;
			printError("statement():: Label [" + m_currentToken.m_text + "] defined twice or more \n");
			f_errorParser = true;
			break;
		}

		m_symbolTable[m_currentToken.m_text] = { m_ramLocation, LabelStatus::NotUsed };

		//DEBUG_printMessage("label address[" + std::to_string(m_ramLocation) + "]");

		m_lineNumber--;
		nextToken();		
		break;
	default:
		break;
	}

	//DEBUG_printMessage("ramLocation[" + std::to_string(m_ramLocation) +"]");
}

void Parser::generateBinaryArr()
{
	std::cout << rang::bg::blue << "Generating binary data..." << rang::style::reset << "\n";
	m_binaryProgram.resize(256);
	for (size_t i = 0; i < m_output.size(); i++)
	{
		int ramLocation = m_output[i].m_ramIndex;

		m_binaryProgram[ramLocation] = m_output[i].m_opcode;
		m_binaryProgram[ramLocation + 1] = m_output[i].m_firstByte;

		if (m_output[i].m_byteAmount == 3)
		{
			m_binaryProgram[ramLocation + 2] = m_output[i].m_secondByte;
		}
	}
	/*std::cout << "HEY!!!\n";
	
	for (size_t i = 0; i < m_binaryProgram.size(); i++)
	{
		std::cout << std::hex << m_binaryProgram[i] << "\n";
	}
	std::cout << std::dec;*/
}
//------------------------------------------------------------------------//
//-----------------------PRINT--------------------------------------------//
//------------------------------------------------------------------------//

void Parser::DEBUG_printMessage(std::string message)
{
	std::cout << rang::style::underline << rang::fg::green << message << rang::style::reset << "\n";
}

void Parser::printWarning(std::string message)
{
	std::cout << rang::fg::yellow
		<< "WARNING::" << message
		//<< " line number = " << m_lineNumber
		//<< " next and peek Token [" << m_currentToken.m_text << " " << m_peekToken.m_text
		<< rang::style::reset << "\n";
}

void Parser::printError(std::string message)
{
	std::cout << rang::fg::red 
		<< "ERROR::"<< message 
		<< " line number = "<< m_lineNumber 
		<< " next and peek Token [" << m_currentToken.m_text << " " << m_peekToken.m_text
		<< "]" << rang::style::reset << "\n";
}

void Parser::printOutput()
{
	std::cout << rang::bg::blue << "Printing output data..." << rang::style::reset <<"\n";
	for (size_t i = 0; i < m_output.size(); i++)
	{		
		std::cout << std::hex << m_output[i].m_opcode << ":" << m_output[i].m_firstByte <<":" << m_output[i].m_secondByte << std::dec <<"\n";
	}
}

//------------------------------------------------------------------------//
//------------------------------------------------------------------------//
//------------------------------------------------------------------------//

void Parser::createMemoryLayout(int byteAmount, int opcode)
{
	MemoryLayout ml;
	ml.m_byteAmount = byteAmount;
	ml.m_ramIndex = m_ramLocation;
	ml.m_opcode = opcode;
	ml.m_firstByte = rdx::hexToDeC(m_currentToken.m_text);

	if (m_peekToken.m_type != asmc::TokenType::ENDOFLINE)
	{
		ml.m_secondByte = rdx::hexToDeC(m_peekToken.m_text);
	}
	
	
	if (byteAmount == 2)
	{
		
		//0000_0rrr
		//00rr_r000
		ml.m_firstByte = ml.m_firstByte << 3;
		//00rr_rRRR
		ml.m_firstByte = ml.m_firstByte | ml.m_secondByte;
	}

	m_output.push_back(ml);

	m_ramLocation += byteAmount;
}

bool Parser::expect(asmc::Token token, asmc::TokenType expectedIdent)
{
	if (token.m_type != expectedIdent)
	{
				
		printError("expect(TokenType):: expected["+ std::string(magic_enum::enum_name(expectedIdent)) + "]\n");
		f_errorParser = true;
		return false;
	}
	
	return true;
}


//------------------------------------------------------------------------//
//-------------------------ParseX()---------------------------------------//
//------------------------------------------------------------------------//

//<add>/<sub>/<or>/<xor>/<and> ::= <register> (<register> | <hex>)
void Parser::parseALUcommands()
{	
	OpcodePair opcode = m_variantTable[m_currentToken.m_type];

	nextToken();
	if (!expect(m_currentToken, asmc::TokenType::REGISTER))
	{
		return;
	}

	//type ADD/SUB/XOR... rx,ry 
	if (checkPeek(asmc::TokenType::REGISTER))
	{		
		createMemoryLayout(2, opcode.regRegister);
		nextToken();
		nextToken();
	}
	//type ADD/SUB/XOR... rx,0xff 
	else if (checkPeek(asmc::TokenType::HEXNUMBER))
	{
		createMemoryLayout(3, opcode.regImmediate);
		nextToken();
		nextToken();
	}
	else
	{
		printError("unexcepted statement\n");
		f_errorParser = true;
	}
}

void Parser::parseJumpCommands()
{
	int opcode = m_currentToken.m_type;

	nextToken();
	if (!expect(m_currentToken, asmc::TokenType::JUMPLOC))
	{
		return;
	}

	//is it forward declared label ?
	if (!m_symbolTable.contains(m_currentToken.m_text))
	{
		m_symbolTable[m_currentToken.m_text] = { m_ramLocation, LabelStatus::Undefined };

		//push jgz location info for second pass calc
		m_jumpTable[m_currentToken.m_text] =
		{
			.m_ramIndex = m_ramLocation,
			.m_opcode = opcode,
			.m_firstByte = 0,
			.m_secondByte = 0,
			.m_byteAmount = 2
		};

		m_ramLocation += 2;
	}
	else
	{
		m_symbolTable[m_currentToken.m_text].m_status = LabelStatus::Used;
		//calc hex push it to std::vector

		MemoryLayout ml;
		ml.m_byteAmount = 2;
		ml.m_ramIndex = m_ramLocation;
		ml.m_opcode = opcode;
		ml.m_firstByte = m_symbolTable[m_currentToken.m_text].m_ramIndex;

		//DEBUG_printMessage("hey [" + std::to_string(ml.m_firstByte) + "]");

		m_output.push_back(ml);

		m_ramLocation += ml.m_byteAmount;
	}

	nextToken();//m_currentToken => Label?
}

void Parser::parseOUT()
{
	nextToken();//m_currentToken => rx
	if (!expect(m_currentToken, asmc::TokenType::REGISTER))
	{
		return;
	}

	if (checkPeek(asmc::TokenType::REGISTER))
	{
		createMemoryLayout(2, 0x05);

		nextToken();//m_currentToken => 0xff
		nextToken();//m_currentToken => nextNewToken
	}
	else if (checkPeek(asmc::TokenType::ADDRESS))
	{
		createMemoryLayout(2, 0x06);

		nextToken();//m_currentToken => 0xff
		nextToken();//m_currentToken => nextNewToken
	}
	else
	{
		printError("OUT :: unexpected ls");
	}

}

void Parser::parseMOV()
{
	nextToken();//m_currentToken => rx
	if (!expect(m_currentToken, asmc::TokenType::REGISTER))
	{
		return;
	}

	if (checkPeek(asmc::TokenType::REGISTER))
	{
		//m_output.push_back(0);

		std::cout << "MOV rx[" << m_currentToken.m_text << "]:: ry[" << m_peekToken.m_text << "]\n";


		MemoryLayout ml;
		ml.m_byteAmount = 2;
		ml.m_ramIndex = m_ramLocation;
		ml.m_opcode = 0x04;
		ml.m_firstByte = rdx::hexToDeC(m_currentToken.m_text);
		ml.m_secondByte = rdx::hexToDeC(m_peekToken.m_text);

		//0000_0rrr
		//00rr_r000
		ml.m_firstByte = ml.m_firstByte << 3;
		//00rr_rRRR
		ml.m_firstByte = ml.m_firstByte | ml.m_secondByte;

		m_output.push_back(ml);

		nextToken();
		nextToken();//m_currentToken => nextNewToken

		m_ramLocation += ml.m_byteAmount;
	}
}

//<load> ::= <register>,(<hexnumber> | <address>)
void Parser::parseLOAD()
{
	//LOAD rx,0xff/@5e

	int opcode = m_currentToken.m_type;

	nextToken();//m_currentToken => rx
	if (!expect(m_currentToken, asmc::TokenType::REGISTER))
	{
		return;
	}

	//is LOAD rx,0xff type?
	if (checkPeek(asmc::TokenType::HEXNUMBER))
	{
		//m_output.push_back(0);

		std::cout <<"LOAD rx[" << m_currentToken.m_text << "]:: sayi[" << m_peekToken.m_text << "]\n";

		createMemoryLayout(3, 0x01);
		
		nextToken();//m_currentToken => 0xff
		nextToken();//m_currentToken => nextNewToken

		
	}
	//LOAD rx,@fa ?
	else if (checkPeek(asmc::TokenType::ADDRESS))
	{
		std::cout << "LOAD rx[" << m_currentToken.m_text << "]:: address[" << m_peekToken.m_text << "]\n";

		createMemoryLayout(3, 0x02);

		

		nextToken();//m_currentToken => address
		nextToken();//m_currentToken => nextNewToken
	}
	else
	{
		printError("parseLoad():: undefined second operand ["+m_peekToken.m_text+"]");
		f_errorParser = true;
		return;
	}
}





void Parser::parseSTR()
{
	int opcode = m_currentToken.m_type;

	nextToken();
	if (!expect(m_currentToken, asmc::TokenType::ADDRESS))
	{
		return;
	}

	if (checkPeek(asmc::TokenType::REGISTER))
	{
		createMemoryLayout(3, opcode);

		nextToken();
		nextToken();
	}

}

void Parser::parseNOT()
{
	int opcode = m_currentToken.m_type;

	nextToken();//currentoken => rx
	if (!expect(m_currentToken, asmc::TokenType::REGISTER))
	{
		return;
	}

	if (checkToken(asmc::TokenType::REGISTER))
	{
		MemoryLayout ml;
		ml.m_byteAmount = 2;
		ml.m_ramIndex = m_ramLocation;
		ml.m_opcode = opcode;
		ml.m_firstByte = rdx::hexToDeC(m_currentToken.m_text);

		m_output.push_back(ml);

		m_ramLocation += 2;

		//currenttoken => nextToken
		//peek => nexttoken + 1
		nextToken();		
	}
}

}
