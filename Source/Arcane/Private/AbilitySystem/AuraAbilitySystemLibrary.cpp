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
	// 1�����ȣ�������ͨ��һ��Widget�����õ�������������������Ǵ�Widget�ڲ���������Ҳ����˵�Ǵӱ�����ҵĽǶȵ��õ�����widget���ڱ�����ҵ��ӽ��´����ģ�
	// ��������ϣ���������иûỰ�ı�����ҿ����������仰˵����GetFirstLocalPlayerController
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		// 2��Ȼ������ϣ����ȡHUD��HUD����������Widget��չʾ�ĵط�������������Ҫ��ȡHUD��Ȼ����HUD�д���һ��OverlayWidgetController
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(PlayerController->GetHUD()))
		{
			AAuraPlayerState* AuraPlayerState = Cast<AAuraPlayerState>(PlayerController->PlayerState);	// ��ȡ���״̬
			UAbilitySystemComponent* AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();	// ��ȡ��ҵ�����ϵͳ���
			UAttributeSet* AttributeSet = AuraPlayerState->GetAttributeSet();	// ��ȡ��ҵ����Լ�

			// 3���������ĸ��������ǾͿ�����HUD�д���һ��OverlayWidgetController
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
		const FCharacterClassDefaultInfo ClassDefaultInfo = AuraGameMode->CharacterClassInfo->GetCharacterClassDefaultInfo(CharacterClass);	// ��ȡ��ɫ��ʼ����Ϣ

		const AActor* SourceActor = ASC->GetAvatarActor();

		FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();	// ����Ч��������
		EffectContext.AddSourceObject(SourceActor);	// ���Դ����

		// PrimaryAttributes
		const FGameplayEffectSpecHandle PrimaryAttrSpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes,  Level, EffectContext);	// ����Ч�����
		ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttrSpecHandle.Data.Get());	// Ӧ��Ĭ������Ч��

		// SecondaryAttributes
		const FGameplayEffectSpecHandle SecondaryAttrSpecHandle = ASC->MakeOutgoingSpec(AuraGameMode->CharacterClassInfo->SecondaryAttributes, Level, EffectContext);
		ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttrSpecHandle.Data.Get());

		// VitalAttributes
		const FGameplayEffectSpecHandle VitalAttrSpecHandle = ASC->MakeOutgoingSpec(AuraGameMode->CharacterClassInfo->VitalAttributes, Level, EffectContext);
		ASC->ApplyGameplayEffectSpecToSelf(*VitalAttrSpecHandle.Data.Get());
	}

}
