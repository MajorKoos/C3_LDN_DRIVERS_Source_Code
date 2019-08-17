#pragma hdrstop

#include "MyListView.h"

#pragma package(smart_init)

TMainForm *MForm;

MyListView::MyListView(Classes::TComponent *AOwner, TWinControl *Parent, TMainForm *Mf, int Left, int Top, int Width, int Height)//构造函数
{
	memset(&ListData, 0, sizeof(ListData));
	MForm = Mf; //主窗口句柄
	ListData.ScrollBox.AOwner = AOwner; //父实例
	ListData.ScrollBox.Top = Top;
	ListData.ScrollBox.Left = Left;
	ListData.ScrollBox.Width = Width;
	ListData.ScrollBox.Height = Height;

	ListData.ScrollBox.SBox = new TScrollBox(AOwner); //创建新的对象
	ListData.ScrollBox.SBox->Parent = Parent; //粘附对象
	ListData.ScrollBox.SBox->Top = Top;
	ListData.ScrollBox.SBox->Left = Left;
	ListData.ScrollBox.SBox->Height = Height;
	ListData.ScrollBox.SBox->Width = Width;

	ListData.ScrollBox.SBox->HorzScrollBar->Visible = false; //禁止显示滚动条
	ListData.ScrollBox.SBox->VertScrollBar->Visible = false;
	ListData.ScrollBox.SBox->BorderStyle = bsNone; //不显示边框
	ListData.ScrollBox.SBox->OnMouseWheel = SBoxMouseWheel;

	ListData.IsFirstRun = 1;
}

//----------------------------------------------------------------------------
MyListView::~MyListView(void)//析构函数
{

	delete[]ListData.ColWidth;
	delete[]ListData.RowHeight;
	for(int i = 0; i < ListData.RowNum; i++)
	{
		delete ListData.Table.RowCaption[i];
	}

	for(int i = 0; i < ListData.ColNum; i++)
	{
		delete[]ListData.Table.CellStyle.RowStyle[i];
	}
	delete[]ListData.Table.CellStyle.RowStyle;

	if(ListData.Image != NULL)
	{
		delete ListData.Image;
	}
	if(ListData.VScroll != NULL)
	{
		delete ListData.VScroll;
	}
	if(ListData.HScroll != NULL)
	{
		delete ListData.HScroll;
	}
	delete ListData.ScrollBox.SBox;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void __fastcall MyListView::SetColNum(int Num)//设置有多少列
{
	ListData.ColNum = Num;
	ListData.ColWidth = new int[ListData.ColNum];
	memset(ListData.ColWidth, 0, ListData.ColNum *sizeof(int));
}

//----------------------------------------------------------------------------
void __fastcall MyListView::SetRowNum(int Num)//设置有多少行
{
	ListData.RowNum = Num;
	ListData.RowHeight = new int[ListData.RowNum];
	memset(ListData.RowHeight, 0, ListData.RowNum *sizeof(int));
}

//----------------------------------------------------------------------------
void __fastcall MyListView::SetHeadHeight(int Height)//设置表头有多高
{
	ListData.TableHead.Height = Height;
}

//----------------------------------------------------------------------------
void __fastcall MyListView::SetHeadFontSize(int Size)//表头的字体大小
{
	ListData.TableHead.FontSize = Size;
}

//----------------------------------------------------------------------------
void __fastcall MyListView::SetHeadBkColor(TColor Color)//设置表格内的背景颜色
{
	ListData.TableHead.BkColor = Color;
}

//----------------------------------------------------------------------------
void __fastcall MyListView::SetHeadFontColor(TColor Color)//设置表格内的字体颜色
{
	ListData.TableHead.FontColor = Color;
}

//----------------------------------------------------------------------------
void __fastcall MyListView::SetHeadPanColor(TColor Color)//设置表格内的画笔颜色
{
	ListData.TableHead.PenColor = Color;
}

//----------------------------------------------------------------------------
void __fastcall MyListView::SetColWidth(int nCol, int Width)
	//设置第几列的宽度,nCol = -1，表示设置所有COL的宽度相同
{
	int i;
	if(nCol == -1)//设置全部的大小
	{
		for(i = 0; i < ListData.ColNum; i++)
		{
			ListData.ColWidth[i] = Width;
		}
	}
	else
	{
		if(nCol < ListData.ColNum)
		{
			ListData.ColWidth[nCol] = Width;
		}
	}
}

//----------------------------------------------------------------------------
void __fastcall MyListView::SetRowHeight(int nRow, int Height)
	//设置第几行的高度,nRow = -1,表示所有的ROW的高度相同
{
	int i;
	if(nRow == -1)//设置全部的大小
	{
		for(i = 0; i < ListData.RowNum; i++)
		{
			ListData.RowHeight[i] = Height;
		}
	}
	else
	{
		if(nRow < ListData.RowNum)
		{
			ListData.RowHeight[nRow] = Height;
		}
	}
}

//----------------------------------------------------------------------------
void __fastcall MyListView::SetTableFontSize(int Size)//表格内的字体大小
{
	ListData.Table.FontSize = Size;
}

//----------------------------------------------------------------------------
void __fastcall MyListView::SetTableBkColor(TColor Color)//设置表格内的背景颜色
{
	ListData.Table.NormalColor.BkColor = Color;
}

//----------------------------------------------------------------------------
void __fastcall MyListView::SetTableFontColor(TColor Color)//设置表格内的字体颜色
{
	ListData.Table.NormalColor.FontColor = Color;
}

//----------------------------------------------------------------------------
void __fastcall MyListView::SetTablePanColor(TColor Color)//设置表格内的画笔颜色
{
	ListData.Table.NormalColor.PenColor = Color;
}

//----------------------------------------------------------------------------
void __fastcall MyListView::SetTableSelectBkColor(TColor Color)
	//设置表格内选中的背景颜色
{
	ListData.Table.SelectColor.BkColor = Color;
}

//----------------------------------------------------------------------------
void __fastcall MyListView::SetTableSelectFontColor(TColor Color)
	//设置表格选中内的字体颜色
{
	ListData.Table.SelectColor.FontColor = Color;
}

//----------------------------------------------------------------------------
void __fastcall MyListView::SetTableSelectPenColor(TColor Color)
	//设置表格内选中的画笔颜色
{
	ListData.Table.SelectColor.PenColor = Color;
}

//----------------------------------------------------------------------------
void __fastcall MyListView::SetOnMouseDown(void __fastcall(*OnMouseDown)(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y,
	int nCol, int nRow))//设置鼠标按下的事件处理句柄
{
	ListData.TableEvent.OnMouseDown = OnMouseDown;
}

//----------------------------------------------------------------------------
void __fastcall MyListView::ImageOnMouseLeave(TObject *Sender)
	//鼠标离开IMAGE的事件
{
	MForm->Panel10->SetFocus();
}

//----------------------------------------------------------------------------
void __fastcall MyListView::ImageOnMouseEnter(TObject *Sender)
	//鼠标进入Image的事件
{
	ListData.ScrollBox.SBox->SetFocus(); //将焦点移动到滚动框上
}

//----------------------------------------------------------------------------
void __fastcall MyListView::SBoxMouseWheel(TObject *Sender, TShiftState Shift, int WheelDelta, const TPoint &MousePos, bool &Handled)
{

	int ScrollSize = ListData.ScrollBox.SBox->Height - ListData.Image->Height;

	if(ListData.Image->Width <= ListData.ScrollBox.SBox->Width && ListData.Image->Height <= ListData.ScrollBox.SBox->Height)
	{
		return;
	}

	if(WheelDelta < 0)//向上滚动
	{
		//计算是否滚动到底
		if((ListData.Image->Top - LST_DEF_SCROLL_PX) > ScrollSize)
		{
			ListData.Image->Top -= LST_DEF_SCROLL_PX; //未滚动到位置，继续向上滚动
		}
		else
		{
			//如果滚动之后，超出了范围
			ListData.Image->Top = ScrollSize;
		}
	}
	else //向下滚动
	{

		if((ListData.Image->Top + LST_DEF_SCROLL_PX) < 0)
		{
			ListData.Image->Top += LST_DEF_SCROLL_PX;
		}
		else
		{
			ListData.Image->Top = 0;
		}
	}
}

//----------------------------------------------------------------------------
void __fastcall MyListView::ImageOnMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
	//鼠标按下事件
	int nCol, nRow;
	GetCellPoint(&nCol, &nRow, X, Y); //计算点击的是哪个单元格
	if(Button == mbLeft)//左键点击
	{
		//恢复上次的
		if(nCol == 0 && nRow >= 0 || nRow == 0 && nCol >= 0)//点击在表头上
		{
			PaintDrawCell(ListData.Table.CellStyle.LastSelectnCol, ListData.Table.CellStyle.LastSelectnRow, 1,
				ListData.Table.CellStyle.LastSelectFontStyle,
				ListData.Table.RowCaption[ListData.Table.CellStyle.LastSelectnRow]->Strings[ListData.Table.CellStyle.LastSelectnCol]);
			ListData.IsFirstRun = 1;
			return;
		}

		if(ListData.IsFirstRun == false)
		{

			PaintDrawCell(ListData.Table.CellStyle.LastSelectnCol, ListData.Table.CellStyle.LastSelectnRow, 1,
				ListData.Table.CellStyle.LastSelectFontStyle,
				ListData.Table.RowCaption[ListData.Table.CellStyle.LastSelectnRow]->Strings[ListData.Table.CellStyle.LastSelectnCol]);
		}
		ListData.IsFirstRun = false;
		//画出本次的
		PaintDrawCell(nCol, nRow, 2, ListData.Table.CellStyle.RowStyle[nRow][nCol], ListData.Table.RowCaption[nRow]->Strings[nCol]);
		//保存本次的坐标
		ListData.Table.CellStyle.LastSelectnCol = nCol;
		ListData.Table.CellStyle.LastSelectnRow = nRow;
		ListData.Table.CellStyle.LastSelectFontStyle = ListData.Table.CellStyle.RowStyle[nRow][nCol];
	}

	ListData.TableEvent.OnMouseDown(Sender, Button, Shift, X, Y, nCol, nRow); //调用回调
}

//----------------------------------------------------------------------------
void __fastcall MyListView::Create(void)//根据设置，创建出控件
{
	int i;
	int tmp;

	//申请保存风格的内存
	ListData.Table.CellStyle.RowStyle = new int *[ListData.RowNum]; //申请一堆指针
	for(i = 0; i < ListData.RowNum; i++)
	{
		ListData.Table.CellStyle.RowStyle[i] = new int[ListData.ColNum];
	}

	//创建image控件
	ListData.Image = new TImage(ListData.ScrollBox.AOwner);

	//计算控件宽度
	tmp = 0;
	for(i = 0; i < ListData.ColNum; i++)
	{
		tmp += ListData.ColWidth[i];
	}
	ListData.Image->Width = tmp;
	//计算控件高度
	tmp = 0;
	for(i = 0; i < ListData.RowNum; i++)
	{
		tmp += ListData.RowHeight[i];
	}
	ListData.Image->Height = tmp;
	ListData.Image->Canvas->Font->Size = ListData.TableHead.FontSize;
	ListData.Image->Canvas->Font->Style = TFontStyles() << fsBold; //粗体
	ListData.Image->Parent = ListData.ScrollBox.SBox;
	ListData.Image->OnMouseEnter = ImageOnMouseEnter; //事件设置
	ListData.Image->OnMouseLeave = ImageOnMouseLeave;
	ListData.Image->OnMouseDown = ImageOnMouseDown; //重要事件

	//画出背景
	PaintRectangle(0, 0, ListData.Image->Width, ListData.Image->Height, ListData.Table.NormalColor.BkColor, ListData.Table.NormalColor.PenColor);

	//生成每一行的数据
	ListData.Table.RowCaption = new TStringList *[ListData.RowNum];

	for(i = 0; i < ListData.RowNum; i++)
	{
		ListData.Table.RowCaption[i] = new TStringList();
		//生成每一列的数据
		for(int z = 0; z < ListData.ColNum; z++)
		{
			ListData.Table.RowCaption[i]->Add(""); //初始值为空
		}
	}
}

//----------------------------------------------------------------------------
void __fastcall MyListView::PaintBox(int x, int y, int width, int height, TColor PenColor)//画一个方框
{
	width-- ;
	height-- ;
	ListData.Image->Canvas->Pen->Color = PenColor;
	//画上横线
	ListData.Image->Canvas->MoveTo(x, y);
	ListData.Image->Canvas->LineTo(x + width + 1, y);

	//画下横线
	ListData.Image->Canvas->MoveTo(x, y + height);
	ListData.Image->Canvas->LineTo(x + width + 1, y + height);

	//画左边竖线
	ListData.Image->Canvas->MoveTo(x, y);
	ListData.Image->Canvas->LineTo(x, y + height + 1);

	//画右边竖线
	ListData.Image->Canvas->MoveTo(x + width, y);
	ListData.Image->Canvas->LineTo(x + width, y + height + 1);
}

//----------------------------------------------------------------------------
void __fastcall MyListView::PaintRectangle(int x, int y, int width, int height, TColor BkColor, TColor PenColor)
	//画一个填充的方框
{
	ListData.Image->Canvas->Brush->Color = BkColor;
	ListData.Image->Canvas->Pen->Color = PenColor;
	ListData.Image->Canvas->Rectangle(x, y, width, height);
}

//----------------------------------------------------------------------------
void __fastcall MyListView::PaintDrawCell(int nCol, int nRow, int CellStyle, int StrStyle, String Str)//画一个单元格
{
	int x, y, width, height;
	int i;
	int FontW, FontH;
	int Fontx, Fonty;

	if(nCol >= ListData.ColNum)
	{
		return;
	}
	if(nRow >= ListData.RowNum)
	{
		return;
	}

	if(CellStyle == 0)//表头
	{
		ListData.Image->Canvas->Brush->Color = ListData.TableHead.BkColor;
		ListData.Image->Canvas->Pen->Color = ListData.TableHead.PenColor;
		ListData.Image->Canvas->Font->Color = ListData.TableHead.FontColor;
	}
	if(CellStyle == 1)//表内容
	{
		ListData.Image->Canvas->Brush->Color = ListData.Table.NormalColor.BkColor;
		ListData.Image->Canvas->Pen->Color = ListData.Table.NormalColor.PenColor;
		ListData.Image->Canvas->Font->Color = ListData.Table.NormalColor.FontColor;
	}
	if(CellStyle == 2)//被选择的表内容
	{
		ListData.Image->Canvas->Brush->Color = ListData.Table.SelectColor.BkColor;
		ListData.Image->Canvas->Pen->Color = ListData.Table.SelectColor.PenColor;
		ListData.Image->Canvas->Font->Color = ListData.Table.SelectColor.FontColor;
	}

	x = 0; //计算X的坐标
	for(i = 0; i < nCol; i++)
	{
		x += ListData.ColWidth[i];
	}
	width = ListData.ColWidth[nCol];

	y = 0; //计算y的坐标
	for(i = 0; i < nRow; i++)
	{
		y += ListData.RowHeight[i];
	}
	height = ListData.RowHeight[nRow];

	ListData.Image->Canvas->Rectangle(x, y, x + width, y + height);

	//保存风格数据
	ListData.Table.CellStyle.RowStyle[nRow][nCol] = StrStyle;

	FontW = ListData.Image->Canvas->TextWidth(Str);
	FontH = ListData.Image->Canvas->TextHeight(Str);
	Fonty = (height / 2) - (FontH / 2) + y; //计算Y坐标
	switch(StrStyle)
	{
		case 0: //左对齐
		{
			Fontx = x + 3;
		} break;
		case 1: //居中
		{
			Fontx = (width / 2) - (FontW / 2) + x; //计算坐标
		} break;
		case 2: //右对齐
		{
			Fontx = (width + x) - (FontW + 3);
		} break;
		default: //其他值为表格的坐标+3
		{
			Fontx = x + 3;
			Fonty = y + 3;
		}
	}
	ListData.Table.RowCaption[nRow]->Strings[nCol] = Str;
	ListData.Image->Canvas->TextOut(Fontx, Fonty, Str);
}

//----------------------------------------------------------------------------
void __fastcall MyListView::GetCellPoint(int *nCol, int *nRow, int x, int y)
	//通过鼠标的坐标，返回表格的坐标
{
	int cx, cy;
	int i;

	cx = x;

	for(i = 0; i < ListData.ColNum; i++)
	{
		cx -= ListData.ColWidth[i];
		if(cx <= 0)
		{
			*nCol = i;
			break;
		}
	}

	cy = y;
	for(i = 0; i < ListData.RowNum; i++)
	{
		cy -= ListData.RowHeight[i];
		if(cy <= 0)
		{
			*nRow = i;
			break;
		}
	}
}

//----------------------------------------------------------------------------
String __fastcall MyListView::GetCellStr(int nCol, int nRow)//获取某单元格的字符串数据
{
	if(nCol >= ListData.ColNum)
	{
		return ("");
	}
	if(nRow >= ListData.RowNum)
	{
		return ("");
	}
	return (ListData.Table.RowCaption[nRow]->Strings[nCol]);
}
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
