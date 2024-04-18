// Copyright INVI1998


#include "AbilitySystem/AuraAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "GameFramework/Character.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"

#include "Net/UnrealNetwork.h"
#include "Player/AuraPlayerController.h"

UAuraAttributeSet::UAuraAttributeSet()
{
	// const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();	// 获取标签

	///* Primary Attributes */
	//TagsToAttributes.Add(GameplayTags.Attributes_Primary_Strength, GetStrengthAttribute);
	//TagsToAttributes.Add(GameplayTags.Attributes_Primary_Intelligence, GetIntelligenceAttribute);
	//TagsToAttributes.Add(GameplayTags.Attributes_Primary_Resilience, GetResilienceAttribute);
	//TagsToAttributes.Add(GameplayTags.Attributes_Primary_Vigor, GetVigorAttribute);

	///* Secondary Attributes */
	//TagsToAttributes.Add(GameplayTags.Attributes_Secondary_Armor, GetArmorAttribute);
	//TagsToAttributes.Add(GameplayTags.Attributes_Secondary_ArmorPenetration, GetArmorPenetrationAttribute);
	//TagsToAttributes.Add(GameplayTags.Attributes_Secondary_BlockChance, GetBlockChanceAttribute);
	//TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CriticalHitChance, GetCriticalHitChanceAttribute);
	//TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CriticalHitResistance, GetCriticalHitResistanceAttribute);
	//TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CriticalHitDamage, GetCriticalHitDamageAttribute);
	//TagsToAttributes.Add(GameplayTags.Attributes_Secondary_HealthRegeneration, GetHealthRegenerationAttribute);
	//TagsToAttributes.Add(GameplayTags.Attributes_Secondary_ManaRegeneration, GetManaRegenerationAttribute);
	//TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MaxHealth, GetMaxHealthAttribute);
	//TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MaxMana, GetMaxManaAttribute);
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

	FEffectProperties EffectProperties;
	SetEffectsProperties(Data, EffectProperties);	// 设置效果属性

	// Clamp
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
	}
	else if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.0f, GetMaxMana()));
	}
	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		const float LocalIncomingDamage = GetIncomingDamage();
		SetIncomingDamage(0.0f);
		if (LocalIncomingDamage > 0.0f)
		{
			// Apply the damage to the target
			// 造成伤害
			const float NewHealth = GetHealth() - LocalIncomingDamage;
			SetHealth(FMath::Clamp(NewHealth, 0.0f, GetMaxHealth()));

			const bool bFatal = GetHealth() <= 0.0f;	// 是否致命

			// 如果不是致命的，那么就激活这个Ability
			if (bFatal)
			{
				// 创建一个TagContainer，将Effect_Fatal标签添加进去
				FGameplayTagContainer FatalTagContainer;
				FatalTagContainer.AddTag(FAuraGameplayTags::Get().Effect_DeathReact);	// 添加致命标签
				EffectProperties.TargetASC->TryActivateAbilitiesByTag(FatalTagContainer);	// 尝试激活标签的能力
				ICombatInterface* CombatInterface = Cast<ICombatInterface>(EffectProperties.TargetAvatarActor);	// 获取战斗接口
				if (CombatInterface)
				{
					CombatInterface->Die();	// 死亡
				}
			}
			else
			{
				// 创建一个TagContainer，将Effect_HitReact标签添加进去
				FGameplayTagContainer HitReactTagContainer;
				HitReactTagContainer.AddTag(FAuraGameplayTags::Get().Effect_HitReact);	// 添加受击标签
				EffectProperties.TargetASC->TryActivateAbilitiesByTag(HitReactTagContainer);	// 尝试激活标签的能力
			}

			ShowFloatingText(
				EffectProperties, 
				LocalIncomingDamage,
				UAuraAbilitySystemLibrary::IsBlockedHit(EffectProperties.EffectContextHandle),
				UAuraAbilitySystemLibrary::IsCriticalHit(EffectProperties.EffectContextHandle));	// 显示浮动文字
		}
	}

}

void UAuraAttributeSet::SetEffectsProperties(const FGameplayEffectModCallbackData& Data,
                                             FEffectProperties& EffectProperties) const
{
	// Source = causer of the effect, Target = the actor the effect is applied to
	// Source = 效果的施法者，Target = 效果应用的目标

	EffectProperties.EffectContextHandle = Data.EffectSpec.GetContext();	// 获取效果上下文
	EffectProperties.SourceASC = EffectProperties.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();	// 获取效果的施法者

	if (IsValid(EffectProperties.SourceASC) && EffectProperties.SourceASC->AbilityActorInfo.IsValid() && EffectProperties.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		EffectProperties.SourceAvatarActor = EffectProperties.SourceASC->AbilityActorInfo->AvatarActor.Get();	// 获取施法者的Actor
		if (IsValid(EffectProperties.SourceAvatarActor))
		{
			EffectProperties.SourceController = EffectProperties.SourceASC->AbilityActorInfo->PlayerController.Get();	// 获取施法者的控制器
			if (IsValid(EffectProperties.SourceController))
			{
				EffectProperties.SourceCharacter = Cast<ACharacter>(EffectProperties.SourceController->GetPawn());	// 获取施法者的角色
			}
			if (EffectProperties.SourceController == nullptr && EffectProperties.SourceAvatarActor->IsA<ACharacter>())
			{
				EffectProperties.SourceCharacter = Cast<ACharacter>(EffectProperties.SourceAvatarActor);	// 获取施法者的角色
				EffectProperties.SourceController = EffectProperties.SourceCharacter->GetController();		// 获取施法者的控制器
			}
		}
	}

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		EffectProperties.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();	// 获取目标的Actor
		EffectProperties.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();	// 获取目标的控制器
		EffectProperties.TargetCharacter = Cast<ACharacter>(EffectProperties.TargetAvatarActor);	// 获取目标的角色

		EffectProperties.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(EffectProperties.TargetAvatarActor);	// 获取目标的能力系统组件
	}

}

void UAuraAttributeSet::ShowFloatingText(const FEffectProperties Props, float Damage, bool bBlockedHit, bool bCriticalHit) const
{
	if (Props.TargetCharacter)
	{
		if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(UGameplayStatics::GetPlayerController(Props.TargetCharacter, 0)))
		{
			AuraPlayerController->ShowDamageText(Damage, Props.TargetCharacter, bBlockedHit, bCriticalHit);	// 显示伤害文本
		}
	}
}

void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	/*
	 * Primary Attributes
	 */
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Strength, COND_None, REPNOTIFY_Always);	// 设置属性的复制方式，传入属性名，复制条件，通知方式
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Agility, COND_None, REPNOTIFY_Always);	// 设置属性的复制方式，传入属性名，复制条件，通知方式
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Intelligence, COND_None, REPNOTIFY_Always);	// 设置属性的复制方式，传入属性名，复制条件，通知方式
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Vigor, COND_None, REPNOTIFY_Always);	// 设置属性的复制方式，传入属性名，复制条件，通知方式
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Resilience, COND_None, REPNOTIFY_Always);	// 设置属性的复制方式，传入属性名，复制条件，通知方式
	
	
	/*
	 * Secondary Attributes (Derived Attributes)
	 */
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);	// 设置属性的复制方式，传入属性名，复制条件，通知方式
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);	// 设置属性的复制方式，传入属性名，复制条件，通知方式

	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Armor, COND_None, REPNOTIFY_Always);	// 设置属性的复制方式，传入属性名，复制条件，通知方式
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ArmorPenetration, COND_None, REPNOTIFY_Always);	// 设置属性的复制方式，传入属性名，复制条件，通知方式
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, BlockChance, COND_None, REPNOTIFY_Always);	// 设置属性的复制方式，传入属性名，复制条件，通知方式
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitChance, COND_None, REPNOTIFY_Always);	// 设置属性的复制方式，传入属性名，复制条件，通知方式
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitDamage, COND_None, REPNOTIFY_Always);	// 设置属性的复制方式，传入属性名，复制条件，通知方式
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitResistance, COND_None, REPNOTIFY_Always);	// 设置属性的复制方式，传入属性名，复制条件，通知方式
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, HealthRegeneration, COND_None, REPNOTIFY_Always);	// 设置属性的复制方式，传入属性名，复制条件，通知方式
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ManaRegeneration, COND_None, REPNOTIFY_Always);	// 设置属性的复制方式，传入属性名，复制条件，通知方式

	/*
	 * Vital Attributes
	 */
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Health, COND_None, REPNOTIFY_Always);	// 设置属性的复制方式，传入属性名，复制条件，通知方式
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Mana, COND_None, REPNOTIFY_Always);	// 设置属性的复制方式，传入属性名，复制条件，通知方式
	
}

void UAuraAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Strength, OldStrength);	// 通知属性变化，传入属性名，新值和旧值
}

void UAuraAttributeSet::OnRep_Agility(const FGameplayAttributeData& OldAgility) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Agility, OldAgility);	// 通知属性变化，传入属性名，新值和旧值
}

void UAuraAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Intelligence, OldIntelligence);	// 通知属性变化，传入属性名，新值和旧值
}

void UAuraAttributeSet::OnRep_Vigor(const FGameplayAttributeData& OldVigor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Vigor, OldVigor);	// 通知属性变化，传入属性名，新值和旧值
}

void UAuraAttributeSet::OnRep_Resilience(const FGameplayAttributeData& OldResilience) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Resilience, OldResilience);	// 通知属性变化，传入属性名，新值和旧值
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

void UAuraAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Armor, OldArmor);	// 通知属性变化，传入属性名，新值和旧值
}

void UAuraAttributeSet::OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ArmorPenetration, OldArmorPenetration);	// 通知属性变化，传入属性名，新值和旧值
}

void UAuraAttributeSet::OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, BlockChance, OldBlockChance);	// 通知属性变化，传入属性名，新值和旧值
}

void UAuraAttributeSet::OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalHitChance, OldCriticalHitChance);	// 通知属性变化，传入属性名，新值和旧值
}

void UAuraAttributeSet::OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalHitDamage, OldCriticalHitDamage);	// 通知属性变化，传入属性名，新值和旧值
}

void UAuraAttributeSet::OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalHitResistance, OldCriticalHitResistance);	// 通知属性变化，传入属性名，新值和旧值
}

void UAuraAttributeSet::OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, HealthRegeneration, OldHealthRegeneration);	// 通知属性变化，传入属性名，新值和旧值
}

void UAuraAttributeSet::OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ManaRegeneration, OldManaRegeneration);	// 通知属性变化，传入属性名，新值和旧值
}

