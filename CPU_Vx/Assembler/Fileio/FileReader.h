#pragma once

//res
/*
https://www.youtube.com/watch?v=JaT0Fnux1nQ&list=PLRwVmtr-pp06qT6ckboaOhnm9FxmzHpbY&index=20
*/
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

class FileReader
{
public:
	FileReader();
	~FileReader();
	
	std::string read(const char* path);	

	void write(std::string& path, std::string& data, std::ios::openmode mode);

	
private:
	
};
