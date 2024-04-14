// Copyright INVI1998


#include "AbilitySystem/AuraAbilitySystemLibrary.h"

#include "AbilitySystemComponent.h"
#include "Game/AuraGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/WidgetController/OverlayWidgetController.h"

UOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	// 1：首先，我们是通过一个Widget来调用的这个函数，所以我们是从Widget内部调用它，也就是说是从本地玩家的角度调用的它，widget是在本地玩家的视角下创建的，
	// 所以我们希望关联具有该会话的本地玩家控制器，换句话说就是GetFirstLocalPlayerController
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		// 2：然后，我们希望获取HUD，HUD是我们所有Widget的展示的地方，所以我们需要获取HUD，然后在HUD中创建一个OverlayWidgetController
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(PlayerController->GetHUD()))
		{
			AAuraPlayerState* AuraPlayerState = Cast<AAuraPlayerState>(PlayerController->PlayerState);	// 获取玩家状态
			UAbilitySystemComponent* AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();	// 获取玩家的能力系统组件
			UAttributeSet* AttributeSet = AuraPlayerState->GetAttributeSet();	// 获取玩家的属性集

			// 3：有了这四个对象，我们就可以在HUD中创建一个OverlayWidgetController
			const FWidgetControllerParams InitParams = { PlayerController, AuraPlayerState, AbilitySystemComponent, AttributeSet };
			return AuraHUD->GetOverlayWidgetController(InitParams);
		}
	}
	return nullptr;
}

UAttributeMenuWidgetController* UAuraAbilitySystemLibrary::GetAttributeMenuWidgetController(
	const UObject* WorldContextObject)
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(PlayerController->GetHUD()))
		{
			AAuraPlayerState* AuraPlayerState = Cast<AAuraPlayerState>(PlayerController->PlayerState);
			UAbilitySystemComponent* AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
			UAttributeSet* AttributeSet = AuraPlayerState->GetAttributeSet();

			const FWidgetControllerParams InitParams = { PlayerController, AuraPlayerState, AbilitySystemComponent, AttributeSet };
			return AuraHUD->GetAttributeMenuWidgetController(InitParams);
		}
	}
	return nullptr;
}

void UAuraAbilitySystemLibrary::InitCharacterAttributesByClassAndLevel(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC)
{
	if (const AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject)))
	{
		const FCharacterClassDefaultInfo ClassDefaultInfo = AuraGameMode->CharacterClassInfo->GetCharacterClassDefaultInfo(CharacterClass);	// 获取角色初始化信息

		const AActor* SourceActor = ASC->GetAvatarActor();

		FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();	// 创建效果上下文
		EffectContext.AddSourceObject(SourceActor);	// 添加源对象

		// PrimaryAttributes
		const FGameplayEffectSpecHandle PrimaryAttrSpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes,  Level, EffectContext);	// 创建效果规格
		ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttrSpecHandle.Data.Get());	// 应用默认属性效果

		// SecondaryAttributes
		const FGameplayEffectSpecHandle SecondaryAttrSpecHandle = ASC->MakeOutgoingSpec(AuraGameMode->CharacterClassInfo->SecondaryAttributes, Level, EffectContext);
		ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttrSpecHandle.Data.Get());

		// VitalAttributes
		const FGameplayEffectSpecHandle VitalAttrSpecHandle = ASC->MakeOutgoingSpec(AuraGameMode->CharacterClassInfo->VitalAttributes, Level, EffectContext);
		ASC->ApplyGameplayEffectSpecToSelf(*VitalAttrSpecHandle.Data.Get());
	}

}
