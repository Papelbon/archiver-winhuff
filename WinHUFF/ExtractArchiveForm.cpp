//-------------------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ExtractArchiveForm.h"
//-------------------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TDecompressionForm *DecompressionForm;
//-------------------------------------------------------------------------------------
__fastcall TDecompressionForm::TDecompressionForm(TComponent* Owner)
	: TForm(Owner)
{
}
//-------------------------------------------------------------------------------------
void __fastcall TDecompressionForm::FormCreate(TObject *Sender)
{
	DestinationDirEdit->Text = DirectoryListBox->Directory;
}
//-------------------------------------------------------------------------------------
void __fastcall TDecompressionForm::CancelImgClick(TObject *Sender)
{
	DecompressionForm->Close();
}
//-------------------------------------------------------------------------------------
void __fastcall TDecompressionForm::DirectoryListBoxChange(TObject *Sender)
{
    DestinationDirEdit->Text = DirectoryListBox->Directory;
}
//-------------------------------------------------------------------------------------
void __fastcall TDecompressionForm::OkImgClick(TObject *Sender)
{
	ArchiverCMD archiver;
	bool skip = false;

	std::vector<std::string> archives;
	for (int i = 0; i < SelectedArchivesListBox->Items->Count; ++i)
		archives.push_back(AnsiString(SelectedArchivesListBox->Items->Strings[i]).c_str());

	std::string extractionPath = AnsiString(DirectoryListBox->Directory).c_str();

	if (SkipArchivesCheckBox->Checked) skip = true;

	bool succes = archiver.Unarchive(archives, extractionPath, skip);

	if (DelArchiveCheckBox->Checked && succes)
		for (int i = 0; i < SelectedArchivesListBox->Items->Count; ++i)
			DeleteFile(SelectedArchivesListBox->Items->Strings[i]);

	DecompressionForm->Close();
}
//-------------------------------------------------------------------------------------

