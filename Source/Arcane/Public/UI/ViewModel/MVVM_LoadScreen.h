// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "MVVM_LoadScreen.generated.h"

class UMVVM_LoadSlot;

/**
 * 
 */
UCLASS()
class ARCANE_API UMVVM_LoadScreen : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMVVM_LoadSlot> LoadSlotViewModelClass;	// 加载槽类

	UMVVM_LoadSlot* GetLoadSlotViewModelByIndex(int32 Index) const;	// 获取加载槽

private:
	UPROPERTY()
	TMap<int32, UMVVM_LoadSlot*> LoadSlots;		// 加载槽


};
