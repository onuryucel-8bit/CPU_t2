#pragma once

#include <vector>
#include <iostream>
#include <fstream>

#include "Assembler/AssemblerCPU_t2.h"

class LogisimRamPrinter
{
public:
	LogisimRamPrinter();
	~LogisimRamPrinter();

	void run(std::vector<asmp::RamLayout>& ramLayout);

private:

};