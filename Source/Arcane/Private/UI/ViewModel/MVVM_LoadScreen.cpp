// Copyright INVI1998


#include "UI/ViewModel/MVVM_LoadScreen.h"

UMVVM_LoadSlot* UMVVM_LoadScreen::GetLoadSlotViewModelByIndex(int32 Index) const
{
	return LoadSlots.FindChecked(Index);		// 返回加载槽
}
