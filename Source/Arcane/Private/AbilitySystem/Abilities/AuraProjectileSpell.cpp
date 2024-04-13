// Copyright INVI1998


#include "AbilitySystem/Abilities/AuraProjectileSpell.h"

#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Kismet/KismetSystemLibrary.h"

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

}

void UAuraProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation)
{
	
	// Ͷ�������ɣ�����ϣ�������ڷ�������ɣ�Ȼ���ڿͻ���ͬ��
	const bool bIsServer = GetOwningActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;

	// ����λ�ã��Ҳ�ϣ����ʹ�ý�ɫ��λ�ã�����ʹ��ʩ���������ϵĲ��λ��
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());

	// TODO: ����Ͷ������ת�����糯��Ŀ��

	if (CombatInterface)
	{
		const FVector SocketLocation = CombatInterface->GetCombatSocketLocation();
		FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();	// ��ȡ����Ŀ�����ת
		// ��ʱ�����������߸��ڻ��ߵ��ڽ�ɫ����ôͶ����ĳ�����ܻ������⣬������Ҫ����һ��
		// ��Ϊ����ϣ��Ͷ�����ܾ�����ƽ�У����������Pith����Ϊ0
		Rotation.Pitch = 0.f;
		
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);		// ʹ���������λ��
		SpawnTransform.SetRotation(Rotation.Quaternion());	// ������ת(������Ҫ������Ԫ����

		// SpawnActorDeferred �첽����Actor ����Ϊ����ϣ��������֮ǰ����һЩ���ԣ������˺����ٶȵ�
		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
			ProjectileClass,	// Ͷ������
			SpawnTransform,		// ����λ��
			GetOwningActorFromActorInfo(),	// ӵ����
			Cast<APawn>(GetOwningActorFromActorInfo()),	// ������
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn	// ��ײ����ʽ, ��������
		);

		// TODO: ����Ͷ�������ԣ������˺����ٶȵ�
		const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwningActorFromActorInfo());	// ��ȡʩ���ߵ�ASC
		const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), SourceASC->MakeEffectContext());	// ����Ч��
		Projectile->DamageEffectSpecHandle = SpecHandle;	// ����Ч�����

		Projectile->FinishSpawning(SpawnTransform);	// �������
	}

}
