//-------------------------------------------------------------------------------------

#pragma hdrstop

#include "MainForm.h"
//-------------------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TArchiverForm *ArchiverForm;
//-------------------------------------------------------------------------------------
__fastcall TArchiverForm::TArchiverForm(TComponent* Owner)
	: TForm(Owner)
{
	CompressImg->OnClick = &COMPRESSIONClick;
	DecompressImg->OnClick = &DECOMPRESSIONClick;
}
//-------------------------------------------------------------------------------------
void __fastcall TArchiverForm::FormResize(TObject *Sender)
{
	BrowseImg->Left = ArchiverForm->ClientWidth - BrowseImg->Width - MARGIN;
	DecompressImg->Left = ArchiverForm->ClientWidth - MARGIN -
		BrowseImg->Width - 15 - DecompressImg->Width;
	CompressImg->Left = ArchiverForm->ClientWidth - MARGIN -
		BrowseImg->Width - 15 - DecompressImg->Width - CompressImg->Width - 31;
	PathEdit->Width = ArchiverForm->ClientWidth - MARGIN * 2 - BrowseImg->Width - 15;
	HistoryRichEdit->Width = ArchiverForm->ClientWidth - MARGIN * 2;
	HistoryRichEdit->Height = ArchiverForm->ClientHeight - HistoryRichEdit->Top - 15;
}
//-------------------------------------------------------------------------------------
void __fastcall TArchiverForm::INFOClick(TObject *Sender)
{
	AboutForm->Show();
}
//-------------------------------------------------------------------------------------
void __fastcall TArchiverForm::OPENClick(TObject *Sender)
{
	OpenDialog->Options.Clear();
	OpenDialog->Options << ofFileMustExist << ofHideReadOnly << ofEnableSizing;
	OpenDialog->Title = "Find archive";
	OpenDialog->Filter = "All archives (*.huf)|*.huf";
	if (OpenDialog->Execute())
		PathEdit->Text = OpenDialog->FileName;
}
//-------------------------------------------------------------------------------------
void __fastcall TArchiverForm::EXITClick(TObject *Sender)
{
	ArchiverForm->Close();
}
//-------------------------------------------------------------------------------------
void __fastcall TArchiverForm::COMPRESSIONClick(TObject *Sender)
{
	CompressionForm->SelectedFilesListBox->Clear();

	String Path = PathEdit->Text;

	TRegEx regexSingleFile(R"(^([^"]+)$)", TRegExOptions());
	TMatchCollection regexMatches = regexSingleFile.Matches(Path);

	if (regexMatches.Count > 0)
	{
		String File = regexMatches.Item[0].Value;

		if (FileExists(File))
		{
			CompressionForm->SelectedFilesListBox->Items->Add(File);
			CompressionForm->Show();
		}
		else
			Warning("Unable to perform operation. Next file does not exist:\n" +
				File + "\n");
	}
	else
	{
		TRegEx regexMultiFiles(R"((".+?")+)", TRegExOptions());
		regexMatches = regexMultiFiles.Matches(Path);
		String NotExistFiles = "";
		if (regexMatches.Count > 0) {
			for (int i = 0; i < regexMatches.Count; ++i)
			{
				TMatch regexMatch = regexMatches.Item[i];

				String File(regexMatch.Value.SubString(2, regexMatch.Value.Length() - 2));
				if (FileExists(File))
					CompressionForm->SelectedFilesListBox->Items->Add(File);
				else
					NotExistFiles += File + "\n";
			}

			if (NotExistFiles.Length() > 0)
				Warning("Unable to perform operation. Next files don't exist:\n" +
					NotExistFiles);

			if (CompressionForm->SelectedFilesListBox->Items->Count > 0)
				CompressionForm->Show();
		}
		else
			Warning("No files selected. You need to select files for this operation!\n" );
	}
}
//-------------------------------------------------------------------------------------
void __fastcall TArchiverForm::DECOMPRESSIONClick(TObject *Sender)
{
	DecompressionForm->SelectedArchivesListBox->Clear();

	String Path = PathEdit->Text;

	TRegEx regexSingleFile(R"(^([^"]+)$)", TRegExOptions());
	TMatchCollection regexMatches = regexSingleFile.Matches(Path);

	if (regexMatches.Count > 0)
	{
		String File = regexMatches.Item[0].Value;

		if (FileExists(File))
		{
			if (ExtractFileExt(File) == ".huf") 
			{
				DecompressionForm->SelectedArchivesListBox->Items->Add(File);
				DecompressionForm->Show();
			}
			else
				Warning("Unable to perform operation. Selected file is not an archive\n");
		}
		else
			Warning("Unable to perform operation. Next file does not exist:\n" +
				File + "\n");
	}
	else
	{
		TRegEx regexMultiFiles(R"((".+?")+)", TRegExOptions());
		regexMatches = regexMultiFiles.Matches(Path);
		String NotExistFiles = "";
		String NotArchivedFiles = "";
		if (regexMatches.Count > 0) {
			for (int i = 0; i < regexMatches.Count; ++i)
			{
				TMatch regexMatch = regexMatches.Item[i];

				String File(regexMatch.Value.SubString(2, regexMatch.Value.Length() - 2));
				if (FileExists(File))
				{
					if (ExtractFileExt(File) == ".huf")
						DecompressionForm->SelectedArchivesListBox->Items->Add(File);
					else
						NotArchivedFiles += File + "\n";
				}
				else
					NotExistFiles += File + "\n";
			}

			if (NotExistFiles.Length() > 0)
				Warning("Unable to perform operation. Next files don't exist:\n" +
					NotExistFiles);

			if (NotArchivedFiles.Length() > 0) 
				Warning("Unable to perform operation. Next files are not archived:\n" +
					NotArchivedFiles);	

			if (DecompressionForm->SelectedArchivesListBox->Items->Count > 0)
				DecompressionForm->Show();
		}
		else
			Warning("No files selected. You need to select files for this operation!\n" );
	}
}
//-------------------------------------------------------------------------------------
void __fastcall TArchiverForm::DELClick(TObject *Sender)
{
	String Path = PathEdit->Text;

	TRegEx regexSingleFile(R"(^([^"]+)$)", TRegExOptions());
	TMatchCollection regexMatches = regexSingleFile.Matches(Path);

	if (regexMatches.Count > 0)
	{
		String File = regexMatches.Item[0].Value;

		if (DeleteFile(File))
			HistoryRichEdit->Lines->Add(TimeToStr(GetTime()) + "\n" + "Deletion ...\n" +
				File + " deleted\n");
		else
			Warning("Unable to perform operation.\nNext file does not exist or "
				"access is denied:\n" + File + "\n");
	}
	else
	{
		TRegEx regexMultiFiles(R"((".+?")+)", TRegExOptions());
		regexMatches = regexMultiFiles.Matches(Path);
		String NotExistFiles = "";
		String DeletedFiles = "";
		if (regexMatches.Count > 0) {
			for (int i = 0; i < regexMatches.Count; ++i)
			{
				TMatch regexMatch = regexMatches.Item[i];

				String File(regexMatch.Value.SubString(2, regexMatch.Value.Length() - 2));
				if (DeleteFile(File))
					DeletedFiles += File + " deleted\n";
				else
					NotExistFiles += File + "\n";
			}

			if (NotExistFiles.Length() > 0)
				Warning("Unable to perform operation.\n"
					"Next files don't exist or access is denied:\n" + NotExistFiles);

			if (DeletedFiles.Length() > 0)
				HistoryRichEdit->Lines->Add(TimeToStr(GetTime()) + "\n" +
					"Deletion ...\n" + DeletedFiles);
		}
		else
			Warning("No files selected. You need to select files for this operation!\n" );
	}
}
//-------------------------------------------------------------------------------------
void __fastcall TArchiverForm::STATSClick(TObject *Sender)
{
	StatisticsForm->Show();
}
//-------------------------------------------------------------------------------------
void TArchiverForm::ArchiverLogs(String Files, String FilesSize, String ArchiveSize,
		String CompressionRatio, String Ratio, String ElapsedTime)
{
   //	AnsiString Path = ExtractFilePath(Application->ExeName);
	AnsiString LogFile = LOG_FILE;
	AnsiString RatioLogFile = RATIO_LOG_FILE;

	int logCount = 0;
	bool isCreateNewFiles = true;

	TFileStream *tLogFile;
	TFileStream *tRatioLogFile;
	TStringList *ratioLogList = new TStringList();

	if (FileExists(LogFile) && FileExists(RatioLogFile))
	{
		isCreateNewFiles = false;
		tLogFile = new TFileStream(LogFile, fmOpenReadWrite);
		tRatioLogFile = new TFileStream(RatioLogFile, fmOpenReadWrite);
		ratioLogList->LoadFromStream(tRatioLogFile);
		logCount = ratioLogList->Count;

		if (logCount >= MAX_LOGS)
		{
			isCreateNewFiles = true;

			ratioLogList->Clear();

			delete tLogFile;
			delete tRatioLogFile;

			DeleteFile(LogFile);
			DeleteFile(RatioLogFile);
		}
	}

	if (isCreateNewFiles)
	{
		tLogFile = new TFileStream(LogFile, fmCreate);
		tRatioLogFile = new TFileStream(RatioLogFile, fmCreate);
		logCount = 0;
	}

	logCount++;

	tLogFile->Seek(0, soFromEnd);
	AnsiString Buffer = Files + FilesSize + "\n" + ArchiveSize + "\n" +
		CompressionRatio + "\n" + ElapsedTime + "\n\n";
	tLogFile->Write(Buffer.c_str(), Buffer.Length());


	ratioLogList->Add(Ratio);
	tRatioLogFile->Seek(0, soFromBeginning);
	ratioLogList->SaveToStream(tRatioLogFile);

	delete tLogFile;
	delete tRatioLogFile;
}
//-------------------------------------------------------------------------------------
void TArchiverForm::Error(String info)
{
	String title = "WinHUFF Error";
	MessageBox(NULL, info.w_str(), title.w_str(), MB_OK|MB_ICONERROR);

	HistoryRichEdit->Lines->Add(TimeToStr(GetTime()) + "\n" + title + "\n" + info);
}
//-------------------------------------------------------------------------------------
void TArchiverForm::Warning(String info)
{
	String title = "WinHUFF Warning";
	MessageBox(NULL, info.w_str(), title.w_str(), MB_OK|MB_ICONWARNING);

	HistoryRichEdit->Lines->Add(TimeToStr(GetTime()) + "\n" + title + "\n" + info);
}
//-------------------------------------------------------------------------------------
void __fastcall TArchiverForm::HANDBOOKClick(TObject *Sender)
{
	String Handbook = "The WinHUFF Handbook RU.pdf";
	if (!FileExists(Handbook)) {
		TResourceStream *ptRes = new TResourceStream(
			(int)HInstance, "HANDBOOK", String("PDFFILE").w_str()
		);
		ptRes->SaveToFile(Handbook);
		delete ptRes;
	}

	ShellExecute(NULL, String("open").w_str(), Handbook.w_str(), NULL, NULL, SW_SHOWNORMAL);
}
//-------------------------------------------------------------------------------------
void __fastcall TArchiverForm::BrowseImgClick(TObject *Sender)
{
	OpenDialog->Options.Clear();
	OpenDialog->Options << ofFileMustExist << ofHideReadOnly <<
		ofEnableSizing << ofAllowMultiSelect;
	OpenDialog->Title = "Find files";
	OpenDialog->Filter = "All files (*.*)|*.*";
	if (OpenDialog->Execute())
	{
		if (OpenDialog->Files->Count == 1)
			PathEdit->Text = OpenDialog->FileName;
		else
		{
			String Tmp = "";
			for (int i = 0; i < OpenDialog->Files->Count; i++)
				Tmp += "\"" + OpenDialog->Files->Strings[i] + "\" ";
			PathEdit->Text = Tmp;
		}
	}
}
//-------------------------------------------------------------------------------------
