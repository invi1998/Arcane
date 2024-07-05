// Copyright INVI1998


#include "AbilitySystem/Abilities/MonkSwing.h"

#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"

void UMonkSwing::SpawnProjectiles(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag)
{
	const bool bIsServer = GetOwningActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;

	// 生成位置，我不希望简单使用角色的位置，而是使用施法者武器上的插槽位置
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());

	if (CombatInterface)
	{
		const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(), SocketTag);

		// 这里我们直接取武器插槽的朝向作为投射物的飞行方向
		const FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();	// 获取朝向目标的旋转

		AAuraProjectile* Projectile = GetWorld()->SpawnActor<AAuraProjectile>(ProjectileClass, SocketLocation, Rotation);

		if (!Projectile) return;

		Projectile->DamageEffectParams.Empty();

		for (auto& DamagePair : DamageType)
		{
			FDamageEffectParams DmageEffectParam = MakeDamageEffectParamsFromClassDefaults(DamagePair.Key);
			Projectile->DamageEffectParams.Add(DamagePair.Key, DmageEffectParam);
		}

		Projectile->SetOwner(GetOwningActorFromActorInfo());

		Projectile->FinishSpawning(FTransform(Rotation, SocketLocation));
	}
}
