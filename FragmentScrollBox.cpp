//---------------------------------------------------------------------------

#pragma hdrstop

#include "FragmentScrollBox.h"
	__fastcall FragmentScrollBox::FragmentScrollBox(TComponent* Owner):TScrollBox(Owner) { }


	void __fastcall FragmentScrollBox::WMHScroll(TMessage& Msg)
	{
		if (FOnHorizontalScroll != NULL)
		{
			FOnHorizontalScroll(this);
		}
	TScrollBox::Dispatch(&Msg);
	}

	void __fastcall FragmentScrollBox::WMVScroll(TMessage& Msg)
	{
		if (FOnVerticalScroll != NULL)
		{
			FOnVerticalScroll(this);
		}
	TScrollBox::Dispatch(&Msg);
	}


//---------------------------------------------------------------------------
#pragma package(smart_init)
