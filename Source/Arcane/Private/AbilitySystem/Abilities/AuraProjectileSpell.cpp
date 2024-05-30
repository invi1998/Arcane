// Copyright INVI1998


#include "AbilitySystem/Abilities/AuraProjectileSpell.h"

#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "Kismet/KismetSystemLibrary.h"

FString UAuraProjectileSpell::GetDescription(int32 Level)
{
	// 更具Tag获取法力消耗
	const float Cooldown = CooldownScalableFloat.GetValueAtLevel(Level);
	const float ManaCost = ManaCostScalableFloat.GetValueAtLevel(Level);

	// 遍历伤害类型，获取每种类型的伤害值，然后拼接字符串
	FString DamageTypeString;
	for (auto& DamagePair : DamageType)
	{
		const FGameplayTag& DamageTag = DamagePair.Key;
		const FScalableFloat& DamageValue = DamagePair.Value;
		const float ScaledDamageValue = DamageValue.GetValueAtLevel(Level);
		DamageTypeString += FString::Printf(TEXT("\t<Default>%s：</><Damage>%.2f</>\n"), *DamageTag.GetTagName().ToString(), ScaledDamageValue);
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

FString UAuraProjectileSpell::GetNextLevelDescription(int32 Level)
{
	const int32 Damage = DamageType[FAuraGameplayTags::Get().Damage_Fire].GetValueAtLevel(Level);
	return FString::Printf(TEXT("<Title>NEXT LEVEL: </>\n\n<Default>Launches %d bolts of fire, exploding on impact and dealing: </><Damage>%d</><Default> fire damage with a chance to burn</>\n\n<Small>Level: </><Level>%d</>"), FMath::Min(Level, NumProjectiles), Damage, Level);
}

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UAuraProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation, const FGameplayTag& Tag, bool bOverridePitch, float PitchOverride)
{
	
	// 投射物生成，我们希望他是在服务端生成，然后在客户端同步
	const bool bIsServer = GetOwningActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;

	// 生成位置，我不希望简单使用角色的位置，而是使用施法者武器上的插槽位置
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());

	// TODO: 设置投射物旋转，比如朝向目标

	if (CombatInterface)
	{
		const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(), Tag);
		FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();	// 获取朝向目标的旋转
		// 此时，如果怪物身高高于或者低于角色，那么投射物的朝向可能会有问题，我们需要调整一下
		// 因为我们希望投射物能尽可能平行，所以这里把Pith调整为0
		// Rotation.Pitch = 0.f;
		
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);		// 使用武器插槽位置
		SpawnTransform.SetRotation(Rotation.Quaternion());	// 设置旋转(这里需要传入四元数）

		if (bOverridePitch)
		{
			SpawnTransform.SetRotation(FRotator(PitchOverride, Rotation.Yaw, Rotation.Roll).Quaternion());
		}

		// SpawnActorDeferred 异步生成Actor 是因为我们希望在生成之前设置一些属性，比如伤害，速度等
		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
			ProjectileClass,	// 投射物类
			SpawnTransform,		// 生成位置
			GetOwningActorFromActorInfo(),	// 拥有者
			Cast<APawn>(GetOwningActorFromActorInfo()),	// 控制者
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn	// 碰撞处理方式, 总是生成
		);

		// TODO: 设置投射物属性，比如伤害，速度等
		const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwningActorFromActorInfo());	// 获取施法者的ASC
		FGameplayEffectContextHandle EffectContextHandle = SourceASC->MakeEffectContext();	// 生成效果上下文
		EffectContextHandle.SetAbility(this);	// 设置技能
		EffectContextHandle.AddSourceObject(Projectile);	// 添加来源对象
		TArray<TWeakObjectPtr<AActor>> Actors;	// 生成一个Actor数组
		Actors.Add(Projectile);	// 添加投射物
		EffectContextHandle.AddActors(Actors);	// 添加Actor
		FHitResult HitResult;	// 生成一个命中结果
		HitResult.Location = ProjectileTargetLocation;	// 设置命中位置
		EffectContextHandle.AddHitResult(HitResult);	// 添加命中结果

		const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), EffectContextHandle);	// 生成效果

		for (auto& DamagePair : DamageType)	// 遍历伤害类型
		{
			const FGameplayTag& DamageTag = DamagePair.Key;	// 获取伤害标签
			const FScalableFloat& DamageValue = DamagePair.Value;	// 获取伤害值
			const float ScaledDamageValue = DamageValue.GetValueAtLevel(GetAbilityLevel());	// 获取伤害值（根据等级，从ScalableFloat中获取）
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DamageTag, ScaledDamageValue);	// 设置伤害
		}

		Projectile->DamageEffectSpecHandle = SpecHandle;	// 设置效果句柄

		Projectile->FinishSpawning(SpawnTransform);	// 完成生成
	}

}

void UAuraProjectileSpell::SetProjectileClass(TSubclassOf<AAuraProjectile> NewProjectileClass)
{
	ProjectileClass = NewProjectileClass;
}
