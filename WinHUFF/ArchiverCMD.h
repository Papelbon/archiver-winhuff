//-------------------------------------------------------------------------------------

#ifndef ArchiverCMDH
#define ArchiverCMDH
//-------------------------------------------------------------------------------------
#include "HuffmanAdapt.h"
#include <vector>
//-------------------------------------------------------------------------------------
class ArchiverCMD
{
private:
	HuffmanAdapt huffCode;

	long long TimerInit();
	double TimerDiff(long long initCounter);
	void GetInfo(std::vector<std::string> &files, std::string archivePath,
		bool skip, unsigned &filesSize);
	int Digits(double number);
	std::string GetFileName(std::string path);

public:
	bool Archive(std::vector<std::string> &files, std::string archivePath,
		std::string archiveName, std::string format, bool skip);
	bool Unarchive(std::vector<std::string> &archives, std::string extractionPath,
		 bool skip);
};

#endif
