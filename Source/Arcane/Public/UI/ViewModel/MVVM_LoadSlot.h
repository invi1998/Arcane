// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "MVVM_LoadSlot.generated.h"

/**
 * 
 */
UCLASS()
class ARCANE_API UMVVM_LoadSlot : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	void InitializeSlot(const FString& SlotName, const FString& SlotDescription, const FString& SlotImage);	// 初始化加载槽


};
