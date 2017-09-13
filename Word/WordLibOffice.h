#include <Classes.hpp>

void OpenWord(bool Visible);
void CloseWord(void);
void AddDoc(void);
void SaveDoc(AnsiString Name);
void CloseDoc(void);
void AddParagraph(AnsiString Text);
void AddText(AnsiString Text);
void AddPicture(AnsiString Name);
void AddTable(int Nrow, int Ncol);
void SetActiveTable(int num);
void SetCell(int Nrow, int Ncol, AnsiString str);
void SetCellColor(int Nrow, int Ncol, TColor clr);
void UnionCell(int Nrow_1, int Ncol_1, int Nrow_2, int Ncol_2);
void SetTextFormat(float TextSize, long IsBold, long IsItalic,
						long IsUnderline, long TypeAlign);
void SetTextSize(float TextSize);
void SetTextBold(long IsBold);
void SetTextItalic(long IsItalic);
void SetTextUnderline(long IsUnderline);
void SetTextAllign(long TypeAlign);
void AddHeader(AnsiString Text);
void AddFooter(AnsiString Text);
void AddFileName(void);
void AddField(AnsiString Text);

