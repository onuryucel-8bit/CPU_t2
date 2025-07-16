#include "FileReader.h"

FileReader::FileReader()
{
}

std::string FileReader::read(const char* path) {
	std::ifstream file(path);

	if (!file.is_open()) {
		std::cout << "Error :: failed to open Shader file " << path << "\n";
		file.close();
		return "ERROR";
	}

	std::stringstream ss;
	ss << file.rdbuf();

	file.close();

	return ss.str();

}



void FileReader::write(std::string& path, std::string& data, std::ios::openmode mode)
{

	std::ofstream myfile(path, mode);
	if (!myfile.is_open())
	{
		return;
	}
	
	
	myfile << data;
	myfile.close();
	
}

FileReader::~FileReader()
{
}