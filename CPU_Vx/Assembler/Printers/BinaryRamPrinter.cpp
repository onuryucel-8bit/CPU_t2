#include "BinaryRamPrinter.h"

BinaryRamPrinter::BinaryRamPrinter()
{
}

BinaryRamPrinter::~BinaryRamPrinter()
{
}

void BinaryRamPrinter::run(std::string path, std::vector<int>& binaryData)
{
	std::ofstream file(path, std::ios::binary);

	if (!file)
	{
		std::cout << "ERROR::BinaryRamPrinter()::run():: Couldnt open the file\n";
		return;
	}

	file.write((char*)binaryData.data(), binaryData.size());

	file.close();
}
