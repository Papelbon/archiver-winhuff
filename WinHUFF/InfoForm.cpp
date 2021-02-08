//-------------------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "InfoForm.h"
//-------------------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TAboutForm *AboutForm;
//-------------------------------------------------------------------------------------
__fastcall TAboutForm::TAboutForm(TComponent* Owner)
	: TForm(Owner)
{
}
//-------------------------------------------------------------------------------------
void __fastcall TAboutForm::GitHubLinkLabelLinkClick(TObject *Sender,
	const UnicodeString Link, TSysLinkType LinkType)
{
    if (LinkType == sltURL)
        ::ShellExecute(Handle, NULL, Link.c_str(), NULL, NULL, SW_SHOWNORMAL);
}
//-------------------------------------------------------------------------------------

