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

	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Activate Projectile Spell"), true, false, FLinearColor::Red, 1.0f);

	// Ͷ�������ɣ�����ϣ�������ڷ�������ɣ�Ȼ���ڿͻ���ͬ��
	const bool bIsServer = HasAuthority(&ActivationInfo);
	if (!bIsServer) return;

	// ����λ�ã��Ҳ�ϣ����ʹ�ý�ɫ��λ�ã�����ʹ��ʩ���������ϵĲ��λ��
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());

	// TODO: ����Ͷ������ת�����糯��Ŀ��

	if (CombatInterface)
	{
		const FVector SocketLocation = CombatInterface->GetCombatSocketLocation();

		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);		// ʹ���������λ��

		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
			ProjectileClass,	// Ͷ������
			SpawnTransform,		// ����λ��
			GetOwningActorFromActorInfo(),	// ӵ����
			Cast<APawn>(GetOwningActorFromActorInfo()),	// ������
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn	// ��ײ����ʽ, ��������
		);

		// TODO: ����Ͷ�������ԣ������˺����ٶȵ�

		Projectile->FinishSpawning(SpawnTransform);	// �������
	}

}
