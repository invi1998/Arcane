// Copyright INVI1998


#include "AbilitySystem/Abilities/AuraProjectileSpell.h"

#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/KismetSystemLibrary.h"

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	
}

void UAuraProjectileSpell::SpawnProjectile()
{
	
	// 投射物生成，我们希望他是在服务端生成，然后在客户端同步
	const bool bIsServer = GetOwningActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;

	// 生成位置，我不希望简单使用角色的位置，而是使用施法者武器上的插槽位置
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());

	// TODO: 设置投射物旋转，比如朝向目标

	if (CombatInterface)
	{
		const FVector SocketLocation = CombatInterface->GetCombatSocketLocation();

		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);		// 使用武器插槽位置

		// SpawnActorDeferred 异步生成Actor 是因为我们希望在生成之前设置一些属性，比如伤害，速度等
		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
			ProjectileClass,	// 投射物类
			SpawnTransform,		// 生成位置
			GetOwningActorFromActorInfo(),	// 拥有者
			Cast<APawn>(GetOwningActorFromActorInfo()),	// 控制者
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn	// 碰撞处理方式, 总是生成
		);

		// TODO: 设置投射物属性，比如伤害，速度等

		Projectile->FinishSpawning(SpawnTransform);	// 完成生成
	}

}
