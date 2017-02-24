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

#include "HelperFunctions.h"

using namespace std;

int main(int argc, char* argv[])
{
	validateCmdParams(argc, argv);

	int numberFiles = 0;
	int arrSize = 0;
	string fileName;
	uint64_t numbersFromFile[10000];

	assert(istringstream(argv[1]) >> numberFiles);

	/*
	fill array with entries from first file; as we search for intersection,
	this array is the most numbers we can get to the final answer
	*/
	getline(cin, fileName);
	fileToArray(fileName, numbersFromFile, arrSize);

	//no need for bigger size - we will not add any more numbers
	Hash hashIntersection(arrSize + 1);

	for (int i = 0; i < arrSize; i++)
	{
		hashIntersection.Add(numbersFromFile[i], 1);
	}

	for (int i = 1; i < numberFiles; i++)
	{
		getline(cin, fileName);
		//increment value for every number that we meet
		fileToHash(fileName, numbersFromFile, hashIntersection);
	}

	int currentValue = 0;
	std::fstream writeFile;
	char* resultFile = "result.bin";

	writeFile.open(resultFile, std::fstream::binary | std::ios_base::app);
	if (!writeFile.is_open())
	{
		std::cerr << "Unable to open file " << resultFile << "\n";
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < arrSize; i++)
	{
		currentValue = hashIntersection.GetValue(numbersFromFile[i]);
		//number was found in all files - its value equals the number of files
		if (currentValue == numberFiles)
		{
			writeFile.write((char*)&numbersFromFile[i], sizeof(numbersFromFile[i]));
		}
	}
	writeFile.close();

	system("pause");
	return 0;
}

