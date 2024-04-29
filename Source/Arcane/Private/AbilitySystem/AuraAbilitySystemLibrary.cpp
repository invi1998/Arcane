// Copyright INVI1998


#include "AbilitySystem/AuraAbilitySystemLibrary.h"

#include "AbilitySystemComponent.h"
#include "AuraAbilityTypes.h"
#include "Game/AuraGameModeBase.h"
#include "Interaction/CombatInterface.h"
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

void UAuraAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass)
{
	if (const UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject))
	{
		for (const TSubclassOf<UGameplayAbility>& AbilityClass : CharacterClassInfo->CommonAbilities)
		{
			if (AbilityClass)
			{
				FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass.GetDefaultObject(), 1);
				ASC->GiveAbility(AbilitySpec);
			}
		}

		const FCharacterClassDefaultInfo ClassDefaultInfo = CharacterClassInfo->GetCharacterClassDefaultInfo(CharacterClass);
		const ICombatInterface* CombatInterface = Cast<ICombatInterface>(ASC->GetAvatarActor());
		for (TSubclassOf<UGameplayAbility> AbilityClass : ClassDefaultInfo.StartupAbilities)
		{
			if (AbilityClass && CombatInterface)
			{
				FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass.GetDefaultObject(), CombatInterface->GetPlayerLevel());
				ASC->GiveAbility(AbilitySpec);
			}
		}
	}
}

UCharacterClassInfo* UAuraAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	if (const AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject)))
	{
		return AuraGameMode->CharacterClassInfo;
	}
	return nullptr;
}

bool UAuraAbilitySystemLibrary::IsBlockedHit(const FGameplayEffectContextHandle& ContextHandle)
{
	const FAuraGameplayEffectContext* AuraContext = static_cast<const FAuraGameplayEffectContext*>(ContextHandle.Get());

	return AuraContext && AuraContext->IsBlockedHit();
}

bool UAuraAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& ContextHandle)
{
	const FAuraGameplayEffectContext* AuraContext = static_cast<const FAuraGameplayEffectContext*>(ContextHandle.Get());

	return AuraContext && AuraContext->IsCriticalHit();
}

void UAuraAbilitySystemLibrary::SetCriticalHit(FGameplayEffectContextHandle& ContextHandle, bool bCriticalHit)
{
	if (FAuraGameplayEffectContext* AuraContext = static_cast<FAuraGameplayEffectContext*>(ContextHandle.Get()))
	{
		AuraContext->SetCriticalHit(bCriticalHit);
	}
}

void UAuraAbilitySystemLibrary::SetBlockedHit(FGameplayEffectContextHandle& ContextHandle, bool bBlockedHit)
{
	if (FAuraGameplayEffectContext* AuraContext = static_cast<FAuraGameplayEffectContext*>(ContextHandle.Get()))
	{
		AuraContext->SetBlockedHit(bBlockedHit);
	}
}

void UAuraAbilitySystemLibrary::GetLivePlayerWithinRadius(const UObject* WorldContextObject,
	TArray<AActor*>& OutPlayers, const TArray<AActor*>& IgnoreActors, const FVector& Origin, float Radius)
{
	FCollisionQueryParams SphereParams;		// ��ײ��ѯ����
	SphereParams.AddIgnoredActors(IgnoreActors);	// ���Ե���ײ��

	TArray<FOverlapResult> Overlaps;	// �ص����

	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		// �ص��������������������᷵���ص��Ķ���
		World->OverlapMultiByObjectType(Overlaps, Origin, FQuat::Identity, FCollisionObjectQueryParams::InitType::AllDynamicObjects, FCollisionShape::MakeSphere(Radius), SphereParams);
		for (FOverlapResult& Overlap : Overlaps)
		{
			if (AActor* OverlapActor = Overlap.GetActor())
			{
				if (OverlapActor->Implements<UCombatInterface>())	// ���ʵ����ս���ӿ�
				{
					if (!ICombatInterface::Execute_IsDead(OverlapActor))	// ������ţ�ΪʲôҪ��Execute_IsDead����ΪIsDead��һ�����麯��������������Ҫ��Execute_IsDead��������
					{
						OutPlayers.AddUnique(ICombatInterface::Execute_GetActor(OverlapActor));		// ��ӵ�������
					}
				}
			}
		}
	}

}

void UAuraAbilitySystemLibrary::GetLivePlayerWithinRaycast(const UObject* WorldContextObject,
	TArray<AActor*>& OutPlayers, const TArray<AActor*>& IgnoreActors, const FVector& Start, const FVector& End)
{
	FCollisionQueryParams LineParams;		// ��ײ��ѯ����
	LineParams.AddIgnoredActors(IgnoreActors);	// ���Ե���ײ��

	TArray<FHitResult> Hits;	// ��ײ���

	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		// ������ײ��������������᷵����ײ�Ķ���
		World->LineTraceMultiByChannel(Hits, Start, End, ECollisionChannel::ECC_Pawn, LineParams);
		for (FHitResult& Hit : Hits)
		{
			if (AActor* HitActor = Hit.GetActor())
			{
				if (HitActor->Implements<UCombatInterface>())	// ���ʵ����ս���ӿ�
				{
					if (!ICombatInterface::Execute_IsDead(HitActor))	// ������ţ�ΪʲôҪ��Execute_IsDead����ΪIsDead��һ�����麯��������������Ҫ��Execute_IsDead��������
					{
						OutPlayers.AddUnique(ICombatInterface::Execute_GetActor(HitActor));		// ��ӵ�������
					}
				}
			}
		}
	}
}

void UAuraAbilitySystemLibrary::GetLivePlayerWithinBoxOverlap(const UObject* WorldContextObject,
	TArray<AActor*>& OutPlayers, const TArray<AActor*>& IgnoreActors, const FVector& Origin, const FVector& BoxExtent)
{
	FCollisionQueryParams BoxParams;		// ��ײ��ѯ����
	BoxParams.AddIgnoredActors(IgnoreActors);	// ���Ե���ײ��

	TArray<FOverlapResult> Overlaps;	// �ص����

	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
				// �������ص���⣬������������᷵���ص��Ķ���
		World->OverlapMultiByObjectType(Overlaps, Origin, FQuat::Identity, FCollisionObjectQueryParams::InitType::AllDynamicObjects, FCollisionShape::MakeBox(BoxExtent), BoxParams);
		for (FOverlapResult& Overlap : Overlaps)
		{
			if (AActor* OverlapActor = Overlap.GetActor())
			{
				if (OverlapActor->Implements<UCombatInterface>())	// ���ʵ����ս���ӿ�
				{
					if (!ICombatInterface::Execute_IsDead(OverlapActor))	// ������ţ�ΪʲôҪ��Execute_IsDead����ΪIsDead��һ�����麯��������������Ҫ��Execute_IsDead��������
					{
						OutPlayers.AddUnique(ICombatInterface::Execute_GetActor(OverlapActor));		// ��ӵ�������
					}
				}
			}
		}
	}
}

void UAuraAbilitySystemLibrary::GetLivePlayerWithinForwardSector(const UObject* WorldContextObject,
	TArray<AActor*>& OutPlayers, const TArray<AActor*>& IgnoreActors, const FVector& Origin, const FVector& Forward,
	float Angle, float Radius)
{
	FCollisionQueryParams SphereParams;		// ��ײ��ѯ����
	SphereParams.AddIgnoredActors(IgnoreActors);	// ���Ե���ײ��

	TArray<FOverlapResult> Overlaps;	// �ص����

	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		// �ص��������������������᷵���ص��Ķ���
		World->OverlapMultiByObjectType(Overlaps, Origin, FQuat::Identity, FCollisionObjectQueryParams::InitType::AllDynamicObjects, FCollisionShape::MakeSphere(Radius), SphereParams);
		for (FOverlapResult& Overlap : Overlaps)
		{
			if (AActor* OverlapActor = Overlap.GetActor())
			{
				if (OverlapActor->Implements<UCombatInterface>())	// ���ʵ����ս���ӿ�
				{
					if (!ICombatInterface::Execute_IsDead(OverlapActor))	// ������ţ�ΪʲôҪ��Execute_IsDead����ΪIsDead��һ�����麯��������������Ҫ��Execute_IsDead��������
					{
						const FVector ToTarget = OverlapActor->GetActorLocation() - Origin;
						const float Dot = FVector::DotProduct(ToTarget.GetSafeNormal(), Forward.GetSafeNormal());
						const float AngleToTarget = FMath::Acos(Dot) * 180.f / PI;
						if (AngleToTarget <= Angle)
						{
							OutPlayers.AddUnique(ICombatInterface::Execute_GetActor(OverlapActor));		// ��ӵ�������
						}
					}
				}
			}
		}
	}
}

bool UAuraAbilitySystemLibrary::IsFriendly(AActor* FirstActor, AActor* SecondActor)
{
	// ͨ���ж�ActorHasTag���ж��Ƿ����ѷ�
	const bool SelfIsPlayer = FirstActor->ActorHasTag("Player");
	const bool TargetIsPlayer = SecondActor->ActorHasTag("Player");
	if (SelfIsPlayer && TargetIsPlayer)
	{
		return true;
	}
	if (!SelfIsPlayer && !TargetIsPlayer)
	{
		return true;
	}

	return false;
}

int32 UAuraAbilitySystemLibrary::GetMonsterEXPRewardByClassAndLv(const UObject* WorldContextObject,
	ECharacterClass CharacterClass, int32 CharacterLv)
{
	if (const UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject))
	{
		const FCharacterClassDefaultInfo ClassDefaultInfo = CharacterClassInfo->GetCharacterClassDefaultInfo(CharacterClass);
		return static_cast<int32>(ClassDefaultInfo.EXPReward.GetValueAtLevel(CharacterLv));
	}
	return 0;
}
