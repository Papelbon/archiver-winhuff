//---------------------------------------------------------------------------

#ifndef ExtractArchiveFormH
#define ExtractArchiveFormH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.FileCtrl.hpp>
#include <Vcl.Imaging.pngimage.hpp>
#include "ArchiverCMD.h"
//---------------------------------------------------------------------------
class TDecompressionForm : public TForm
{
__published:	// IDE-managed Components
	TLabel *DestinationDirLabel;
	TEdit *DestinationDirEdit;
	TLabel *SelectedArchivesLabel;
	TListBox *SelectedArchivesListBox;
	TImage *OkImg;
	TImage *CancelImg;
	TDirectoryListBox *DirectoryListBox;
	TDriveComboBox *DriveComboBox;
	TLabel *ExtractionOptionsLabel;
	TCheckBox *DelArchiveCheckBox;
	TCheckBox *SkipArchivesCheckBox;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall CancelImgClick(TObject *Sender);
	void __fastcall DirectoryListBoxChange(TObject *Sender);
	void __fastcall OkImgClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TDecompressionForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TDecompressionForm *DecompressionForm;
//---------------------------------------------------------------------------
#endif
