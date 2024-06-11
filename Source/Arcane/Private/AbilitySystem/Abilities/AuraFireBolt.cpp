// Copyright INVI1998


#include "AbilitySystem/Abilities/AuraFireBolt.h"

#include "AuraGameplayTags.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/KismetSystemLibrary.h"

FString UAuraFireBolt::GetDescription(int32 Level)
{
	// 更具Tag获取法力消耗
	const float Cooldown = GetCooldown(Level);
	const float ManaCost = GetManaCost(Level);

	// 遍历伤害类型，获取每种类型的伤害值，然后拼接字符串
	FString DamageTypeString;
	for (auto& DamagePair : DamageType)
	{
		const FGameplayTag& DamageTag = DamagePair.Key;
		const FScalableFloat& DamageValue = DamagePair.Value;
		const float ScaledDamageValue = DamageValue.GetValueAtLevel(Level);
		const FString TagName = FAuraGameplayTags::GetDamageCnName(DamageTag);
		DamageTypeString += FString::Printf(TEXT("\t<Default>%s：</><Damage>%.2f</>\n"), *TagName, ScaledDamageValue);
	}

	FString Desc = FString::Printf(TEXT("<Title>火球术</>\t<Small>FIRE BOLT</>\n\n"
		"<Default>发射一道火焰，在撞击和造成伤害时爆炸 </>\n\n"
		"\t<Default>技能等级：</><Level>%d</>\n"
		"\t<Default>冷却时间：</><Cooldown>%.1f s</>\n"
		"\t<Default>法力消耗：</><ManaCast>%.1f</>\n"
		"\t<Default>火球数量：</><Time>%d</>\n\n"
		"<Default>技能详细伤害描述：</>\n"
	), Level, Cooldown, ManaCost, NumProjectiles);
	Desc += DamageTypeString;

	return Desc;
}

FString UAuraFireBolt::GetNextLevelDescription(int32 Level)
{
	// 更具Tag获取法力消耗
	const float Cooldown = GetCooldown(Level + 1);
	const float ManaCost = GetManaCost(Level + 1);

	// 遍历伤害类型，获取每种类型的伤害值，然后拼接字符串
	FString DamageTypeString;
	for (auto& DamagePair : DamageType)
	{
		const FGameplayTag& DamageTag = DamagePair.Key;
		const FScalableFloat& DamageValue = DamagePair.Value;
		const float ScaledDamageValue = DamageValue.GetValueAtLevel(Level + 1);
		const FString TagName = FAuraGameplayTags::GetDamageCnName(DamageTag);
		DamageTypeString += FString::Printf(TEXT("\t<Default>%s：</><Damage>%.2f</>\n"), *TagName, ScaledDamageValue);
	}

	FString Desc = FString::Printf(TEXT("<Title>火球术</>\t<Small>FIRE BOLT</>\n\n"
		"<Default>发射一道火焰，在撞击和造成伤害时爆炸 </>\n\n"
		"\t<Default>技能等级：</><Level>%d</>\n"
		"\t<Default>冷却时间：</><Cooldown>%.1f s</>\n"
		"\t<Default>法力消耗：</><ManaCast>%.1f</>\n"
		"\t<Default>火球数量：</><Time>%d</>\n\n"
		"<Default>技能详细伤害描述：</>\n\n"
	), Level + 1, Cooldown, ManaCost, NumProjectiles);
	Desc += DamageTypeString;

	return Desc;
}

void UAuraFireBolt::SpawnProjectiles(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag,AActor* HomingTarget, bool bOverridePitch, float Pitch)
{
	// 投射物生成，我们希望他是在服务端生成，然后在客户端同步
	const bool bIsServer = GetOwningActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;

	// 生成位置，我不希望简单使用角色的位置，而是使用施法者武器上的插槽位置
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());

	if (CombatInterface)
	{
		const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(), SocketTag);
		FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();	// 获取朝向目标的旋转
		// 此时，如果怪物身高高于或者低于角色，那么投射物的朝向可能会有问题，我们需要调整一下
		// 因为我们希望投射物能尽可能平行，所以这里把Pith调整为0
		// Rotation.Pitch = 0.f;

		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);		// 使用武器插槽位置
		SpawnTransform.SetRotation(Rotation.Quaternion());	// 设置旋转(这里需要传入四元数）

		if (bOverridePitch)
		{
			SpawnTransform.SetRotation(FRotator(Pitch, Rotation.Yaw, Rotation.Roll).Quaternion());
		}

		const FVector ForwardVector = Rotation.Vector();
		const FVector LeftVector = ForwardVector.RotateAngleAxis(-ProjectileSpread / 2.f, FVector::UpVector);

		// const int32 RealNumProjectiles = FMath::Min(MaxProjectileCount, GetAbilityLevel());

		const int32 RealNumProjectiles = 10;

		const float DeltaSpread = ProjectileSpread / FMath::Max(1, RealNumProjectiles - 1);

		
		if (RealNumProjectiles > 1)
		{
			for (int32 i = 0; i < RealNumProjectiles; ++i)
			{
				const FVector SpreadVector = LeftVector.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
				const FRotator SpreadRotation = SpreadVector.Rotation();

				// 在生成点和目标点之间生成贝塞尔曲线

				UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), SocketLocation, SocketLocation + SpreadVector * 1000.f, 100.f, FLinearColor::Green, 125.f, 1.f);
			}
		}
		else
		{
			
		}

		UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), SocketLocation, SocketLocation + Rotation.Vector() * 1000.f, 100.f, FLinearColor::Red, 125.f, 5.f);
	}
}
