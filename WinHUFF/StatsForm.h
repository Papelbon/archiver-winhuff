//---------------------------------------------------------------------------

#ifndef StatsFormH
#define StatsFormH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>

#define MAX_LOGS 15
#define LOG_FILE "WinHUFFLogs.txt"
#define RATIO_LOG_FILE "WinHUFFRatioLogs.txt"
//---------------------------------------------------------------------------
class TStatisticsForm : public TForm
{
__published:	// IDE-managed Components
	TRichEdit *LogsRichEdit;
	TImage *GraphImage;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	    // User declarations
	int GetElementLen();
	float *ratioStats;
	void DrawCoordinateLines();
	void DrawStats();
	void ClearGraphRect();
	void ReDrawStats();
public:		    // User declarations
	__fastcall TStatisticsForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TStatisticsForm *StatisticsForm;
//---------------------------------------------------------------------------
#endif
