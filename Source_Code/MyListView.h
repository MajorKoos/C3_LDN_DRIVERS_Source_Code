// ---------------------------------------------------------------------------

#ifndef MyListViewH
#define MyListViewH
// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include "RecvAndSendData.h"
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
#include <Graphics.hpp>
#include <ButtonGroup.hpp>
#include <XPMan.hpp>
#include <Menus.hpp>
#include <Grids.hpp>
#include <CheckLst.hpp>
#include <Dialogs.hpp>
#include <GIFImg.hpp>
#include <jpeg.hpp>
#include <CustomizeDlg.hpp>
#include "Unit1.h"

#define LST_DEF_SCROLL_PX       35          //����������

// ����������
struct LST_STRUCT_SCROLL_BOX_INFO
{
	TScrollBox *SBox; // ������

	Classes::TComponent* AOwner; // �����ھ��

	int Top;
	int Left;
	int Width;
	int Height;
};

// ��ͷ�Ľṹ
struct LST_STRUCT_MY_LIST_VIEW_TABLE_HEAD
{
	int Height; // ��
	int FontSize; // ����Ĵ�С
	TColor BkColor; // ������ɫ
	TColor FontColor; // ������ɫ
	TColor PenColor; // ������ɫ
};

struct LST_STRUCT_MY_LIST_VIEW_TABLE_ONE_ROW
{
	TStringList *Caption; // ÿһ�еı���
	int Height; // ÿһ�еĸ߶�
};

struct LST_STRUCT_MY_LIST_VIEW_TABLE_ONE_ROW_STYLE
{
	int *ColFontStyle; // ÿ���е�������
};

// �����������
struct LST_STRUCT_MY_LIST_VIEW_TABLE_ALL_ROW
{
	TStringList **RowCaption; // ÿһ�е����ݣ����256��
	int FontSize; // ����Ĵ�С

	// ������ʾ����ɫ
	struct
	{
		TColor BkColor; // ������ɫ
		TColor FontColor; // ������ɫ
		TColor PenColor; // ������ɫ
	}

	NormalColor;

	// ѡ�к����ɫ
	struct
	{
		TColor BkColor; // ������ɫ
		TColor FontColor; // ������ɫ
		TColor PenColor; // ������ɫ
	}

	SelectColor;

	struct
	{
		int **RowStyle; // ����ķ��
		int LastSelectnCol; // �ϴ�ѡ���������
		int LastSelectnRow; // �ϴ�ѡ���������
		int LastSelectFontStyle; // �ϴ�ѡ�����������
	}

	CellStyle;
};

struct LST_STRUCT_MY_LIST_VIEW_ALL_DATA
{
	int RowNum; // �ж�����
	int ColNum; // �ж�����
	int *ColWidth; // ÿ�����ж��,��̬��new�������رյ�ʱ��Ҫdelete
	int *RowHeight; // ÿ���еĸ߶�
	TImage *Image; // ��ͼ�Ļ���
	TImage *VScroll; // ���������
	TImage *HScroll; // ���������
	struct LST_STRUCT_SCROLL_BOX_INFO ScrollBox; // �����������
	struct LST_STRUCT_MY_LIST_VIEW_TABLE_HEAD TableHead; // ��ͷ
	struct LST_STRUCT_MY_LIST_VIEW_TABLE_ALL_ROW Table; // �������

	struct
	{
		void __fastcall(*OnMouseDown)(TObject * Sender, TMouseButton Button, TShiftState Shift, int X, int Y, int nCol, int nRow);
	}TableEvent;
    bool IsFirstRun;

};

class MyListView
{

private:
	void __fastcall ImageOnMouseLeave(TObject *Sender); // ����뿪IMAGE���¼�
	void __fastcall ImageOnMouseEnter(TObject *Sender); // ������Image���¼�
	void __fastcall ImageOnMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall SBoxMouseWheel(TObject *Sender, TShiftState Shift, int WheelDelta, const TPoint &MousePos, bool &Handled); // ��������������¼�

public:
	MyListView::MyListView(Classes::TComponent* AOwner, TWinControl *Parent, TMainForm *Mf, int Left, int Top, int Width, int Height); // ���캯��
	MyListView::~MyListView(void); // ��������

	struct LST_STRUCT_MY_LIST_VIEW_ALL_DATA ListData; // ����

	// ��ͷ����
	void __fastcall MyListView::SetColNum(int Num); // �����ж�����
	void __fastcall MyListView::SetRowNum(int Num); // �����ж�����

	void __fastcall MyListView::SetHeadHeight(int Height); // ���ñ�ͷ�ж��
	void __fastcall MyListView::SetHeadFontSize(int Size); // ��ͷ�������С
	void __fastcall MyListView::SetHeadBkColor(TColor Color); // ���ñ���ڵı�����ɫ
	void __fastcall MyListView::SetHeadFontColor(TColor Color); // ���ñ���ڵ�������ɫ
	void __fastcall MyListView::SetHeadPanColor(TColor Color); // ���ñ���ڵĻ�����ɫ

	void __fastcall MyListView::SetColWidth(int nCol, int Width);
	// ���õڼ��еĿ��,nCol = -1����ʾ��������COL�Ŀ����ͬ
	void __fastcall MyListView::SetRowHeight(int nRow, int Height);
	// ���õڼ��еĸ߶�,nRow = -1,��ʾ���е�ROW�ĸ߶���ͬ

	void __fastcall MyListView::SetTableFontSize(int Size); // ����ڵ������С
	// �������ڵ���ɫ
	void __fastcall MyListView::SetTableBkColor(TColor Color); // ���ñ���ڵı�����ɫ
	void __fastcall MyListView::SetTableFontColor(TColor Color); // ���ñ���ڵ�������ɫ
	void __fastcall MyListView::SetTablePanColor(TColor Color); // ���ñ���ڵĻ�����ɫ

	// ѡ�еĿ����ɫ
	void __fastcall MyListView::SetTableSelectBkColor(TColor Color);
	// ���ñ����ѡ�еı�����ɫ
	void __fastcall MyListView::SetTableSelectFontColor(TColor Color);
	// ���ñ��ѡ���ڵ�������ɫ
	void __fastcall MyListView::SetTableSelectPenColor(TColor Color);
	// ���ñ����ѡ�еĻ�����ɫ

	// �����¼�
	void __fastcall MyListView::SetOnMouseDown(void __fastcall(*OnMouseDown)(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y,
		int nCol, int nRow)); // ������갴�µ��¼�������

	void __fastcall MyListView::Create(void); // �������ã��������ؼ�
	void __fastcall MyListView::PaintBox(int x, int y, int width, int height, TColor PenColor); // ��һ������
	void __fastcall MyListView::PaintRectangle(int x, int y, int width, int height, TColor BkColor, TColor PenColor); // ��һ�����ķ���
	void __fastcall MyListView::PaintDrawCell(int nCol, int nRow, int CellStyle, int StrStyle, String Str); // ��һ����Ԫ��
	void __fastcall MyListView::GetCellPoint(int *nCol, int *nRow, int x, int y); // ͨ���������꣬���ر�������
	String __fastcall MyListView::GetCellStr(int nCol, int nRow);
	// ��ȡĳ��Ԫ����ַ�������

};

#endif
