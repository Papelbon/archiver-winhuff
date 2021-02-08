//-------------------------------------------------------------------------------------

#pragma hdrstop

#include "ArchiverCMD.h"
#include "MainForm.h"
//-------------------------------------------------------------------------------------
#pragma package(smart_init)
//-------------------------------------------------------------------------------------
void ArchiverCMD::GetInfo(std::vector<std::string> &files, std::string archivePath,
	bool skip, unsigned &filesSize)
{
	char byte[1];
	std::basic_string<char> sInfo = "";
	filesSize = 0;

	remove((archivePath + "\\Info.txt").c_str());
	FILE *info = fopen((archivePath + "\\Info.txt").c_str(), "a+");
	int bytesSize=0;
	for(std::vector<std::string>::iterator it = files.begin(); it != files.end(); ++it)
	{
		FILE *file = fopen((*it).c_str(), "rb");
		if(!file)
		{
			if(skip) continue;
			else break;
		}

		fseek(file, 0, SEEK_END);
		int size = ftell(file);
		filesSize += size;

		std::string fname = GetFileName(*it);

		char *mSize = new char[Digits(size)];
		itoa(size, mSize, 10);
		fclose(file);

		bytesSize += Digits(size);
		bytesSize += strlen(fname.c_str());


		sInfo.append(mSize);
		sInfo.append("||");
		sInfo.append(fname);
		sInfo.append("||");

		delete [] mSize;
	}

	bytesSize = sInfo.size() + 2;
	char *bBuff = new char[Digits(bytesSize)];
	itoa(bytesSize, bBuff, 10);

	if(Digits(bytesSize) < 5)
		fputs(std::string(5 - Digits(bytesSize), '0').c_str(), info);

	fputs(bBuff, info);
	fputs("||", info);
	fputs(sInfo.c_str(), info);

	fclose(info);
	delete [] bBuff;
}
//-------------------------------------------------------------------------------------
bool ArchiverCMD::Archive(std::vector<std::string> &files, std::string archivePath,
	std::string archiveName, std::string format, bool skip)
{
	unsigned filesSize = 0;
	unsigned archiveSize = 0;
	bool succes = false;
	long long start = TimerInit();
	String ArchivingInfo = TimeToStr(GetTime()) + "\nArchiving ...\n";
	String Files = "";

	// archiving block
	char byte[1];
	GetInfo(files, archivePath, skip, filesSize);

	std::string archive = archivePath + "\\" + archiveName;

	FILE *file;
	FILE *main = fopen((archive).c_str(), "wb");
	FILE *info = fopen((archivePath + "\\Info.txt").c_str(), "rb");

	while(!feof(info)) if(fread(byte, 1, 1, info)==1) fwrite(byte, 1, 1, main);

	fclose(info);
	remove((archivePath + "\\Info.txt").c_str());

	for(std::vector<std::string>::iterator it = files.begin(); it != files.end(); ++it)
	{
		file = fopen((*it).c_str(), "rb");
		if(!file)
		{
			ArchiverForm->Error((*it).c_str() + String(" not found\n"));
			if (skip)
				continue;
			else
			{
				succes = false;
				break;
			}
		}

		while(!feof(file)) if(fread(byte, 1, 1, file) == 1) fwrite(byte, 1, 1, main);

		String FileInfo = (*it).c_str();
		Files += FileInfo + "\n";
		FileInfo += " added to archive\n";
		ArchivingInfo += FileInfo;

		fclose(file);
		succes = true;
	}
	fclose(main);

	if (succes)
	{
		// compressing block
		archiveSize = huffCode.Compress(archive, archive + format);
		long long time = TimerDiff(start);

		// result block
		String FilesSize = "Source files size: " + UIntToStr(filesSize) + " bytes";
		String ArchiveSize = "Archive size: " + UIntToStr(archiveSize) + " bytes";
		double K = (double)archiveSize * 100 / filesSize;
		String Ratio = FloatToStrF(K, ffGeneral, 7, 5);
		String CompressionRatio = "Compression ratio: " + Ratio + "%";
		String ElapsedTime =  "Elapsed time: " + IntToStr(time) + " msec";

		ArchivingInfo += FilesSize + "\n" + ArchiveSize + "\n" + CompressionRatio + "\n"+
			ElapsedTime + "\n";

		ArchiverForm->HistoryRichEdit->Lines->Add(ArchivingInfo);
		ArchiverForm->ArchiverLogs(Files, FilesSize, ArchiveSize, CompressionRatio,
			Ratio, ElapsedTime);
	}
	remove(archive.c_str());

	return succes;
}
//-------------------------------------------------------------------------------------
int ArchiverCMD::Digits(double number)
{
	int i;
	for (i = 0; number > 10; ++i, number /= 10);
	return i + 1;
}
//-------------------------------------------------------------------------------------
long long ArchiverCMD::TimerInit()
{
	long long initCounter;
	QueryPerformanceCounter((LARGE_INTEGER*)&initCounter);
	return initCounter;
}
//-------------------------------------------------------------------------------------
double ArchiverCMD::TimerDiff(long long initCounter)
{
	long long freq, diffCounter;
	QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
	QueryPerformanceCounter((LARGE_INTEGER*)&diffCounter);
	return (diffCounter-initCounter) * 1000.0 / freq;
}
//-------------------------------------------------------------------------------------
bool ArchiverCMD::Unarchive(std::vector<std::string> &archives, std::string extractionPath,
	bool skip)
{
	long long start = TimerInit();
	String UnarchivingInfo = TimeToStr(GetTime()) + "\nUnarchiving ...\n";

	for(std::vector<std::string>::iterator it = archives.begin(); it != archives.end(); ++it)
	{
		FILE *archiveCompressed = fopen((*it).c_str(), "rb");
		if(!archiveCompressed)
		{
			ArchiverForm->Error((*it).c_str() + String(" not found\n"));
			if (skip) continue;
			else return false;
		}
		fclose(archiveCompressed);
	}

	for(std::vector<std::string>::iterator it = archives.begin(); it != archives.end(); ++it)
	{
		FILE *archiveCompressed = fopen((*it).c_str(), "rb");
		if(!archiveCompressed) continue;
		fclose(archiveCompressed);

		std::string tempArchive = extractionPath + "\\" + GetFileName(*it) + ".decompressed";

		huffCode.Decompress(*it, tempArchive);

		FILE *archiveDecompressed = fopen(tempArchive.c_str(), "rb");

		char infoBlockSize[5];
		fread(infoBlockSize, 1, 5, archiveDecompressed);
		int size = atoi(infoBlockSize);

		char *infoBlock = new char[size];
		fread(infoBlock, 1, size, archiveDecompressed);

		std::vector<std::string> tokens;
		char *tok = strtok(infoBlock, "||");
		int toks = 0;
		while(tok)
		{
			if(strlen(tok) == 0) break;
			tokens.push_back(tok);
			tok = strtok(NULL,"||");
			toks++;
		}

		if(toks % 2 == 1 ) --toks;
		int files = toks / 2;

		char byte[1];

		for(int i = 0; i < files; ++i)
		{
			const char* size = tokens[i * 2].c_str();
			const char* name = tokens[i * 2 + 1].c_str();
			char fullPath[255];
			strcpy(fullPath, (extractionPath + "\\").c_str());
			strcat(fullPath, name);
			int sz = atoi(size);
			UnarchivingInfo += String(name) + " extracted to " + extractionPath.c_str() + "\n";
			FILE *current = fopen(fullPath, "wb");
			for(int j = 1; j <= sz; ++j)
			{
				if(fread(byte, 1, 1, archiveDecompressed) == 1)
					fwrite(byte, 1, 1, current);
			}

			fclose(current);
		}

		delete [] infoBlock;
		fclose(archiveDecompressed);
		remove(tempArchive.c_str());
	}

	long long time = TimerDiff(start);

	String ElapsedTime =  "Elapsed time: " + IntToStr(time) + " msec";
	UnarchivingInfo += ElapsedTime + "\n";
	ArchiverForm->HistoryRichEdit->Lines->Add(UnarchivingInfo);

	return true;
}
//-------------------------------------------------------------------------------------
std::string ArchiverCMD::GetFileName(std::string path)
{
	return path.substr(path.find_last_of("\\") + 1, path.size());
}
//-------------------------------------------------------------------------------------
