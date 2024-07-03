// Copyright INVI1998


#include "AbilitySystem/Abilities/AuraBeamSpell.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Character/AuraCharacterBase.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/KismetSystemLibrary.h"

void UAuraBeamSpell::StoreMouseInfo(const FHitResult& HitResult)
{
	if (HitResult.bBlockingHit)
	{
		MouseHitLocation = HitResult.ImpactPoint;
		MouseHitActor = HitResult.GetActor();
	}
	else
	{
		// 如果没有目标，就取消当前技能
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
	}
}

void UAuraBeamSpell::StoreOwnerPlayer()
{
	if (CurrentActorInfo)
	{
		OwnerPlayerController = CurrentActorInfo->PlayerController.Get();

		OwnerCharacter = Cast<ACharacter>(CurrentActorInfo->AvatarActor.Get());
	}
}

void UAuraBeamSpell::TraceFirstTarget(const FVector& BeamTargetLocation)
{
	// 采用球形射线检测
	check(OwnerCharacter);

	if (OwnerCharacter->Implements<UCombatInterface>())
	{
		if (IsWeaponCast)
		{
			if (USkeletalMeshComponent* Weapon = ICombatInterface::Execute_GetWeaponMesh(OwnerCharacter))
			{
				// 获取武器插槽位置
				const FVector SocketLocation = Weapon->GetSocketLocation(FName("TipSocket"));

				TArray<AActor*> IgnoredActors;
				IgnoredActors.Add(OwnerCharacter);

				FHitResult HitResult;

				UKismetSystemLibrary::SphereTraceSingle(
					OwnerCharacter,
					SocketLocation,
					BeamTargetLocation,
					10.f,
					TraceTypeQuery1,
					false,
					IgnoredActors,
					EDrawDebugTrace::None,
					HitResult,
					true,
					FLinearColor::Red,
					FLinearColor::Green,
					5.f);

				if (HitResult.bBlockingHit)
				{
					MouseHitLocation = HitResult.ImpactPoint;
					MouseHitActor = HitResult.GetActor();
				}
			}
		}
		else
		{
			if (USkeletalMeshComponent* CharacterMesh = ICombatInterface::Execute_GetCharacterMesh(OwnerCharacter))
			{
				// 获取角色手部施法插槽位置
				const FVector SocketLocation = CharacterMesh->GetSocketLocation(FName("LeftHeadSpellSocket"));

				TArray<AActor*> IgnoredActors;
				IgnoredActors.Add(OwnerCharacter);

				FHitResult HitResult;

				UKismetSystemLibrary::SphereTraceSingle(
					OwnerCharacter,
					SocketLocation,
					BeamTargetLocation,
					10.f,
					TraceTypeQuery1,
					false,
					IgnoredActors,
					EDrawDebugTrace::None,
					HitResult,
					true,
					FLinearColor::Red,
					FLinearColor::Green,
					5.f);

				if (HitResult.bBlockingHit)
				{
					MouseHitLocation = HitResult.ImpactPoint;
					MouseHitActor = HitResult.GetActor();
				}
			}
		}
		
	}

	// 绑定角色死亡委托
	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(OwnerCharacter))
	{
		// 如果该委托没有绑定当前对象，则绑定
		if (!CombatInterface->GetOnCharacterDeathDelegate().IsAlreadyBound(this, &UAuraBeamSpell::PrimaryBeamTargetDied))
		{
			CombatInterface->GetOnCharacterDeathDelegate().AddDynamic(this, &UAuraBeamSpell::PrimaryBeamTargetDied);
		}
	}
}

void UAuraBeamSpell::StoreAdditionalTarget(TArray<AActor*>& OutAdditionalTargets)
{
	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(GetAvatarActorFromActorInfo());
	IgnoredActors.Add(MouseHitActor);

	TArray<AActor*> OverlappingActors;
	UAuraAbilitySystemLibrary::GetLivePlayerWithinRadius(
		GetAvatarActorFromActorInfo(),
		OverlappingActors,
		IgnoredActors,
		MouseHitActor->GetActorLocation(),
		BeamRange
	);

	// 将所有的Actor按照距离MouseHitActor的距离进行排序，将前MaxNumOfTargets个Actor加入到OutAdditionalTargets中
	const int32 RealNumOfTargets = FMath::Min(GetAbilityLevel(), MaxNumOfTargets);
	// const int32 RealNumOfTargets = 5;
	UAuraAbilitySystemLibrary::GetClosestTargets(RealNumOfTargets, OverlappingActors, MouseHitActor->GetActorLocation(), OutAdditionalTargets);

	// 为每个额外的目标绑定死亡委托
	for (AActor* AdditionalTarget : OutAdditionalTargets)
	{
		if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(AdditionalTarget))
		{
			if (!CombatInterface->GetOnCharacterDeathDelegate().IsAlreadyBound(this, &UAuraBeamSpell::AdditionalBeamTargetDied))
			{
				CombatInterface->GetOnCharacterDeathDelegate().AddDynamic(this, &UAuraBeamSpell::AdditionalBeamTargetDied);
			}
		}
	}
}

float UAuraBeamSpell::GetMaxSpellContinuationTime() const
{
	if (!MaxSpellContinuationTime.IsValid())
	{
		return 0.f;
	}
	return MaxSpellContinuationTime.GetValueAtLevel(GetAbilityLevel());
}

FString UAuraBeamSpell::GetDescription(int32 Level)
{
	// 获取法力消耗，获取技能冷却时间
	const float ManaCost = GetManaCost(Level);
	const float Cooldown = GetCooldown(Level);

	// 遍历伤害类型，获取每种类型的伤害值，然后拼接字符串
	FString DamageTypeString;
	for (auto& DamagePair : DamageType)
	{
		const FGameplayTag& DamageTag = DamagePair.Key;
		const FScalableFloat& DamageValue = DamagePair.Value;
		const float ScaledDamageValue = DamageValue.GetValueAtLevel(Level);
		const FString TagName = FAuraGameplayTags::GetDamageCnName(DamageTag);
		DamageTypeString += FString::Printf(TEXT("\t<Default>%s：</><Damage>%.0f</>\n"), *TagName, ScaledDamageValue);
	}

	const int32 RealMaxNumOfTargets = FMath::Min(Level, MaxNumOfTargets);

	FString Desc = FString::Printf(TEXT("<Title>连锁闪电</>\t<Small>Electrocube</>\n\n"
			"<Default>发射一道闪电，对目标造成伤害和短暂眩晕。闪电索敌目标会跟随技能等级提升变化 </>\n\n"
			"\t<Default>技能等级：</><Level>%d</>\n"
			"\t<Default>冷却时间：</><Cooldown>%.1f s</>\n"
			"\t<Default>法力消耗：</><ManaCast>%.1f</>\n"
			"\t<Default>闪电索敌范围：</><Time>%.1f</>\n"
			"\t<Default>闪电索敌最大数量：</><Time>%d</>\n"
			"\t<Default>技能持续时间：</><Time>%.1f</>\n\n"
			"<Default>技能详细伤害描述：</>\n\n"
		), Level, Cooldown, ManaCost, BeamRange, RealMaxNumOfTargets, GetMaxSpellContinuationTime());

	Desc += DamageTypeString;

	return Desc;

}

FString UAuraBeamSpell::GetNextLevelDescription(int32 Level)
{
	// 获取法力消耗，获取技能冷却时间
	const float ManaCost = GetManaCost(Level + 1);
	const float Cooldown = GetCooldown(Level + 1);

	// 遍历伤害类型，获取每种类型的伤害值，然后拼接字符串
	FString DamageTypeString;
	for (auto& DamagePair : DamageType)
	{
		const FGameplayTag& DamageTag = DamagePair.Key;
		const FScalableFloat& DamageValue = DamagePair.Value;
		const float ScaledDamageValue = DamageValue.GetValueAtLevel(Level + 1);
		const FString TagName = FAuraGameplayTags::GetDamageCnName(DamageTag);

		DamageTypeString += FString::Printf(TEXT("\t<Default>%s：</><Damage>%.0f</>\n"), *TagName, ScaledDamageValue);
	}

	const int32 RealMaxNumOfTargets = FMath::Min(Level + 1, MaxNumOfTargets);

	FString Desc = FString::Printf(TEXT("<Title>连锁闪电</>\t<Small>Electrocube</>\n\n"
				"<Default>发射一道闪电，对目标造成伤害 </>\n\n"
				"\t<Default>技能等级：</><Level>%d</>\n"
				"\t<Default>冷却时间：</><Cooldown>%.1f s</>\n"
				"\t<Default>法力消耗：</><ManaCast>%.1f</>\n"
				"\t<Default>闪电索敌范围：</><Time>%.1f</>\n"
				"\t<Default>闪电索敌最大数量：</><Time>%d</>\n"
				"\t<Default>技能持续时间：</><Time>%.1f</>\n\n"
				"<Default>技能详细伤害描述：</>\n\n"
			), Level + 1, Cooldown, ManaCost, BeamRange, RealMaxNumOfTargets, GetMaxSpellContinuationTime());

	Desc += DamageTypeString;

	return Desc;
}
