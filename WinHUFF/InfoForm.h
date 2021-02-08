//-------------------------------------------------------------------------------------

#ifndef InfoFormH
#define InfoFormH
//-------------------------------------------------------------------------------------
#include <System.Classes.hpp>

#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Imaging.pngimage.hpp>
#include <Vcl.Graphics.hpp>
//-------------------------------------------------------------------------------------
class TAboutForm : public TForm
{
__published:
	TImage *ArchiverImg;
	TLabel *TitleLabel;
	TLabel *VersionLabel;
	TLabel *DeveloperLabel;
	TLabel *YearLabel;
	TLabel *GitHubLabel;
	TLinkLabel *GitHubLinkLabel;
	void __fastcall GitHubLinkLabelLinkClick(TObject *Sender, const UnicodeString Link, TSysLinkType LinkType);

private:
public:
	__fastcall TAboutForm(TComponent* Owner);
};
//-------------------------------------------------------------------------------------
extern PACKAGE TAboutForm *AboutForm;
//-------------------------------------------------------------------------------------
#endif

