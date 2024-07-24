// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ArcaneBlueprintFunctionLibrary.generated.h"

class UMenuSaveGame;

/**
 * 
 */
UCLASS()
class ARCANE_API UArcaneBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// 传入插槽名前缀(默认FString("UMS"))，查找所有的游戏存档名，返回命名的数组（存档限制100个)
	UFUNCTION(BlueprintCallable, Category = "Arcane|SaveGame", meta=(DefaultToSelf="WorldContextObject"))
	static TArray<FString> FindAllSaveGames(const UObject* WorldContextObject, FString SlotPrefix = FString("UMS"));

	// 传入角色名（FName)，传入插槽名前缀(默认FString("UMS")），创建一个新的游戏存档，返回存档对象
	UFUNCTION(BlueprintCallable, Category = "Arcane|SaveGame", meta = (DefaultToSelf = "WorldContextObject"))
	static UMenuSaveGame* CreateNewGameSave(const UObject* WorldContextObject, FName PlayerName, TSubclassOf<USaveGame> SaveGameClass, FString SlotPrefix = FString("UMS"));

	// 传入插槽名，获取存档对象
	UFUNCTION(BlueprintCallable, Category = "Arcane|SaveGame", meta = (DefaultToSelf = "WorldContextObject"))
	static UMenuSaveGame* LoadSpecificSlot(const UObject* WorldContextObject, FString SlotName);

};
