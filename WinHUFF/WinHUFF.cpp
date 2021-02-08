//-------------------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
#include <tlhelp32.h>
//-------------------------------------------------------------------------------------
#include <Vcl.Styles.hpp>
#include <Vcl.Themes.hpp>
USEFORM("StatsForm.cpp", StatisticsForm);
USEFORM("MainForm.cpp", ArchiverForm);
USEFORM("CreateArchiveForm.cpp", CompressionForm);
USEFORM("ExtractArchiveForm.cpp", DecompressionForm);
USEFORM("InfoForm.cpp", AboutForm);
//-------------------------------------------------------------------------------------
bool IsProcessRunning(const TCHAR* const executableName);
//-------------------------------------------------------------------------------------

int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	try
	{
		Application->Initialize();
		Application->MainFormOnTaskBar = true;

		// default style: Windows
		// to change default style type this: TStyleManager::TrySetStyle("Windows10");

		Application->Title = "WinHUFF";

		Application->CreateForm(__classid(TArchiverForm), &ArchiverForm);
		Application->CreateForm(__classid(TAboutForm), &AboutForm);
		Application->CreateForm(__classid(TCompressionForm), &CompressionForm);
		Application->CreateForm(__classid(TDecompressionForm), &DecompressionForm);
		Application->CreateForm(__classid(TStatisticsForm), &StatisticsForm);
		Application->Run();
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	catch (...)
	{
		try
		{
			throw Exception("");
		}
		catch (Exception &exception)
		{
			Application->ShowException(&exception);
		}
	}
	return 0;
}
//-------------------------------------------------------------------------------------
bool IsProcessRunning(const TCHAR* const executableName) {
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    const auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    if (!Process32First(snapshot, &entry)) {
        CloseHandle(snapshot);
        return false;
    }

    do {
        if (!_tcsicmp(entry.szExeFile, executableName)) {
            CloseHandle(snapshot);
            return true;
        }
    } while (Process32Next(snapshot, &entry));

    CloseHandle(snapshot);
    return false;
}
//-------------------------------------------------------------------------------------
