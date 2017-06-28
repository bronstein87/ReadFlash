//---------------------------------------------------------------------------
#include <Vcl.Forms.hpp>
#ifndef FragmentScrollBoxH
#define FragmentScrollBoxH

class PACKAGE FragmentScrollBox : public TScrollBox
{
__published:
	__property TNotifyEvent OnVerticalScroll =
	{read = FOnVerticalScroll, write = FOnVerticalScroll};
	__property TNotifyEvent OnHorizontalScroll =
	{read = FOnHorizontalScroll, write = FOnHorizontalScroll};

public:
	__fastcall FragmentScrollBox(TComponent* Owner);
	private:
	TNotifyEvent FOnVerticalScroll;
	TNotifyEvent FOnHorizontalScroll;

private:
	void __fastcall WMHScroll(TMessage& Msg);
	void __fastcall WMVScroll(TMessage& Msg);

public:
	BEGIN_MESSAGE_MAP
	MESSAGE_HANDLER(WM_HSCROLL, TMessage, WMHScroll)
	MESSAGE_HANDLER(WM_VSCROLL, TMessage, WMVScroll)
	END_MESSAGE_MAP(TScrollBox)
};

//---------------------------------------------------------------------------
#endif
