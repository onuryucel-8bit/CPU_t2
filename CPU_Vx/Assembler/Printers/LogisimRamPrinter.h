#pragma once

#include <vector>
#include <iostream>
#include <fstream>
#include <cstdint>

#include "../utils/Radix.h"

class LogisimRamPrinter
{
public:
	LogisimRamPrinter();
	~LogisimRamPrinter();

	void run(std::string outputPath, std::vector<uint8_t>& ramLayout);

	
private:
	
};