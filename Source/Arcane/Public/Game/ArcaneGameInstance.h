// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ArcaneGameInstance.generated.h"

class UMenuSaveGame;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOverlayWidgetShowSignature);		// OverlayWidget显示的委托

/**
 * 
 */
UCLASS()
class ARCANE_API UArcaneGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	FName PlayerStartTag = FName("");

	UPROPERTY(BlueprintReadWrite)
	FString CurrentSlotName = FString();

	// 当前存档
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UMenuSaveGame> LoadedGame = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TArray<TObjectPtr<UTexture2D>> SlotBackgrounds;

	// 保存当前游戏存档
	UFUNCTION(BlueprintCallable)
	void OnSaveCurrentGame();

	UPROPERTY(BlueprintAssignable)
	FOnOverlayWidgetShowSignature OnOverlayWidgetShow;

private:
	
};
