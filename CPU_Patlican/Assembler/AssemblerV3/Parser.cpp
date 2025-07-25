#include "Parser.h"

namespace asmc
{

//TODO put lexer inside of parser as a member
Parser::Parser(std::string& program)
	: m_lexer(program)
{
	

	f_errorParser = false;

	//---deb flags---/
	f_debugTokenData = true;
	f_debugAll = true;
	f_debugOpcodes = false;
	//--------------//

	m_lineNumber = 1;
	m_ramLocation = 0;

	m_peekToken = m_lexer.getToken();
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

void Parser::program()
{
	std::cout << rang::bg::blue << "Starting first pass..." << rang::style::reset << "\n";

	while (m_currentToken.m_type != asmc::TokenType::ENDOFLINE && !f_errorParser && !m_lexer.getErrorFlag())
	{
		statement();
		m_lineNumber++;
	}

	std::cout << std::boolalpha << "lexer error flag " << m_lexer.getErrorFlag() << "\n";

	if (m_lexer.getErrorFlag())
	{
		printError("LEXER error occured check");
		return;
	}

	//----------------------------Second Pass-------------------------------------//

	if (!f_errorParser)
	{

		if (f_debugAll)
		{
			std::cout << rang::bg::blue << "Printing m_symbolTable" << rang::style::reset << "\n";
			for (const auto& [key, value] : m_symbolTable)
			{
				std::cout << '[' << key << "] address [" << value.m_ramIndex << "]\n";
			}
		}


		secondPass();
		printOutput();
		generateBinaryArr();
	}

}

void Parser::secondPass()
{
	std::cout << rang::bg::blue << "Starting second pass..." << rang::style::reset << "\n";

	for (const auto& [key, value] : m_symbolTable)
	{
		if (value.m_status == LabelStatus::Undefined)
		{
			printError("Undefined label [" + key + "] line number [test]");
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

void Parser::statement()
{
	if (f_debugTokenData || f_debugAll)
	{
		if (m_currentToken.m_text == "\n")
		{
			std::cout << "CurrentToken.txt[" << "\\n" << "]\n";
		}
		else
		{
			std::cout << "CurrentToken.txt[" << m_currentToken.m_text << "]\n";
		}
		
	}

	switch (m_currentToken.m_type)
	{
	
	case asmc::TokenType::DB:
		parseDB();
		break;

	case asmc::TokenType::ORIGIN:
		parseORIGIN();		
		break;

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

	case asmc::TokenType::ADC:
		parseADC();
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
			break;
		}

		m_symbolTable[m_currentToken.m_text] = { m_ramLocation, LabelStatus::NotUsed };

		
		//labels doesnt count as opcode
		m_lineNumber--;
		nextToken();		
		break;
	default:
		break;
	}	
}

//-----------------------------------------------------------------------//


void Parser::generateBinaryArr()
{
	std::cout << rang::bg::blue << "Generating binary data..." << rang::style::reset << "\n";
	m_binaryProgram.resize(256);
	for (size_t i = 0; i < m_output.size(); i++)
	{
		int ramLocation = m_output[i].m_ramIndex;

		m_binaryProgram[ramLocation] = m_output[i].m_opcode;		

		switch (m_output[i].m_byteAmount)
		{
		
		case 2:			
			m_binaryProgram[ramLocation + 1] = m_output[i].m_firstByte;
			break;

		case 3:						
			m_binaryProgram[ramLocation + 1] = m_output[i].m_firstByte;
			m_binaryProgram[ramLocation + 2] = m_output[i].m_secondByte;
			break;
		}		
	}
	
}

std::vector<uint8_t> Parser::getBinaryData()
{
	return m_binaryProgram;
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

	f_errorParser = true;
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
//--------------------------UTILS-----------------------------------------//
//------------------------------------------------------------------------//

bool Parser::checkCurrentToken(asmc::TokenType type)
{
	return type == m_currentToken.m_type;
}

bool Parser::checkPeek(asmc::TokenType type)
{
	return type == m_peekToken.m_type;
}

bool Parser::checkError()
{
	return f_errorParser || m_lexer.getErrorFlag();
}

void Parser::nextToken()
{
	m_currentToken = m_peekToken;
	m_peekToken = m_lexer.getToken();//lexer
}

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
		printError("Parser::expect(TokenType):: expected["+ std::string(magic_enum::enum_name(expectedIdent)) + "]\n");	
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
		printError("parseALUcommands():: unexcepted statement\n");		
	}
}

//<JMP/JGZ/JUC...> = JMP | JGZ | ... <JUMPLOC>
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

//<OUT> = OUT <REGISTER> | <ADDRESS> | <REGADR>
void Parser::parseOUT()
{

	nextToken();//m_currentToken => rx
	

	if (checkCurrentToken(asmc::TokenType::REGISTER))
	{
		MemoryLayout ml;
		ml.m_byteAmount = 2;
		ml.m_ramIndex = m_ramLocation;
		ml.m_opcode = 0x5;
		ml.m_firstByte = rdx::hexToDeC(m_currentToken.m_text);

		m_output.push_back(ml);

		m_ramLocation += ml.m_byteAmount;

		nextToken();
		nextToken();//m_currentToken => nextNewToken
	}
	else if (checkCurrentToken(asmc::TokenType::ADDRESS))
	{
		MemoryLayout ml;
		ml.m_byteAmount = 2;
		ml.m_ramIndex = m_ramLocation;
		ml.m_opcode = 0x6;
		ml.m_firstByte = rdx::hexToDeC(m_currentToken.m_text);

		m_output.push_back(ml);

		m_ramLocation += ml.m_byteAmount;

		nextToken();
		nextToken();//m_currentToken => nextNewToken
	}
	else if (checkCurrentToken(asmc::TokenType::REGADR))
	{
		MemoryLayout ml;
		ml.m_byteAmount = 2;
		ml.m_ramIndex = m_ramLocation;
		ml.m_opcode = 0x7;
		ml.m_firstByte = rdx::hexToDeC(m_currentToken.m_text);
		
		m_output.push_back(ml);

		m_ramLocation += ml.m_byteAmount;

		nextToken();
		nextToken();//m_currentToken => nextNewToken
	}
	else
	{
		printError("OUT :: unexpected token");
	}

}

//<MOV> = MOV <REGISTER> <REGISTER>
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
		return;
	}
}

//<STR> = STR <ADDRESS> <REGISTER>
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
		MemoryLayout ml;
		ml.m_byteAmount = 3;
		ml.m_ramIndex = m_ramLocation;
		ml.m_opcode = opcode;

		ml.m_firstByte = rdx::hexToDeC(m_peekToken.m_text);
		ml.m_secondByte = rdx::hexToDeC(m_currentToken.m_text);
				
		m_output.push_back(ml);

		m_ramLocation += ml.m_byteAmount;

		nextToken();
		nextToken();
	}
	else
	{
		printError("parseSTR():: check REGISTER part");
	}

}

//<NOT> = NOT <REGISTER>
void Parser::parseNOT()
{
	int opcode = m_currentToken.m_type;

	nextToken();//currentoken => rx
	if (!expect(m_currentToken, asmc::TokenType::REGISTER))
	{
		return;
	}

	if (checkCurrentToken(asmc::TokenType::REGISTER))
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

//<ADC> = <REGISTER>
void Parser::parseADC()
{
	int opcode = m_currentToken.m_type;

	nextToken();//currentoken => rx
	if (!expect(m_currentToken, asmc::TokenType::REGISTER))
	{
		return;
	}

	if (checkCurrentToken(asmc::TokenType::REGISTER))
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

//<ORIGIN> = .ORIGIN <hexnumber>
void Parser::parseORIGIN()
{
	nextToken();
	if (!expect(m_currentToken, asmc::TokenType::HEXNUMBER))
	{
		return;
	}

	m_ramLocation = rdx::hexToDeC(m_currentToken.m_text);

	if (m_ramLocation > 0xff)
	{
		printError("Parser::parseORIGIN:: RAM location out of range expected < 0xff");
	}
	nextToken();
}

//<DB> ::= .DB <hexNumber>+
void Parser::parseDB()
{
	nextToken();
	if (!expect(m_currentToken, asmc::TokenType::HEXNUMBER))
	{
		return;
	}

	while(true)
	{
		MemoryLayout ml;
		ml.m_byteAmount = 1;
		ml.m_ramIndex = m_ramLocation;
		ml.m_opcode = rdx::hexToDeC(m_currentToken.m_text);

		m_output.push_back(ml);

		m_ramLocation += 1;

		if (!checkPeek(asmc::TokenType::HEXNUMBER))
		{
			break;
		}

		nextToken();

	}
	
	nextToken();

}

}
