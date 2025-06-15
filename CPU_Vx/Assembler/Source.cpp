//std::string program;
//
//int position = -1;
//char currentChar = 0;
//size_t m_lineNumber = 1;
//bool m_error = false;
//size_t ramIndex = 0;
//
//
//std::vector<int> RAM(16*16);
//
////--------------------------------------------//
//void advance()
//{
//	position++;
//	if (position >= program.length())
//	{
//		currentChar = EOF;
//	}
//	else
//	{
//		currentChar = program[position];
//	}
//}
//
//void skipWhiteSpace()
//{
//	while (std::isspace(currentChar) && currentChar != EOF)
//	{
//		advance();
//	}
//}
//
//void skipComments()
//{
//	if (currentChar == ';')
//	{
//		while (currentChar != '\n' && currentChar != EOF)
//		{
//			advance();
//		}
//
//		advance();
//	}
//}
//
//std::string getNextToken()
//{
//	std::string token;
//	skipWhiteSpace();
//	skipComments();
//
//	while (currentChar != ',' && 
//		currentChar != ' '  && 
//		currentChar != '\t' && 
//		currentChar != '\n' && 
//		currentChar != EOF)
//	{
//		token += currentChar;
//		advance();
//	}
//	advance();
//	return token;
//}
////--------------------------------------------//
//void removeRRegisterPrefix(std::string& operand)
//{
//	if (operand[0] == 'r' || operand[0] == 'R')
//	{
//		operand.erase(operand.begin());
//	}
//}
//
//void removeHexPrefix(std::string& operand)
//{
//	if (operand[0] == '0' && operand[1] == 'x')
//	{
//		operand.erase(0, 2);	
//	}
//}
//
//void processTwoPartOpcode(std::string& opcode)
//{
//
//	std::string operandA = getNextToken();
//	std::string operandB = getNextToken();
//
//	//ADD r5, r1
//	removeRRegisterPrefix(operandA);//5
//	removeRRegisterPrefix(operandB);//1
//
//	//							rx	 ry
//	//ADD 0000_1000	0x08	00(00_0)(000)
//	//operandA = 0000_0101 => dec5
//	//operandB = 0000_0001 => dec1
//
//	//operandA << 3 0010_1000
//	//operandB		0000_0001
//	//| OR			0010_1001
//
//	int rx = std::stoi(operandA); // 5
//	int ry = std::stoi(operandB); // 1
//
//	rx = (rx << 3) | ry;
//
//	
//	
//	RAM[ramIndex] = rdx::hexToDeC(m_opcodeList[opcode].hexVal);
//	ramIndex++;
//	RAM[ramIndex] = rx;
//	ramIndex++;	
//}
//
//void translateToHex(std::string opcode)
//{
//	if (!m_opcodeList.contains(opcode))
//	{
//		std::cout << "ERROR::translateToHex():: opcode [" << opcode <<"] is not valid | line number = " << m_lineNumber << "\n";
//		m_error = true;
//		return;
//	}
//
//	std::string operandA, operandB, output;
//	int rx, ry;
//	
//	switch (m_opcodeList[opcode].opcodeID)
//	{
//		
//	case OpcodeID::NOT:
//		operandA = getNextToken();//get r0 part
//		
//		removeRRegisterPrefix(operandA);
//
//		RAM[ramIndex] = rdx::hexToDeC(m_opcodeList[opcode].hexVal);
//		ramIndex++;
//		RAM[ramIndex] = rdx::hexToDeC(operandA);
//		ramIndex++;
//		break;
//
//	case OpcodeID::SHL:
//	case OpcodeID::SHR:
//		break;
//
//	case OpcodeID::ADD:
//	case OpcodeID::SUB:	
//	case OpcodeID::AND:
//	case OpcodeID::OR:
//	case OpcodeID::XOR:
//	case OpcodeID::MOV:
//
//		processTwoPartOpcode(opcode);		
//		break;
//
//	case OpcodeID::LOAD:
//		//LOAD r0,0x5f <= input
//		operandA = getNextToken();//get r0 part
//		operandB = getNextToken();//get 0x5f
//
//		removeRRegisterPrefix(operandA);//result 0 <= r0
//		removeHexPrefix(operandB);
//		
//		
//		//= "01"+"0"+"5"+"5f"
//		//01055f 3byte
//		RAM[ramIndex] = rdx::hexToDeC(m_opcodeList[opcode].hexVal);
//		ramIndex++;
//
//		RAM[ramIndex] = rdx::hexToDeC(operandA);
//		ramIndex++;
//
//		RAM[ramIndex] = rdx::hexToDeC(operandB);
//		ramIndex++;		
//		break;
//
//
//	case OpcodeID::STR:
//		operandA = getNextToken();
//		operandB = getNextToken();
//
//		removeHexPrefix(operandA);//adres
//		removeRRegisterPrefix(operandB);//reg
//
//		RAM[ramIndex] = rdx::hexToDeC(m_opcodeList[opcode].hexVal);
//		ramIndex++;
//
//		RAM[ramIndex] = rdx::hexToDeC(operandA);
//		ramIndex++;
//
//		RAM[ramIndex] = rdx::hexToDeC(operandB);		
//		ramIndex++;
//		break;
//	}
//}
//
//void logisimRAMOutput()
//{
//	std::string standartHeader = "v3.0 hex words plain\n";
//	std::string outPath = "D:\\ProgrammingProjects\\Logisim\\8bitCPU_t2\\CpuLogisim\\CpuLogisimoutputRAM.txt";
//
//	//std::ios_base::app
//	std::ofstream file(outPath);
//	if (!file.is_open())
//	{
//		std::cout << "ERROR::main()::file\n";
//	}
//
//	//print
//	for (size_t i = 0; i < 10; i++)
//	{
//		std::cout << std::hex << RAM[i] << "\n";
//	}
//	//--------------------------------------------//
//	file << standartHeader;
//	
//	//NOT:
//	//output.length() kismi dinamik yapilabilir 2byte veri icin
//	for (size_t i = 0; i < RAM.size(); i++)
//	{
//		std::string output = rdx::decToHex(RAM[i]);
//		
//		if (output.length() == 1)
//		{
//			output = "0" + output;
//		}
//		
//		file << output << " ";
//
//		if ((i + 1) % 16 == 0)
//		{
//			file << "\n";
//		}
//	}
//
//	//--------------------------------------------//
//	file.close();
//}

#include <string>


#include "Assembler/AssemblerCPU_t2.h"
#include "Fileio/FileReader.h"
#include "LogisimRamPrinter.h"

int main()
{
	FileReader fr;
	std::string file = fr.read("program.txt");

	asmp::AssemblerCPU_t2 assembler(file);
	std::vector<int> output = assembler.run();
	
	 

	LogisimRamPrinter printer;
	printer.run(output);

}