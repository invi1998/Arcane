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
	InitHealth(10.f);	// 初始化生命值
	InitMaxHealth(100.f);	// 初始化最大生命值
	InitMana(20.f);		// 初始化魔法
	InitMaxMana(150.f);	// 初始化最大魔法
}

void UAuraAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	// 限制生命值和最大生命值的范围
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
	// Source = 效果的施法者，Target = 效果应用的目标
	const FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();	// 获取效果上下文
	const UAbilitySystemComponent* SourceASC = Context.GetOriginalInstigatorAbilitySystemComponent();	// 获取效果的施法者

	if (IsValid(SourceASC) && SourceASC->AbilityActorInfo.IsValid() && SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		const AActor* SourceAvatarActor = SourceASC->AbilityActorInfo->AvatarActor.Get();	// 获取施法者的Actor
		if (IsValid(SourceAvatarActor))
		{
			const AController* PC = SourceASC->AbilityActorInfo->PlayerController.Get();	// 获取施法者的控制器
			if (IsValid(PC))
			{
				const ACharacter* Character = Cast<ACharacter>(PC->GetPawn());	// 获取施法者的角色
				if (IsValid(Character))
				{
					// 如果生命值小于等于0，那么角色死亡
					if (GetHealth() <= 0.f)
					{
						Character->GetCharacterMovement()->DisableMovement();	// 禁用角色移动
						Character->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);	// 禁用角色碰撞
					}
				}
			}
		}
	}

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		AActor* TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();	// 获取目标的Actor
		AController* TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();	// 获取目标的控制器
		ACharacter* TargetCharacter = Cast<ACharacter>(TargetAvatarActor);	// 获取目标的角色

		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetAvatarActor);	// 获取目标的能力系统组件
	}

}

void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Health, COND_None, REPNOTIFY_Always);	// 设置属性的复制方式，传入属性名，复制条件，通知方式
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);	// 设置属性的复制方式，传入属性名，复制条件，通知方式
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Mana, COND_None, REPNOTIFY_Always);	// 设置属性的复制方式，传入属性名，复制条件，通知方式
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);	// 设置属性的复制方式，传入属性名，复制条件，通知方式
}

void UAuraAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Health, OldHealth);	// 通知属性变化，传入属性名，新值和旧值
}

void UAuraAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxHealth, OldMaxHealth);	// 通知属性变化，传入属性名，新值和旧值
}

void UAuraAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Mana, OldMana);	// 通知属性变化，传入属性名，新值和旧值
}

void UAuraAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxMana, OldMaxMana);	// 通知属性变化，传入属性名，新值和旧值
}
