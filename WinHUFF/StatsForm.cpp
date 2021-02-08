//-------------------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "StatsForm.h"
//-------------------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TStatisticsForm *StatisticsForm;
//-------------------------------------------------------------------------------------
__fastcall TStatisticsForm::TStatisticsForm(TComponent* Owner)
	: TForm(Owner)
{
}
//-------------------------------------------------------------------------------------
void __fastcall TStatisticsForm::FormShow(TObject *Sender)
{
   // 	AnsiString Path = ExtractFilePath(Application->ExeName);
	AnsiString LogFile = LOG_FILE;
	AnsiString RatioLogFile = RATIO_LOG_FILE;

	if (FileExists(LogFile)) {
		TFileStream *tfile = new TFileStream(LogFile, fmOpenRead);
		LogsRichEdit->Lines->LoadFromStream(tfile);
		delete tfile;
	}
	else
	{
		String title = "WinHUFF Stats Warning";
		String text = "Nothing to show. Try to add to archive your first file!";
		MessageBox(NULL, text.w_str(), title.w_str(), MB_OK|MB_ICONWARNING);
	}

	ClearGraphRect();
	DrawCoordinateLines();

	ratioStats = new float[MAX_LOGS] ();
    for (int i = 0; i < MAX_LOGS; i++)
		ratioStats[i] = 100;

	if (FileExists(RatioLogFile))
	{
		TFileStream *tfile = new TFileStream(RatioLogFile, fmOpenReadWrite);
		TStringList *tlist = new TStringList();
		tlist->LoadFromStream(tfile);

		for (int i = 0; i < tlist->Count; i++)
			ratioStats[i] = StrToFloat(tlist->Strings[i]);

		DrawStats();

		delete tfile;
        delete tlist;
	}
}
//-------------------------------------------------------------------------------------
void TStatisticsForm::DrawCoordinateLines()
{
	int elementLen = GetElementLen();
	TCanvas *pCanvas = GraphImage->Canvas;

	pCanvas->MoveTo(20, 10);
	pCanvas->LineTo(26, 20);
	pCanvas->MoveTo(20, 10);
	pCanvas->LineTo(14, 20);
	pCanvas->MoveTo(20, 10);
	pCanvas->LineTo(20, GraphImage->Height - 10);
	pCanvas->LineTo(GraphImage->Width - 10, GraphImage->Height - 10);
	pCanvas->LineTo(GraphImage->Width - 20, GraphImage->Height - 4);
	pCanvas->MoveTo(GraphImage->Width - 10, GraphImage->Height - 10);
	pCanvas->LineTo(GraphImage->Width - 20, GraphImage->Height - 16);

	for (int i = 0, j = 20; i < MAX_LOGS; i++, j += elementLen)
	{
		pCanvas->MoveTo(j, GraphImage->Height - 10);
		pCanvas->LineTo(j, GraphImage->Height - 5);
	}

	pCanvas->Font->Name = "Segoe UI";
	pCanvas->Font->Size = 10;
	LOGFONT lf;
	GetObject(pCanvas->Font->Handle, sizeof(LOGFONT), &lf);
	lf.lfEscapement = 900;
	lf.lfOrientation = 450;
	pCanvas->Font->Handle = CreateFontIndirect(&lf);
	pCanvas->TextOut(0, GraphImage->Height / 2 + 60, "Compression Ratio, %");
}
//-------------------------------------------------------------------------------------
void TStatisticsForm::DrawStats()
{
	int elementLen = GetElementLen();
	TCanvas *pCanvas = GraphImage->Canvas;

	int ratioMax = GraphImage->Height - 40;    // compression ratio: 100% value
	int yBottom = GraphImage->Height - 10;

	pCanvas->Brush->Color = clWhite;
	pCanvas->Pen->Color = (TColor) RGB(0xC8, 0xC8, 0xC8);
	pCanvas->Pen->Style = psDash;

	for (int i = 25, xPoint = 20; i <= 100; i += 25)
	{
		float yPoint = yBottom - ratioMax * i / 100;
		pCanvas->MoveTo(xPoint, yPoint);
		pCanvas->LineTo(GraphImage->Width - 10, yPoint);
	}

	pCanvas->Brush->Color = clBlack;
	pCanvas->Pen->Color = clBlack;
	pCanvas->Pen->Style = psSolid;

	pCanvas->MoveTo(20, GraphImage->Height - 10);
	for (int i = 0, xPoint = 20; i < MAX_LOGS; i++, xPoint += elementLen)
	{
		float yPoint = yBottom - ratioMax * ratioStats[i] / 100;
		pCanvas->LineTo(xPoint, yPoint);
		pCanvas->Ellipse(xPoint - 3, yPoint - 3, xPoint + 3, yPoint + 3);
	}

}
//-------------------------------------------------------------------------------------
void __fastcall TStatisticsForm::FormResize(TObject *Sender)
{
	LogsRichEdit->Height = StatisticsForm->ClientHeight - 40;

	GraphImage->Width = StatisticsForm->ClientWidth - 545;
	GraphImage->Height = StatisticsForm->ClientHeight - 20;
	GraphImage->Picture->Bitmap->SetSize(GraphImage->Width, GraphImage->Height);

	ReDrawStats();
}
//-------------------------------------------------------------------------------------
void TStatisticsForm::ReDrawStats()
{
	ClearGraphRect();
	DrawCoordinateLines();
	DrawStats();
}
//-------------------------------------------------------------------------------------
void TStatisticsForm::ClearGraphRect()
{
	TCanvas *pCanvas = GraphImage->Canvas;
	pCanvas->Brush->Style = bsSolid;
	pCanvas->Brush->Color = clWhite;
	pCanvas->FillRect(pCanvas->ClipRect);
}
//-------------------------------------------------------------------------------------
int TStatisticsForm::GetElementLen()
{
	return (GraphImage->Width - 50) / MAX_LOGS;
}
//-------------------------------------------------------------------------------------
void __fastcall TStatisticsForm::FormClose(TObject *Sender, TCloseAction &Action)
{
	delete ratioStats;
}
//-------------------------------------------------------------------------------------

