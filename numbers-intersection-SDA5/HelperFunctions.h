/**
*
* Solution to homework task
* Data Structures Course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2016/2017
*
* @author Stanislav Zmiycharov
* @idnumber 61883
* @task 5
* @compiler VC
*
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <assert.h>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "Hash.h"

//check if a number is given from console
void validateCmdParams(int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cerr << "Usage: " << argv[0] << " <number>" << "\n";
		system("pause");
		exit(EXIT_FAILURE);
	}
}

//fill array with entries from binary file
void fileToArray(std::string fileName, uint64_t(&numbersFromFile)[10000], int &arrSize)
{
	std::fstream readFile;

	uint64_t num;

	readFile.open(fileName, std::ios_base::binary | std::ios_base::in);
	if (readFile.is_open())
	{
		while (readFile.read((char*)&num, sizeof(num)))
		{
			numbersFromFile[arrSize] = num;
			++arrSize;
		}
	}
	else
	{
		std::cerr << "Unable to open file " << fileName << "\n";
		exit(EXIT_FAILURE);
	}
	readFile.close();
}

//increment value for every entry in filename to our hash
void fileToHash(std::string fileName, uint64_t(&numbersFromFile)[10000], Hash &hash)
{
	std::fstream readFile;
	int numbsFromFile[10000];
	int arrSize = 0;

	uint64_t num;

	readFile.open(fileName, std::ios_base::binary | std::ios_base::in);
	if (readFile.is_open())
	{
		while (readFile.read((char*)&num, sizeof(num)))
		{	
			if (std::find(std::begin(numbsFromFile), std::end(numbsFromFile), num) == std::end(numbsFromFile))
			{
				numbsFromFile[arrSize] = num;
				arrSize++;
				hash.IncrementValue(num);
			}
			
		}
	}
	else
	{
		std::cerr << "Unable to open file " << fileName << "\n";
		exit(EXIT_FAILURE);
	}
	readFile.close();
}