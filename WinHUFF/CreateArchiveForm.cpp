//-------------------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "CreateArchiveForm.h"
//-------------------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "cdiroutl"
#pragma resource "*.dfm"
TCompressionForm *CompressionForm;
//-------------------------------------------------------------------------------------
__fastcall TCompressionForm::TCompressionForm(TComponent* Owner)
	: TForm(Owner)
{
}
//-------------------------------------------------------------------------------------
void __fastcall TCompressionForm::FormCreate(TObject *Sender)
{
	ArchiveDirEdit->Text = DirectoryListBox->Directory;
}
//-------------------------------------------------------------------------------------
void __fastcall TCompressionForm::CancelImgClick(TObject *Sender)
{
	CompressionForm->Close();
}
//-------------------------------------------------------------------------------------
void __fastcall TCompressionForm::DirectoryListBoxChange(TObject *Sender)
{
	ArchiveDirEdit->Text = DirectoryListBox->Directory;
}
//-------------------------------------------------------------------------------------
void __fastcall TCompressionForm::OkImgClick(TObject *Sender)
{
	ArchiverCMD archiver;
	bool skip = false;

	std::vector<std::string> files;
	for (int i = 0; i < SelectedFilesListBox->Items->Count; ++i)
		files.push_back(AnsiString(SelectedFilesListBox->Items->Strings[i]).c_str());

	if (ArchiveNameEdit->Text == "") ArchiveNameEdit->Text = "archive1";

	std::string archiveName = AnsiString(ArchiveNameEdit->Text).c_str();
	std::string archivePath = AnsiString(DirectoryListBox->Directory).c_str();
	std::string format;
	if (DefaultRadioBtn->Checked || HufRadioBtn->Checked)
		format = ".huf";

	if (SkipFilesCheckBox->Checked) skip = true;

	//if (MultithreadingCheckBox->Checked) { }

	bool succes = archiver.Archive(files, archivePath, archiveName, format, skip);

	if (DelFileCheckBox->Checked && succes)
		for (int i = 0; i < SelectedFilesListBox->Items->Count; ++i)
			DeleteFile(SelectedFilesListBox->Items->Strings[i]);

	CompressionForm->Close();
}
//-------------------------------------------------------------------------------------

