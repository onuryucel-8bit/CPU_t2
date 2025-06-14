#include "LogisimRamPrinter.h"

LogisimRamPrinter::LogisimRamPrinter()
{
}

LogisimRamPrinter::~LogisimRamPrinter()
{
}

void LogisimRamPrinter::run(std::vector<asmp::RamLayout>& ramLayout)
{
	
	std::string outpath = "outputRAM.txt";
	
	//std::ios_base::app
	std::ofstream file(outpath);
	if (!file.is_open())
	{
		std::cout << "error::main()::file\n";
	}
	
	
	//--------------------------------------------//
	file << "v3.0 hex words plain\n";
	
	std::cout << "================================\n";
	for (size_t i = 0; i < ramLayout.size(); i++)
	{
		std::cout << ramLayout[i].m_type
			<< " " << ramLayout[i].m_package.partx
			<< " " << ramLayout[i].m_package.party
			<< " " << ramLayout[i].m_ramIndex 
			<< "\n";
	}
	////not:
	////output.length() kismi dinamik yapilabilir 2byte veri icin
	//for (size_t i = 0; i < ram.size(); i++)
	//{
	//	std::string output = rdx::dectohex(ram[i]);
	//	
	//	if (output.length() == 1)
	//	{
	//		output = "0" + output;
	//	}
	//	
	//	file << output << " ";
	//
	//	if ((i + 1) % 16 == 0)
	//	{
	//		file << "\n";
	//	}
	//}
	
	//--------------------------------------------//
	file.close();
}
