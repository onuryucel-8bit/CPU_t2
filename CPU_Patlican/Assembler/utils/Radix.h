#pragma once
//update file test 160924
//TEST
#include<iostream>
//----
#include<vector>
#include<cstdint>
#include<string>
#include<sstream>
#include<algorithm>
#include<cmath>


namespace rdx {

	/*
	* hex => dec
	* dec => bin
	* dec => hex
	*/

	std::string toBinaryString(size_t dec);

	char printDec_inhexType(int dec);

	//TODO 
	//BIN => X
	size_t binToDec(std::string bin);
	std::string binToHex(size_t bin);

	//HEX => X
	uint8_t hexToDeC(std::string hex);
	uint8_t hexToDeC(char hex);
	std::string hextoBin(std::string bin);
	
	//DEC => X
	std::string decToBin(int dec);
	std::string decToHex(size_t dec);

	//BIN => STR
	//Gets bool array translates to bin str
	std::string binToStr(std::vector<bool> arr, size_t begin, size_t end);
	std::string binToStr(bool arr[], size_t size);
	std::string binToStr(bool arr[], size_t begin, size_t size);
}