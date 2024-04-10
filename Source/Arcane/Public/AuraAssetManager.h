// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "AuraAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class ARCANE_API UAuraAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	static UAuraAssetManager& Get();

protected:
	virtual void StartInitialLoading() override;	// �����������Դ���������������������ʱ���ã������������ʱ���ó�ʼ������Tags��õĵط�
	
};
