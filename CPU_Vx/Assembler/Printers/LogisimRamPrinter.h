#pragma once

#include <vector>
#include <iostream>
#include <fstream>

#include "../utils/Radix.h"

class LogisimRamPrinter
{
public:
	LogisimRamPrinter();
	~LogisimRamPrinter();

	void run(std::vector<int>& ramLayout);

	
private:
	
};