//-------------------------------------------------------------------------------------

#pragma hdrstop

#include "HuffmanAdapt.h"
#include "MainForm.h"
//-------------------------------------------------------------------------------------
#pragma package(smart_init)
//-------------------------------------------------------------------------------------
unsigned HuffmanAdapt::FileSize(std::string fileName)
{
	unsigned size = 0;
	FILE *file = fopen(fileName.c_str(), "r");
	if (file == NULL)
		return 0;
	fseek(file, 0, SEEK_END);
	size = ftell(file);
	fclose(file);
	return size;
}
//-------------------------------------------------------------------------------------
HuffmanAdapt::BitFile* HuffmanAdapt::OpenOutputFile(std::string fileName)
{
	BitFile* file = new BitFile[sizeof(HuffmanAdapt::BitFile)];
	file->file = fopen(fileName.c_str(), "wb");
	file->rack = 0;
	file->mask = 0x80;
	return file;
}
//-------------------------------------------------------------------------------------
HuffmanAdapt::BitFile* HuffmanAdapt::OpenInputFile(std::string fileName)
{
	BitFile* file = new BitFile[sizeof(HuffmanAdapt::BitFile)];
	file->file = fopen(fileName.c_str(), "rb");
	file->rack = 0;
	file->mask = 0x80;
	return file;
}
//-------------------------------------------------------------------------------------
void HuffmanAdapt::CloseOutputFile(BitFile* file)
{
	if (file->mask != 0x80)
		if (putc(file->rack, file->file) != file->rack)
			ArchiverForm->Error("Error writing to file\n");
	fclose(file->file);
	delete file;
}
//-------------------------------------------------------------------------------------
void HuffmanAdapt::CloseInputFile(BitFile* file)
{
	fclose(file->file);
	delete file;
}
//-------------------------------------------------------------------------------------
void HuffmanAdapt::OutputBits(BitFile* file, const unsigned long code, const int count)
{
	unsigned long mask = 1L << (count - 1);
	while (mask != 0)
	{
		if (mask & code)
			file->rack |= file->mask;
		file->mask >>= 1;
		if (file->mask == 0)
		{
			if (putc(file->rack, file->file)!= file->rack)
				ArchiverForm->Error("Error writing to file\n");
			file->rack = 0;
			file->mask = 0x80;
		}
		mask >>= 1;
	}
}
//-------------------------------------------------------------------------------------
bool HuffmanAdapt::InputBit(BitFile* file)
{
	bool bit;
	if (file->mask == 0x80)
	{
		file->rack = getc(file->file);
		if (file->rack == EOF)
			ArchiverForm->Error("Error reading from file\n");
	}
	bit = file->rack & file->mask;
	file->mask >>= 1;
	if (file->mask == 0)
		file->mask = 0x80;
	return bit;
}
//-------------------------------------------------------------------------------------
unsigned long HuffmanAdapt::InputBits(BitFile* file, const int count)
{
	unsigned long mask = 1L << (count - 1);
	unsigned long code = 0;
	while (mask != 0)
	{
		if (file->mask == 0x80)
		{
			file->rack = getc(file->file);
			if (file->rack == EOF)
				ArchiverForm->Error("Error reading from file\n");
		}
		if (file->rack & file->mask)
			code |= mask;
		mask >>= 1;
		file->mask >>= 1;
		if (file->mask == 0)
			file->mask = 0x80;
	}
	return code;
}
//-------------------------------------------------------------------------------------
void HuffmanAdapt::EncodeSymbol(const unsigned c, BitFile* file)
{
	unsigned long code = 0;
	unsigned long currentBit = 1;
	int codeSize = 0;
	int currentNode = tree.Leaves(c);
	if (currentNode == -1)
		currentNode = tree.Leaves(ESCAPE);
	while (currentNode != ROOT)
	{
		if ((currentNode & 1) == 0)
			code |= currentBit;
		currentBit <<= 1;
		codeSize++;
		currentNode = tree.Nodes(currentNode).parent;
	}
	OutputBits(file, code, codeSize);
	if (tree.Leaves(c) == -1)
	{
		OutputBits(file, (unsigned long)c, 8);
		AddNewNode(c);
	}
}
//-------------------------------------------------------------------------------------
int HuffmanAdapt::DecodeSymbol(BitFile* file)
{
	int currentNode = ROOT;
	while (!tree.Nodes(currentNode).isLeafChild)
	{
		currentNode = tree.Nodes(currentNode).child;
		currentNode += InputBit(file);
	}
	int c = tree.Nodes(currentNode).child;
	if (c == ESCAPE)
	{
		c = (int)InputBits(file, 8);
		AddNewNode(c);
	}
	return c;
}
//-------------------------------------------------------------------------------------
void HuffmanAdapt::UpdateModel(const int c)
{
	if (tree.Nodes(ROOT).weight == MAX_WEIGHT)
		RebuildTree();
	int currentNode = tree.Leaves(c);
	while (currentNode != -1)
	{
		tree.AddWeight(currentNode);
		int newNode = currentNode;
		while (newNode > ROOT)
		{
			if (tree.Nodes(newNode - 1).weight >= tree.Nodes(currentNode).weight)
				break;
			newNode--;
		}
		if (currentNode != newNode)
		{
			SwapNodes(currentNode, newNode);
			currentNode = newNode;
		}
		currentNode = tree.Nodes(currentNode).parent;
	}
}
//-------------------------------------------------------------------------------------
void HuffmanAdapt::RebuildTree()
{
	tree.RebuildTree();
}
//-------------------------------------------------------------------------------------
void HuffmanAdapt::SwapNodes(const int i, const int j)
{
	tree.SwapNodes(i, j);
}
//-------------------------------------------------------------------------------------
void HuffmanAdapt::AddNewNode(const int c)
{
	tree.AddNewNode(c);
}
//-------------------------------------------------------------------------------------
void HuffmanAdapt::CompressFile(FILE* file, BitFile* compressedFile)
{
	tree.InitializeTree();
	int c = 0;
	while ((c = getc(file)) != EOF)
	{
		EncodeSymbol(c, compressedFile);
		UpdateModel(c);
	}
	EncodeSymbol(END_OF_STREAM, compressedFile);
}
//-------------------------------------------------------------------------------------
void HuffmanAdapt::ExpandFile(FILE* newFile, BitFile* compressedFile)
{
	tree.InitializeTree();
	int c = 0;
	while ((c = DecodeSymbol(compressedFile)) != END_OF_STREAM)
	{
		if (putc(c, newFile) == EOF)
			ArchiverForm->Error("Error writing to file\n");
		UpdateModel(c);
	}
}
//-------------------------------------------------------------------------------------
unsigned HuffmanAdapt::Compress(std::string file, std::string archive)
{
	BitFile* output = OpenOutputFile(archive.c_str());
	FILE* input = fopen(file.c_str(), "rb");
	CompressFile(input, output);
	CloseOutputFile(output);
	fclose(input);
	return FileSize(archive);
}
//-------------------------------------------------------------------------------------
void HuffmanAdapt::Decompress(std::string archive, std::string file)
{
	FILE* output = fopen(file.c_str(), "wb");
	BitFile* input = OpenInputFile(archive.c_str());
	ExpandFile(output, input);
	CloseInputFile(input);
	fclose(output);
}
//-------------------------------------------------------------------------------------
