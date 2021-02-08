//---------------------------------------------------------------------------

#ifndef CreateArchiveFormH
#define CreateArchiveFormH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.FileCtrl.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.Outline.hpp>
#include <Vcl.Samples.DirOutln.hpp>
#include "cdiroutl.h"
#include <Vcl.Imaging.pngimage.hpp>
#include <vector>
#include "ArchiverCMD.h"
//---------------------------------------------------------------------------
class TCompressionForm : public TForm
{
__published:	// IDE-managed Components
	TEdit *ArchiveNameEdit;
	TLabel *ArchiveNameLabel;
	TLabel *ArchiveFormatLabel;
	TRadioButton *DefaultRadioBtn;
	TRadioButton *HufRadioBtn;
	TLabel *ArchivingOptionsLabel;
	TCheckBox *DelFileCheckBox;
	TLabel *ArchiveDirLabel;
	TDirectoryListBox *DirectoryListBox;
	TImage *OkImg;
	TImage *CancelImg;
	TEdit *ArchiveDirEdit;
	TLabel *SelectedFilesLabel;
	TListBox *SelectedFilesListBox;
	TDriveComboBox *DriveComboBox;
	TCheckBox *SkipFilesCheckBox;
	void __fastcall CancelImgClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall DirectoryListBoxChange(TObject *Sender);
	void __fastcall OkImgClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TCompressionForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TCompressionForm *CompressionForm;
//---------------------------------------------------------------------------
#endif
