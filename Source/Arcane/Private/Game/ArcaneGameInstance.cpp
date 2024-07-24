// Copyright INVI1998


#include "Game/ArcaneGameInstance.h"

#include "Character/AuraCharacter.h"
#include "Game/MenuSaveGame.h"
#include "Kismet/GameplayStatics.h"

void UArcaneGameInstance::OnSaveCurrentGame()
{

	// 获取当前角色等级
	if (AAuraCharacter* PlayerCharacter = Cast<AAuraCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		if (const ICombatInterface* CombatInterface = PlayerCharacter->Implements<UCombatInterface>() ? Cast<ICombatInterface>(PlayerCharacter) : nullptr)
		{
			const int32 Level = CombatInterface->Execute_GetCharacterLevel(PlayerCharacter);

			// 从SlotBackgrounds中随机选择一个背景
			const TObjectPtr<UTexture2D> BG = SlotBackgrounds[FMath::RandRange(0, SlotBackgrounds.Num() - 1)];

			LoadedGame->SaveGameSlot.DateTime = FDateTime::Now();	// 保存当前时间
			LoadedGame->SaveGameSlot.Level = Level;	// 保存当前等级
			LoadedGame->SaveGameSlot.QuestImage = BG;	// 保存当前背景

			const FTransform PlayerTransform = PlayerCharacter->GetActorTransform();
			LoadedGame->SavedGameInfo.PlayerTransform = PlayerTransform;	// 保存当前角色位置
			LoadedGame->SavedGameInfo.IsNewGame = false;					// 保存当前是否为新游戏

			// 保存当前游戏
			UGameplayStatics::SaveGameToSlot(LoadedGame, CurrentSlotName, 0);
		}
	}
	
}
