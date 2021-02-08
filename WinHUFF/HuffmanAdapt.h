//-------------------------------------------------------------------------------------

#ifndef HuffmanAdaptH
#define HuffmanAdaptH
//-------------------------------------------------------------------------------------
#include <string>
#include <stdio.h>
#include "Tree.h"
//-------------------------------------------------------------------------------------
class HuffmanAdapt
{
private:
	struct BitFile
	{
		FILE *file;
		unsigned char mask;
		int rack;
	};

	Tree tree;
	unsigned FileSize(std::string fileName);
	BitFile* OpenOutputFile(std::string fileName);
	BitFile* OpenInputFile(std::string fileName);
	void CloseOutputFile(BitFile* file);
	void CloseInputFile(BitFile* file);
	void OutputBits(BitFile* file, const unsigned long code, const int count);
	bool InputBit(BitFile* file);
	unsigned long InputBits(BitFile* file, const int count);
	void EncodeSymbol(const unsigned c, BitFile* file);
	int DecodeSymbol(BitFile* file);
	void UpdateModel(const int c);
	void RebuildTree();
	void SwapNodes(const int i, const int j);
	void AddNewNode(const int c);
	void CompressFile(FILE *file, BitFile* compressedFile);
	void ExpandFile(FILE* newFile, BitFile* compressedFile);
public:
	unsigned Compress(std::string file, std::string archive);
	void Decompress(std::string archive, std::string file);
};
//-------------------------------------------------------------------------------------
#endif
