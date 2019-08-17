//---------------------------------------------------------------------------

#ifndef LedSelectH
#define LedSelectH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include "Unit1.h"

struct SEL_STRUCT_ICON_DATA
{
    int px;
    int py;
    int ux;
    int uy;
};


//---------------------------------------------------------------------------
class TLedSel : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);

    void __fastcall IconClicks(TObject *Sender);//ͼ�걻����

private:	// User declarations
public:		// User declarations

//�û�����ĵ�ַ
struct
{
    int *px;//Ӳ����ַ
    int *py;
    int *ux;//�û���ַ
    int *uy;

    TImage *icon[128];
    struct SEL_STRUCT_ICON_DATA icondata[128];
}ParaData;






void __fastcall TLedSel::DispLedInfo(void);//��ʾLEDѡ���



	__fastcall TLedSel(TComponent* Owner,TMainForm *Mf,int *px,int *py,int *ux,int *uy);
};
//---------------------------------------------------------------------------
extern PACKAGE TLedSel *LedSel;
//---------------------------------------------------------------------------
#endif
