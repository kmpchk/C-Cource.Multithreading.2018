#pragma once

#include "picosha2.h"

class MerkleTree 
{
private:
	std::string* _roothash;
	std::string* _input;
	std::string* _hashes;
public:	
	void BuildTree(std::string* input) 
	{
		_input = input;
		_hashes = new std::string[4];
		for (int i = 0; i < 4; i++)
		{
			_hashes[i] = picosha2::hash256_hex_string(input[i]);
		}
		//std::string hash0 = hashes[0] + hashes[1];
		//std::string hash1 = hashes[2] + hashes[3];
		_roothash = new std::string(_hashes[0] + _hashes[1] + _hashes[2] + _hashes[3]);
	}

	std::string* GetRootHash()
	{
		return _roothash;
	}

	void ClearMemory()
	{
		delete[] _input;
		delete[] _hashes;
		delete _roothash;
	}

};