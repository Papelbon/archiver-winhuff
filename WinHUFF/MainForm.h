//---------------------------------------------------------------------------

#ifndef MainFormH
#define MainFormH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <System.RegularExpressions.hpp>

#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Imaging.pngimage.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.WinXCtrls.hpp>
#include <VCLTee.TeCanvas.hpp>
#include <Vcl.Graphics.hpp>

#include "InfoForm.h"
#include "StatsForm.h"
#include "CreateArchiveForm.h"
#include "ExtractArchiveForm.h"

#define MARGIN 8
//---------------------------------------------------------------------------
class TArchiverForm : public TForm
{
__published:	// IDE-managed Components
	TOpenDialog *OpenDialog;
	TMainMenu *MainMenu;
	TMenuItem *FILE;
	TMenuItem *OPEN;
	TMenuItem *EXIT;
	TMenuItem *COMMANDS;
	TMenuItem *COMPRESSION;
	TMenuItem *DECOMPRESSION;
	TMenuItem *HELP;
	TMenuItem *HANDBOOK;
	TMenuItem *INFO;
	TMenuItem *FILESEP;
	TMenuItem *STATS;
	TMenuItem *COMMANDSSEP;
	TMenuItem *DEL;
	TEdit *PathEdit;
	TLabel *InfoLabel;
	TRichEdit *HistoryRichEdit;
	TImage *CompressImg;
	TImage *DecompressImg;
	TImage *BrowseImg;
	TLabel *SelectInfoLabel;
	void __fastcall INFOClick(TObject *Sender);
	void __fastcall OPENClick(TObject *Sender);
	void __fastcall EXITClick(TObject *Sender);
	void __fastcall COMPRESSIONClick(TObject *Sender);
	void __fastcall DECOMPRESSIONClick(TObject *Sender);
	void __fastcall DELClick(TObject *Sender);
	void __fastcall HANDBOOKClick(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall STATSClick(TObject *Sender);
	void __fastcall BrowseImgClick(TObject *Sender);
private:	    // User declarations

public:		    // User declarations
	// void UnarchiveInformation(__int64 time);
	void Error(String);
	void Warning(String);
    void ArchiverLogs(String, String, String, String, String, String);

	__fastcall TArchiverForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TArchiverForm *ArchiverForm;
//---------------------------------------------------------------------------
#endif
