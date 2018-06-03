#pragma once

#include <string>
#include <vector>

typedef struct _CBlock
{
	std::vector<unsigned char>* data;
	std::string hash;
} CBlock, *PCBlock;