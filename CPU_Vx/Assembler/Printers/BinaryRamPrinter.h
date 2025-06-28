#pragma once

#include <iostream>
#include <vector>
#include <fstream>


class BinaryRamPrinter
{
public:
	BinaryRamPrinter();
	~BinaryRamPrinter();

	void run(std::string path, std::vector<int>& binaryData);
private:

};
