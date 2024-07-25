// Copyright INVI1998


#include "AbilitySystem/AuraAbilitySystemLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilityTypes.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Blueprint/UserWidget.h"
#include "Components/Widget.h"
#include "Components/WidgetComponent.h"
#include "Game/AuraGameModeBase.h"
#include "Game/MenuSaveGame.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/WidgetController/OverlayWidgetController.h"


bool UAuraAbilitySystemLibrary::MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutParams, AAuraHUD*& OutAuraHUD)
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		// 2：然后，我们希望获取HUD，HUD是我们所有Widget的展示的地方，所以我们需要获取HUD，然后在HUD中创建一个OverlayWidgetController
		OutAuraHUD = Cast<AAuraHUD>(PlayerController->GetHUD());	// 获取HUD
		if (OutAuraHUD)
		{
			AAuraPlayerState* AuraPlayerState = Cast<AAuraPlayerState>(PlayerController->PlayerState);	// 获取玩家状态
			UAbilitySystemComponent* AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();	// 获取玩家的能力系统组件
			UAttributeSet* AttributeSet = AuraPlayerState->GetAttributeSet();	// 获取玩家的属性集

			// 3：有了这四个对象，我们就可以在HUD中创建一个OverlayWidgetController
			OutParams.PlayerController = PlayerController;
			OutParams.PlayerState = AuraPlayerState;
			OutParams.AbilitySystemComponent = AbilitySystemComponent;
			OutParams.AttributeSet = AttributeSet;

			return true;
		}
	}
	return false;
}

UOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	// 1：首先，我们是通过一个Widget来调用的这个函数，所以我们是从Widget内部调用它，也就是说是从本地玩家的角度调用的它，widget是在本地玩家的视角下创建的，
	// 所以我们希望关联具有该会话的本地玩家控制器，换句话说就是GetFirstLocalPlayerController

	FWidgetControllerParams InitParams;
	AAuraHUD* AuraHUD = nullptr;

	if (MakeWidgetControllerParams(WorldContextObject, InitParams, AuraHUD))
	{
		return AuraHUD->GetOverlayWidgetController(InitParams);
	}

	return nullptr;
}

UAttributeMenuWidgetController* UAuraAbilitySystemLibrary::GetAttributeMenuWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams InitParams;
	AAuraHUD* AuraHUD = nullptr;

	if (MakeWidgetControllerParams(WorldContextObject, InitParams, AuraHUD))
	{
		return AuraHUD->GetAttributeMenuWidgetController(InitParams);
	}
	
	return nullptr;
}

USpellMenuWidgetController* UAuraAbilitySystemLibrary::GetSpellMenuWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams InitParams;
	AAuraHUD* AuraHUD = nullptr;

	if (MakeWidgetControllerParams(WorldContextObject, InitParams, AuraHUD))
	{
		return AuraHUD->GetSpellMenuWidgetController(InitParams);
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

void UAuraAbilitySystemLibrary::InitCharacterAttributesBySaveData(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, UMenuSaveGame* SaveGame)
{
	const UCharacterClassInfo* ClassDefaultInfo = GetCharacterClassInfo(WorldContextObject);	// 获取角色初始化信息
	if (ClassDefaultInfo)
	{
		const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();	// 获取游戏标签
		const AActor* SourceActor = ASC->GetAvatarActor();	// 获取角色

		FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();	// 创建效果上下文
		EffectContextHandle.AddSourceObject(SourceActor);	// 添加源对象

		const FGameplayEffectSpecHandle PrimaryAttrSpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo->PrimaryAttributes_SetByCaller, 1, EffectContextHandle);	// 创建效果规格

		// 通过PrimaryAttributes_SetByCaller来设置角色的属性，这个属性值是直接通过SaveGame中获取的值来设置的，没有经过任何等级缩放计算，所以这个属性值是固定的，存档加载就需要这种效果
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(PrimaryAttrSpecHandle, GameplayTags.Attributes_Primary_Strength, SaveGame->PlayerData.Strength);	// 分配体质属性
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(PrimaryAttrSpecHandle, GameplayTags.Attributes_Primary_Intelligence, SaveGame->PlayerData.Intelligence);	// 分配智力属性
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(PrimaryAttrSpecHandle, GameplayTags.Attributes_Primary_Agility, SaveGame->PlayerData.Agility);	// 分配敏捷属性
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(PrimaryAttrSpecHandle, GameplayTags.Attributes_Primary_Vigor, SaveGame->PlayerData.Vigor);	// 分配活力
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(PrimaryAttrSpecHandle, GameplayTags.Attributes_Primary_Resilience, SaveGame->PlayerData.Resilience);	// 分配韧性

		// 将效果应用到自己
		ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttrSpecHandle.Data);

		// 主属性设置完成，下一步就是设置次要属性
		// SecondaryAttributes
		const FGameplayEffectSpecHandle SecondaryAttrSpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo->SecondaryAttributes_Infinite, 1, EffectContextHandle);
		ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttrSpecHandle.Data.Get());

		// VitalAttributes
		const FGameplayEffectSpecHandle VitalAttrSpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo->VitalAttributes, 1, EffectContextHandle);
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
		const ICombatInterface* CombatInterface = ASC->GetAvatarActor()->Implements<UCombatInterface>() ? Cast<ICombatInterface>(ASC->GetAvatarActor()) : nullptr;
		for (TSubclassOf<UGameplayAbility> AbilityClass : ClassDefaultInfo.StartupAbilities)
		{
			if (AbilityClass && CombatInterface)
			{
				FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass.GetDefaultObject(), CombatInterface->Execute_GetCharacterLevel(ASC->GetAvatarActor()));
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

UAbilityInfo* UAuraAbilitySystemLibrary::GetAbilityInfo(const UObject* WorldContextObject)
{
	if (const AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject)))
	{
		return AuraGameMode->AbilityInfo;
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

bool UAuraAbilitySystemLibrary::IsSuccessfulDebuff(const FGameplayEffectContextHandle& ContextHandle)
{
	const FAuraGameplayEffectContext* AuraContext = static_cast<const FAuraGameplayEffectContext*>(ContextHandle.Get());

	return AuraContext && AuraContext->IsSuccessfulDebuff();
}

float UAuraAbilitySystemLibrary::GetDebuffDamage(const FGameplayEffectContextHandle& ContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraContext = static_cast<const FAuraGameplayEffectContext*>(ContextHandle.Get()))
	{
		return AuraContext->GetDebuffDamage();
	}
	return 0.f;
}

float UAuraAbilitySystemLibrary::GetDebuffFrequency(const FGameplayEffectContextHandle& ContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraContext = static_cast<const FAuraGameplayEffectContext*>(ContextHandle.Get()))
	{
		return AuraContext->GetDebuffFrequency();
	}
	return 0.f;
}

float UAuraAbilitySystemLibrary::GetDebuffDuration(const FGameplayEffectContextHandle& ContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraContext = static_cast<const FAuraGameplayEffectContext*>(ContextHandle.Get()))
	{
		return AuraContext->GetDebuffDuration();
	}
	return 0.f;
}

FGameplayTag UAuraAbilitySystemLibrary::GetDamageType(const FGameplayEffectContextHandle& ContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraContext = static_cast<const FAuraGameplayEffectContext*>(ContextHandle.Get()))
	{
		return AuraContext->GetDamageType().IsValid() ? *AuraContext->GetDamageType() : FGameplayTag::EmptyTag;
	}
	return FGameplayTag::EmptyTag;
}

FVector UAuraAbilitySystemLibrary::GetDeathImpulse(const FGameplayEffectContextHandle& ContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraContext = static_cast<const FAuraGameplayEffectContext*>(ContextHandle.Get()))
	{
		return AuraContext->GetDeathImpulse();
	}
	return FVector::ZeroVector;
}

FVector UAuraAbilitySystemLibrary::GetKnockbackImpulse(const FGameplayEffectContextHandle& ContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraContext = static_cast<const FAuraGameplayEffectContext*>(ContextHandle.Get()))
	{
		return AuraContext->GetKnockbackImpulse();
	}
	return FVector::ZeroVector;
}

void UAuraAbilitySystemLibrary::SetKnockbackImpulse(FGameplayEffectContextHandle& ContextHandle,
	const FVector& KnockbackImpulse)
{
	if (FAuraGameplayEffectContext* AuraContext = static_cast<FAuraGameplayEffectContext*>(ContextHandle.Get()))
	{
		AuraContext->SetKnockbackImpulse(KnockbackImpulse);
	}
}

bool UAuraAbilitySystemLibrary::IsRadialDamage(const FGameplayEffectContextHandle& ContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraContext = static_cast<const FAuraGameplayEffectContext*>(ContextHandle.Get()))
	{
		return AuraContext->IsRadialDamage();
	}
	return false;
}

float UAuraAbilitySystemLibrary::GetRadialDamageInnerRadius(const FGameplayEffectContextHandle& ContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraContext = static_cast<const FAuraGameplayEffectContext*>(ContextHandle.Get()))
	{
		return AuraContext->GetRadialDamageInnerRadius();
	}
	return 0.f;
}

float UAuraAbilitySystemLibrary::GetRadialDamageOuterRadius(const FGameplayEffectContextHandle& ContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraContext = static_cast<const FAuraGameplayEffectContext*>(ContextHandle.Get()))
	{
		return AuraContext->GetRadialDamageOuterRadius();
	}
	return 0.f;
}

FVector UAuraAbilitySystemLibrary::GetRadialDamageOrigin(const FGameplayEffectContextHandle& ContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraContext = static_cast<const FAuraGameplayEffectContext*>(ContextHandle.Get()))
	{
		return AuraContext->GetRadialDamageOrigin();
	}
	return FVector::ZeroVector;
}

void UAuraAbilitySystemLibrary::SetIsRadialDamage(FGameplayEffectContextHandle& ContextHandle, bool bIsRadialDamage)
{
	if (FAuraGameplayEffectContext* AuraContext = static_cast<FAuraGameplayEffectContext*>(ContextHandle.Get()))
	{
		AuraContext->SetRadialDamage(bIsRadialDamage);
	}
}

void UAuraAbilitySystemLibrary::SetRadialDamageInnerRadius(FGameplayEffectContextHandle& ContextHandle, float RadialDamageInnerRadius)
{
	if (FAuraGameplayEffectContext* AuraContext = static_cast<FAuraGameplayEffectContext*>(ContextHandle.Get()))
	{
		AuraContext->SetRadialDamageInnerRadius(RadialDamageInnerRadius);
	}
}

void UAuraAbilitySystemLibrary::SetRadialDamageOuterRadius(FGameplayEffectContextHandle& ContextHandle, float RadialDamageOuterRadius)
{
	if (FAuraGameplayEffectContext* AuraContext = static_cast<FAuraGameplayEffectContext*>(ContextHandle.Get()))
	{
		AuraContext->SetRadialDamageOuterRadius(RadialDamageOuterRadius);
	}
}

void UAuraAbilitySystemLibrary::SetRadialDamageOrigin(FGameplayEffectContextHandle& ContextHandle, const FVector& RadialDamageOrigin)
{
	if (FAuraGameplayEffectContext* AuraContext = static_cast<FAuraGameplayEffectContext*>(ContextHandle.Get()))
	{
		AuraContext->SetRadialDamageOrigin(RadialDamageOrigin);
	}
}

void UAuraAbilitySystemLibrary::SetDeathImpulse(FGameplayEffectContextHandle& ContextHandle, const FVector& DeathImpulse)
{
	if (FAuraGameplayEffectContext* AuraContext = static_cast<FAuraGameplayEffectContext*>(ContextHandle.Get()))
	{
		AuraContext->SetDeathImpulse(DeathImpulse);
	}
}

void UAuraAbilitySystemLibrary::SetSuccessfulDebuff(FGameplayEffectContextHandle& ContextHandle, bool bSuccessfulDebuff)
{
	if (FAuraGameplayEffectContext* AuraContext = static_cast<FAuraGameplayEffectContext*>(ContextHandle.Get()))
	{
		AuraContext->SetSuccessfulDebuff(bSuccessfulDebuff);
	}
}

void UAuraAbilitySystemLibrary::SetDebuffDamage(FGameplayEffectContextHandle& ContextHandle, float DebuffDamage)
{
	if (FAuraGameplayEffectContext* AuraContext = static_cast<FAuraGameplayEffectContext*>(ContextHandle.Get()))
	{
		AuraContext->SetDebuffDamage(DebuffDamage);
	}
}

void UAuraAbilitySystemLibrary::SetDebuffFrequency(FGameplayEffectContextHandle& ContextHandle, float DebuffFrequency)
{
	if (FAuraGameplayEffectContext* AuraContext = static_cast<FAuraGameplayEffectContext*>(ContextHandle.Get()))
	{
		AuraContext->SetDebuffFrequency(DebuffFrequency);
	}
}

void UAuraAbilitySystemLibrary::SetDebuffDuration(FGameplayEffectContextHandle& ContextHandle, float DebuffDuration)
{
	if (FAuraGameplayEffectContext* AuraContext = static_cast<FAuraGameplayEffectContext*>(ContextHandle.Get()))
	{
		AuraContext->SetDebuffDuration(DebuffDuration);
	}
}

void UAuraAbilitySystemLibrary::SetDamageType(FGameplayEffectContextHandle& ContextHandle, FGameplayTag DamageType)
{
	if (FAuraGameplayEffectContext* AuraContext = static_cast<FAuraGameplayEffectContext*>(ContextHandle.Get()))
	{
		AuraContext->SetDamageType(MakeShared<FGameplayTag>(DamageType));
	}
}

void UAuraAbilitySystemLibrary::GetLivePlayerWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutPlayers, const TArray<AActor*>& IgnoreActors, const FVector& Origin, float Radius)
{
	FCollisionQueryParams SphereParams;		// 碰撞查询参数
	SphereParams.AddIgnoredActors(IgnoreActors);	// 忽略的碰撞体

	TArray<FOverlapResult> Overlaps;	// 重叠结果

	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		// 重叠多个对象，下面这个函数会返回重叠的对象
		World->OverlapMultiByObjectType(Overlaps, Origin, FQuat::Identity, FCollisionObjectQueryParams::InitType::AllDynamicObjects, FCollisionShape::MakeSphere(Radius), SphereParams);
		for (FOverlapResult& Overlap : Overlaps)
		{
			if (AActor* OverlapActor = Overlap.GetActor())
			{
				if (OverlapActor->Implements<UCombatInterface>())	// 如果实现了战斗接口
				{
					if (!ICombatInterface::Execute_IsDead(OverlapActor))	// 如果活着，为什么要用Execute_IsDead，因为IsDead是一个纯虚函数，所以我们需要用Execute_IsDead来调用它
					{
						OutPlayers.AddUnique(ICombatInterface::Execute_GetActor(OverlapActor));		// 添加到数组中
					}
				}
			}
		}
	}

}

void UAuraAbilitySystemLibrary::GetLivePlayerWithinRaycast(const UObject* WorldContextObject,
	TArray<AActor*>& OutPlayers, const TArray<AActor*>& IgnoreActors, const FVector& Start, const FVector& End)
{
	FCollisionQueryParams LineParams;		// 碰撞查询参数
	LineParams.AddIgnoredActors(IgnoreActors);	// 忽略的碰撞体

	TArray<FHitResult> Hits;	// 碰撞结果

	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		// 射线碰撞，下面这个函数会返回碰撞的对象
		World->LineTraceMultiByChannel(Hits, Start, End, ECollisionChannel::ECC_Pawn, LineParams);
		for (FHitResult& Hit : Hits)
		{
			if (AActor* HitActor = Hit.GetActor())
			{
				if (HitActor->Implements<UCombatInterface>())	// 如果实现了战斗接口
				{
					if (!ICombatInterface::Execute_IsDead(HitActor))	// 如果活着，为什么要用Execute_IsDead，因为IsDead是一个纯虚函数，所以我们需要用Execute_IsDead来调用它
					{
						OutPlayers.AddUnique(ICombatInterface::Execute_GetActor(HitActor));		// 添加到数组中
					}
				}
			}
		}
	}
}

void UAuraAbilitySystemLibrary::GetLivePlayerWithinBoxOverlap(const UObject* WorldContextObject,
	TArray<AActor*>& OutPlayers, const TArray<AActor*>& IgnoreActors, const FVector& Origin, const FVector& BoxExtent)
{
	FCollisionQueryParams BoxParams;		// 碰撞查询参数
	BoxParams.AddIgnoredActors(IgnoreActors);	// 忽略的碰撞体

	TArray<FOverlapResult> Overlaps;	// 重叠结果

	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
				// 立方体重叠检测，下面这个函数会返回重叠的对象
		World->OverlapMultiByObjectType(Overlaps, Origin, FQuat::Identity, FCollisionObjectQueryParams::InitType::AllDynamicObjects, FCollisionShape::MakeBox(BoxExtent), BoxParams);
		for (FOverlapResult& Overlap : Overlaps)
		{
			if (AActor* OverlapActor = Overlap.GetActor())
			{
				if (OverlapActor->Implements<UCombatInterface>())	// 如果实现了战斗接口
				{
					if (!ICombatInterface::Execute_IsDead(OverlapActor))	// 如果活着，为什么要用Execute_IsDead，因为IsDead是一个纯虚函数，所以我们需要用Execute_IsDead来调用它
					{
						OutPlayers.AddUnique(ICombatInterface::Execute_GetActor(OverlapActor));		// 添加到数组中
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
	FCollisionQueryParams SphereParams;		// 碰撞查询参数
	SphereParams.AddIgnoredActors(IgnoreActors);	// 忽略的碰撞体

	TArray<FOverlapResult> Overlaps;	// 重叠结果

	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		// 重叠多个对象，下面这个函数会返回重叠的对象
		World->OverlapMultiByObjectType(Overlaps, Origin, FQuat::Identity, FCollisionObjectQueryParams::InitType::AllDynamicObjects, FCollisionShape::MakeSphere(Radius), SphereParams);
		for (FOverlapResult& Overlap : Overlaps)
		{
			if (AActor* OverlapActor = Overlap.GetActor())
			{
				if (OverlapActor->Implements<UCombatInterface>())	// 如果实现了战斗接口
				{
					if (!ICombatInterface::Execute_IsDead(OverlapActor))	// 如果活着，为什么要用Execute_IsDead，因为IsDead是一个纯虚函数，所以我们需要用Execute_IsDead来调用它
					{
						const FVector ToTarget = OverlapActor->GetActorLocation() - Origin;
						const float Dot = FVector::DotProduct(ToTarget.GetSafeNormal(), Forward.GetSafeNormal());
						const float AngleToTarget = FMath::Acos(Dot) * 180.f / PI;
						if (AngleToTarget <= Angle)
						{
							OutPlayers.AddUnique(ICombatInterface::Execute_GetActor(OverlapActor));		// 添加到数组中
						}
					}
				}
			}
		}
	}
}

bool UAuraAbilitySystemLibrary::IsFriendly(AActor* FirstActor, AActor* SecondActor)
{
	// 通过判断ActorHasTag来判断是否是友方
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

void UAuraAbilitySystemLibrary::GetWidgetPositionAndSize(const UObject* WorldContextObject, UUserWidget* MyWidgetInstance, FVector2D& OutPosition, FVector2D& OutSize)
{
	if (MyWidgetInstance)
	{
		OutPosition = MyWidgetInstance->GetCachedGeometry().GetAbsolutePosition();
		UKismetSystemLibrary::PrintString(WorldContextObject, FString::Printf(TEXT("Position: %s"), *OutPosition.ToString()), true, false, FLinearColor::Green, 5.f);
	}
}

void UAuraAbilitySystemLibrary::GetAbilityDescriptionByTag(const UObject* WorldContextObject, const FGameplayTag& AbilityTag, FString& OutDescription, FString& OutNextLevelDescription)
{
	if (const APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		const AAuraPlayerState* AuraPlayerState = Cast<AAuraPlayerState>(PlayerController->PlayerState);	// 获取玩家状态
		UAuraAbilitySystemComponent* AbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(AuraPlayerState->GetAbilitySystemComponent());	// 获取玩家的能力系统组件

		AbilitySystemComponent->GetDescriptionByTag(AbilityTag, OutDescription, OutNextLevelDescription);
	}
}

int32 UAuraAbilitySystemLibrary::GetAbilityLevelByTag(const UObject* WorldContextObject, const FGameplayTag& AbilityTag)
{
	if (const APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		const AAuraPlayerState* AuraPlayerState = Cast<AAuraPlayerState>(PlayerController->PlayerState);	// 获取玩家状态
		UAuraAbilitySystemComponent* AbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(AuraPlayerState->GetAbilitySystemComponent());	// 获取玩家的能力系统组件

		return AbilitySystemComponent->GetAbilityLevelByTag(AbilityTag);
	}
	return 0;
}

float UAuraAbilitySystemLibrary::GetAbilityManaCostByTag(const UObject* WorldContextObject, const FGameplayTag& AbilityTag)
{
	if (const APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		const AAuraPlayerState* AuraPlayerState = Cast<AAuraPlayerState>(PlayerController->PlayerState);	// 获取玩家状态
		UAuraAbilitySystemComponent* AbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(AuraPlayerState->GetAbilitySystemComponent());	// 获取玩家的能力系统组件

		return AbilitySystemComponent->GetManaCostByAbilityTag(AbilityTag);
	}
	return 0.f;
}

FGameplayEffectContextHandle UAuraAbilitySystemLibrary::ApplyDamageEffect(const FDamageEffectParams& Params)
{
	if (Params.WorldContextObject && Params.DamageEffectClass && IsValid(Params.InstigatorASC) && IsValid(Params.TargetASC))
	{
		FAuraGameplayTags Tags = FAuraGameplayTags::Get();	// 获取游戏标签

		FGameplayEffectContextHandle ContextHandle = Params.InstigatorASC->MakeEffectContext();	// 创建效果上下文
		ContextHandle.AddSourceObject(Params.WorldContextObject);	// 添加源对象
		SetDeathImpulse(ContextHandle, Params.DeathImpulse);	// 设置死亡冲量
		SetKnockbackImpulse(ContextHandle, Params.KnockbackImpulse);	// 设置击退冲量

		SetIsRadialDamage(ContextHandle, Params.bIsRadialDamage);	// 设置是否径向伤害
		SetRadialDamageInnerRadius(ContextHandle, Params.RadialDamageInnerRadius);	// 设置径向伤害内半径
		SetRadialDamageOuterRadius(ContextHandle, Params.RadialDamageOuterRadius);	// 设置径向伤害外半径
		SetRadialDamageOrigin(ContextHandle, Params.RadialDamageOrigin);	// 设置径向伤害源

		const FGameplayEffectSpecHandle DamageSpecHandle = Params.InstigatorASC->MakeOutgoingSpec(Params.DamageEffectClass, Params.AbilityLevel, ContextHandle);	// 创建效果规格

		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, Params.DamageType, Params.BaseDamage);	// Assign the damage value to the tag（注册伤害值到标签）
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, Tags.Debuff_Chance, Params.DebuffChance);	// Assign the debuff chance value to the tag（注册debuff几率到标签）
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, Tags.Debuff_Damage, Params.DebuffDamage);	// Assign the debuff damage value to the tag（注册debuff伤害到标签）
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, Tags.Debuff_Frequency, Params.DebuffFrequency);	// Assign the debuff frequency value to the tag（注册debuff频率到标签）
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, Tags.Debuff_Duration, Params.DebuffDuration);	// Assign the debuff duration value to the tag（注册debuff持续时间到标签）

		Params.TargetASC->ApplyGameplayEffectSpecToSelf(*DamageSpecHandle.Data.Get());	// 应用效果到自己

		return ContextHandle;
	}
	   
	return FGameplayEffectContextHandle();
}

TArray<FVector> UAuraAbilitySystemLibrary::EvenlySpacedVectors(const FVector& Forward, const FVector& Axis, float SpreadAngle, int32 VectorNum)
{
	TArray<FVector> Vectors;
	if (VectorNum <= 0)
	{
		return Vectors;
	}

	if (VectorNum == 1)
	{
		Vectors.Add(Forward);
		return Vectors;
	}

	const FVector LeftVector = Forward.RotateAngleAxis(-SpreadAngle / 2.f, Axis);

	const float DeltaAngle = SpreadAngle / (VectorNum - 1);

	for (int32 i = 0; i < VectorNum; i++)
	{
		const FVector RotatedVector = LeftVector.RotateAngleAxis(DeltaAngle * i, Axis);
		Vectors.Add(RotatedVector);
	}

	return Vectors;
}

TArray<FRotator> UAuraAbilitySystemLibrary::EvenlySpacedRotators(const FVector& Forward, const FVector& Axis, float SpreadAngle, int32 RotatorNum)
{
	TArray<FRotator> Rotators;
	if (RotatorNum <= 0)
	{
		return Rotators;
	}

	if (RotatorNum == 1)
	{
		Rotators.Add(Forward.Rotation());
		return Rotators;
	}

	const FVector LeftVector = Forward.RotateAngleAxis(-SpreadAngle / 2.f, Axis);

	const float DeltaAngle = SpreadAngle / RotatorNum;

	for (int32 i = 0; i < RotatorNum; i++)
	{
		const FVector RotatedVector = LeftVector.RotateAngleAxis(DeltaAngle * i, Axis);
		Rotators.Add(RotatedVector.Rotation());
	}
	return Rotators;
}

void UAuraAbilitySystemLibrary::GetClosestTargets(int32 MaxTargets, const TArray<AActor*>& TargetActors,
	const FVector& Origin, TArray<AActor*>& OutClosestTargets)
{
	TArray<AActor*> SortedActors = TargetActors;
	SortedActors.Sort([Origin](const AActor& A, const AActor& B)
	{
		return FVector::DistSquared(A.GetActorLocation(), Origin) < FVector::DistSquared(B.GetActorLocation(), Origin);
	});
	/*Algo::Sort(SortedActors, [Origin](const AActor& A, const AActor& B)
	{
		return FVector::DistSquared(A.GetActorLocation(), Origin) < FVector::DistSquared(B.GetActorLocation(), Origin);
	});*/

	for (int32 i = 0; i < FMath::Min(MaxTargets, SortedActors.Num()); i++)
	{
		OutClosestTargets.Add(SortedActors[i]);
	}
}

FVector UAuraAbilitySystemLibrary::GetPositionByAngleAndRadius(const FVector& Origin, const FVector& Forward, float Angle, float Radius)
{
	// 获取当前前向向量的以Origin为中心，以Forward为轴，Angle为角度，Radius为半径的位置
	const FRotator TemRotator = FRotator(0.f, Angle, 0.f);
	const FVector Position = Origin + TemRotator.RotateVector(Forward) * Radius;
	return Position;
}

