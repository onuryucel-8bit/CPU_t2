#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <cstdint>


class BinaryRamPrinter
{
public:
	BinaryRamPrinter();
	~BinaryRamPrinter();

	void run(std::string path, std::vector<uint8_t>& binaryData);
private:

};
