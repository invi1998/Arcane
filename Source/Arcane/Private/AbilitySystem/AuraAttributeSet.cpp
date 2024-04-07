// Copyright INVI1998


#include "AbilitySystem/AuraAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameFramework/Character.h"
#include "GameplayEffectExtension.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Net/UnrealNetwork.h"

UAuraAttributeSet::UAuraAttributeSet()
{
	InitHealth(10.f);	// ��ʼ������ֵ
	InitMaxHealth(100.f);	// ��ʼ���������ֵ
	InitMana(20.f);		// ��ʼ��ħ��
	InitMaxMana(150.f);	// ��ʼ�����ħ��
}

void UAuraAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	// ��������ֵ���������ֵ�ķ�Χ
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	else if (Attribute == GetMaxHealthAttribute())
	{
		NewValue = FMath::Max(NewValue, 0.f);
	}
	else if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
	}
	else if (Attribute == GetMaxManaAttribute())
	{
		NewValue = FMath::Max(NewValue, 0.f);
	}
}

void UAuraAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	// Source = causer of the effect, Target = the actor the effect is applied to
	// Source = Ч����ʩ���ߣ�Target = Ч��Ӧ�õ�Ŀ��
	const FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();	// ��ȡЧ��������
	const UAbilitySystemComponent* SourceASC = Context.GetOriginalInstigatorAbilitySystemComponent();	// ��ȡЧ����ʩ����

	if (IsValid(SourceASC) && SourceASC->AbilityActorInfo.IsValid() && SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		const AActor* SourceAvatarActor = SourceASC->AbilityActorInfo->AvatarActor.Get();	// ��ȡʩ���ߵ�Actor
		if (IsValid(SourceAvatarActor))
		{
			const AController* PC = SourceASC->AbilityActorInfo->PlayerController.Get();	// ��ȡʩ���ߵĿ�����
			if (IsValid(PC))
			{
				const ACharacter* Character = Cast<ACharacter>(PC->GetPawn());	// ��ȡʩ���ߵĽ�ɫ
				if (IsValid(Character))
				{
					// �������ֵС�ڵ���0����ô��ɫ����
					if (GetHealth() <= 0.f)
					{
						Character->GetCharacterMovement()->DisableMovement();	// ���ý�ɫ�ƶ�
						Character->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);	// ���ý�ɫ��ײ
					}
				}
			}
		}
	}

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		AActor* TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();	// ��ȡĿ���Actor
		AController* TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();	// ��ȡĿ��Ŀ�����
		ACharacter* TargetCharacter = Cast<ACharacter>(TargetAvatarActor);	// ��ȡĿ��Ľ�ɫ

		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetAvatarActor);	// ��ȡĿ�������ϵͳ���
	}

}

void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Health, COND_None, REPNOTIFY_Always);	// �������Եĸ��Ʒ�ʽ������������������������֪ͨ��ʽ
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);	// �������Եĸ��Ʒ�ʽ������������������������֪ͨ��ʽ
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Mana, COND_None, REPNOTIFY_Always);	// �������Եĸ��Ʒ�ʽ������������������������֪ͨ��ʽ
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);	// �������Եĸ��Ʒ�ʽ������������������������֪ͨ��ʽ
}

void UAuraAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Health, OldHealth);	// ֪ͨ���Ա仯����������������ֵ�;�ֵ
}

void UAuraAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxHealth, OldMaxHealth);	// ֪ͨ���Ա仯����������������ֵ�;�ֵ
}

void UAuraAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Mana, OldMana);	// ֪ͨ���Ա仯����������������ֵ�;�ֵ
}

void UAuraAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxMana, OldMaxMana);	// ֪ͨ���Ա仯����������������ֵ�;�ֵ
}
