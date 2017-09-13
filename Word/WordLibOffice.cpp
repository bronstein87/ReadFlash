#include <Word_2k_srvr.h>
#include "ConstWord.h"

TWordApplication *WordApplication1;
TWordDocument *WordDocument1;
TWordFont *WordFont1;
TablePtr table;
TVariant SaveChanges, Direction; //= wdCollapseEnd;
int NumDoc, NumTab, NumImage, NumPage;
int BoldT=0, ItalicT=0, UnderlineT=0, SizeT=12, TypeAlignT=0;

void SetFont(TVariant _start, TVariant _finish);

void OpenWord(bool Visible)
{
   WordApplication1=new TWordApplication(0);
   WordDocument1=new TWordDocument(0);

   WordApplication1->Connect();   // Установка соединения с сервером
   WordApplication1->set_Visible(true);  // Делаем Word видимым

   NumDoc=0;
}

void CloseWord(void)
{
   WordApplication1->Quit();
   WordApplication1->Disconnect();
}

void AddDoc(void)
{
   NumDoc++;
   NumTab=0;
   NumImage=0;

   WordApplication1->Documents->Add();   // Создаем новый документ
   WordDocument1->ConnectTo(WordApplication1->ActiveDocument); // Связывает WordDocument с активным документом
}

void SaveDoc(AnsiString Name)
{
   if (Name!=NULL) WordDocument1->SaveAs(TVariant(Name));
   else WordDocument1->Save();
}

void CloseDoc(void)
{
   WordDocument1->Close();
   WordDocument1->Disconnect();
}

void AddParagraph(AnsiString Text)
{
int StartCount=WordDocument1->Characters->Count-1;
   WordDocument1->Range(TVariant(WordDocument1->Characters->Count-1), EmptyParam())->
				InsertAfter(TVariant(Text+"\n"));
   SetFont(TVariant(StartCount), EmptyParam());
}

void AddText(AnsiString Text)
{
int StartCount=WordDocument1->Characters->Count-1;
   WordDocument1->Range(TVariant(WordDocument1->Characters->Count-1), EmptyParam())->
				InsertAfter(TVariant(Text));
   SetFont(TVariant(StartCount), EmptyParam());
}

void AddPicture(AnsiString Name)
{
   NumImage++;
   WordDocument1->Range(TVariant(WordDocument1->Characters->Count-1), EmptyParam())->
                InlineShapes->AddPicture(StringToOleStr(Name));
   AddParagraph("");
}

void AddTable(int Nrow, int Ncol)
{
   NumTab++;
   WordDocument1->Tables->Add(WordDocument1->Range(TVariant(WordDocument1->Characters->Count-1) ,EmptyParam()),
						Nrow, Ncol, TVariant(1),TVariant(0));
   table = WordDocument1->Tables->Item(NumTab);
   table->LeftPadding  = 0;//отступы в ячейках слева
   table->RightPadding = 0;//и справа
}

void SetActiveTable (int num)
{
   table = WordDocument1->Tables->Item(num);
}

void SetCell(int Nrow, int Ncol, AnsiString str)
{
long StartPos, EndPos;
   table->Cell(Nrow, Ncol)->Range->InsertAfter( TVariant(str));
   table->Cell(Nrow, Ncol)->Range->get_Start(&StartPos);
   table->Cell(Nrow, Ncol)->Range->get_End(&EndPos);
//   table->Cell(Nrow, Ncol)->Range->Font->Color=clRed;
   SetFont(TVariant(StartPos), TVariant(EndPos));
}

void SetCellColor(int Nrow, int Ncol, TColor clr)
{
  table->Cell(Nrow, Ncol)->Range->Shading->BackgroundPatternColor=clr;
}

void UnionCell(int Nrow_1, int Ncol_1, int Nrow_2, int Ncol_2)
{
   table = WordDocument1->Tables->Item(NumTab);
   table->Cell(Nrow_1,Ncol_1)->Merge(table->Cell(Nrow_2, Ncol_2));
}

void SetTextFormat(float TextSize, long IsBold, long IsItalic, long IsUnderline, long TypeAlign)
{
  BoldT=IsBold;
  ItalicT=IsItalic;
  UnderlineT=IsUnderline;
  SizeT=TextSize;
  TypeAlignT=TypeAlign;
  WordDocument1->Paragraphs->get_Last()->Format->
								set_Alignment(WdParagraphAlignment(TypeAlign));
}
void SetFontName(AnsiString FontName)
{

}
void SetTextBold(long IsBold)
{
	BoldT=IsBold;
}

void SetTextItalic(long IsItalic)
{
	ItalicT=IsItalic;
}

void SetTextUnderline(long IsUnderline)
{
	UnderlineT=IsUnderline;
}

void SetTextSize(float TextSize)
{
	SizeT=TextSize;
}

void SetTextAllign(long TypeAlign)
{
	TypeAlignT=TypeAlign;
}
/*
#define wdSeekMainDocument		0x00
#define wdSeekPrimaryHeader 	0x01
#define wdSeekFirstPageHeader	0x02
#define wdSeekEvenPagesHeader	0x03
#define wdSeekPrimaryFooter		0x04
#define wdSeekFirstPageFooter	0x05
#define wdSeekEvenPagesFooter	0x06
#define wdSeekFootnotes			0x07
#define wdSeekEndnotes			0x08
#define wdSeekCurrentPageHeader 0x09
#define wdSeekCurrentPageFooter 0x0A
*/
void AddHeader(AnsiString Text)
{
	WdHeaderFooterIndex  Index=1;
	HeaderFooter *CurHF;
	WordDocument1->Sections->Item(1)->Headers->Item(Index, &CurHF);
	CurHF->Range->InsertAfter((TVariant)Text);
}

void AddFooter(AnsiString Text)
{
	WdHeaderFooterIndex  Index=1;
	HeaderFooter *CurHF;
	WordRange *WR;
	WordDocument1->Sections->Item(1)->Footers->Item(Index, &CurHF);
//	CurHF->Range->InsertAfter((TVariant)Text);
	CurHF->PageNumbers->Add(TVariant(2), TVariant(1));
	CurHF->get_Range(&WR);
	WR->InsertAfter((TVariant)Text);
//	CurHF->Range->Fields->Add(CurHF->Range,TVariant(0),TVariant("FILENAME  "), TVariant(1));
//	WordDocument1->Fields->Add(WR, WordDocument1->Fields->   wdFieldDate,  TVariant(0),TVariant("FILENAME  "), TVariant(1));
//	int nn=WordDocument1->ComputeStatistics(2);
}

void SetFont(TVariant _start, TVariant _finish)
{
   WordDocument1->Range(_start, _finish)->Font->set_Bold(BoldT);
   WordDocument1->Range(_start, _finish)->Font->set_Italic(ItalicT);
   WordDocument1->Range(_start, _finish)->Font->set_Underline(UnderlineT);
   WordDocument1->Range(_start, _finish)->Font->set_Size(SizeT);
}
void AddFileName(void)
{
//   WordDocument1->Range(TVariant(WordDocument1->Characters->Count-1), EmptyParam())->
//				InsertAfter(TVariant(Text));

//	CurHF->get_Range(&WR);
//	CurHF->Range->Fields->Add(CurHF->Range,TVariant(0),TVariant("FILENAME  "), TVariant(1));
/*
	.Selection.TypeText Text:="Стр. "
	.Selection.Fields.Add Range:=.Selection.Range, Type:=.WdFieldType.wdFieldEmpty, Text:= _
		"PAGE  ", PreserveFormatting:=True
	.Selection.TypeText Text:=" из "
	.Selection.Fields.Add Range:=.Selection.Range, Type:=.WdFieldType.wdFieldEmpty, Text:= _
		"NUMPAGES  ", PreserveFormatting:=True
	.ActiveWindow.ActivePane.View.SeekView = .WdSeekView.wdSeekMainDocument
*/
//	WordDocument1->Fields->Add(WordDocument1->get_r),TVariant(0),  TVariant(0),TVariant(1));
//	int nn=WordDocument1->ComputeStatistics(2);
}

void AddField (AnsiString Text)
{
WordDocument1->Fields->Add
(
// WordDocument1->ActiveWindow->ActivePane->Selection->Range,
WordDocument1->Paragraphs->get_Last()->get_Range(),
(OleVariant)wdFieldFileName,//Empty,
(OleVariant)Text,
(OleVariant)true
);
}
