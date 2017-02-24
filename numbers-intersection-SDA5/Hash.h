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

#pragma once

#include <limits>
#include <cstdint>

const int INT_MIN_VALUE = std::numeric_limits<int>::min();
const int INT_MAX_VALUE = std::numeric_limits<int>::max();

class Hash
{
public:
	Hash(size_t MaxSize);
	~Hash();

	bool Add(const uint64_t ID, const int Value);
	bool IncrementValue(const uint64_t ID);
	uint64_t GetValue(const uint64_t ID);

private:
	int CalculateHash(uint64_t ID);
	
	struct bucket
	{
		uint64_t id;
		uint64_t value;
		bucket() : id(0), value(0) {};
	};

	size_t BufferSize;
	size_t ElementsCount;
	bucket* pBuffer;
};