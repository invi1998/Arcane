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
#include "Player/AuraPlayerState.h"

UAuraAttributeSet::UAuraAttributeSet()
{
	// const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();	// ��ȡ��ǩ

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

	FEffectProperties EffectProperties;
	SetEffectsProperties(Data, EffectProperties);	// ����Ч������

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
			// ����˺�
			const float NewHealth = GetHealth() - LocalIncomingDamage;
			SetHealth(FMath::Clamp(NewHealth, 0.0f, GetMaxHealth()));

			const bool bFatal = GetHealth() <= 0.0f;	// �Ƿ�����

			// ������������ģ���ô�ͼ������Ability
			if (bFatal)
			{
				// ����һ��TagContainer����Effect_Fatal��ǩ��ӽ�ȥ
				FGameplayTagContainer FatalTagContainer;
				FatalTagContainer.AddTag(FAuraGameplayTags::Get().Effect_DeathReact);	// ���������ǩ
				EffectProperties.TargetASC->TryActivateAbilitiesByTag(FatalTagContainer);	// ���Լ����ǩ������
				if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(EffectProperties.TargetAvatarActor))
				{
					CombatInterface->Die();	// ����
				}
				SendEXPEvent(EffectProperties);	// ���;����¼�
			}
			else
			{
				// ����һ��TagContainer����Effect_HitReact��ǩ��ӽ�ȥ
				FGameplayTagContainer HitReactTagContainer;
				HitReactTagContainer.AddTag(FAuraGameplayTags::Get().Effect_HitReact);	// ����ܻ���ǩ
				EffectProperties.TargetASC->TryActivateAbilitiesByTag(HitReactTagContainer);	// ���Լ����ǩ������
			}

			ShowFloatingText(
				EffectProperties, 
				LocalIncomingDamage,
				UAuraAbilitySystemLibrary::IsBlockedHit(EffectProperties.EffectContextHandle),
				UAuraAbilitySystemLibrary::IsCriticalHit(EffectProperties.EffectContextHandle));	// ��ʾ��������
		}
	}

	if (Data.EvaluatedData.Attribute == GetRewardExperienceAttribute())
	{
		const float LocalRewardExperience = GetRewardExperience();
		SetRewardExperience(0.0f);
		if (LocalRewardExperience > 0.0f)
		{
			// ��������
			AAuraPlayerState* AuraPlayerState = Cast<AAuraPlayerState>(EffectProperties.TargetController->PlayerState);

			AuraPlayerState->AddEXP(LocalRewardExperience);	// ���Ӿ���
		}
	}

}

void UAuraAttributeSet::SetEffectsProperties(const FGameplayEffectModCallbackData& Data,
                                             FEffectProperties& EffectProperties) const
{
	// Source = causer of the effect, Target = the actor the effect is applied to
	// Source = Ч����ʩ���ߣ�Target = Ч��Ӧ�õ�Ŀ��

	EffectProperties.EffectContextHandle = Data.EffectSpec.GetContext();	// ��ȡЧ��������
	EffectProperties.SourceASC = EffectProperties.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();	// ��ȡЧ����ʩ����

	if (IsValid(EffectProperties.SourceASC) && EffectProperties.SourceASC->AbilityActorInfo.IsValid() && EffectProperties.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		EffectProperties.SourceAvatarActor = EffectProperties.SourceASC->AbilityActorInfo->AvatarActor.Get();	// ��ȡʩ���ߵ�Actor
		if (IsValid(EffectProperties.SourceAvatarActor))
		{
			EffectProperties.SourceController = EffectProperties.SourceASC->AbilityActorInfo->PlayerController.Get();	// ��ȡʩ���ߵĿ�����
			if (IsValid(EffectProperties.SourceController))
			{
				EffectProperties.SourceCharacter = Cast<ACharacter>(EffectProperties.SourceController->GetPawn());	// ��ȡʩ���ߵĽ�ɫ
			}
			if (EffectProperties.SourceController == nullptr && EffectProperties.SourceAvatarActor->IsA<ACharacter>())
			{
				EffectProperties.SourceCharacter = Cast<ACharacter>(EffectProperties.SourceAvatarActor);	// ��ȡʩ���ߵĽ�ɫ
				EffectProperties.SourceController = EffectProperties.SourceCharacter->GetController();		// ��ȡʩ���ߵĿ�����
			}
		}
	}

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		EffectProperties.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();	// ��ȡĿ���Actor
		EffectProperties.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();	// ��ȡĿ��Ŀ�����
		EffectProperties.TargetCharacter = Cast<ACharacter>(EffectProperties.TargetAvatarActor);	// ��ȡĿ��Ľ�ɫ

		EffectProperties.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(EffectProperties.TargetAvatarActor);	// ��ȡĿ�������ϵͳ���
	}

}

void UAuraAttributeSet::ShowFloatingText(const FEffectProperties Props, float Damage, bool bBlockedHit, bool bCriticalHit) const
{
	if (Props.TargetCharacter)
	{
		// ���Ŀ���ɫ�Ŀ�������AAuraPlayerController����ҿ�������
		if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(Props.SourceCharacter->GetController()))
		{
			AuraPlayerController->ShowDamageText(Damage, Props.TargetCharacter, bBlockedHit, bCriticalHit);	// ��ʾ�˺��ı�
			return;
		}

		// ���Ŀ���ɫ�Ŀ�������AAuraPlayerController ��NPC
		if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(Props.TargetCharacter->GetController()))
		{
			AuraPlayerController->ShowDamageText(Damage, Props.TargetCharacter, bBlockedHit, bCriticalHit);	// ��ʾ�˺��ı�
			return;
		}
	}
}

void UAuraAttributeSet::SendEXPEvent(const FEffectProperties& Props) const
{
	if (Props.TargetCharacter)
	{
		// ��ȡCombat
		if (const ICombatInterface* CombatInterface = Cast<ICombatInterface>(Props.TargetCharacter))
		{
			// `CombatInterface->GetCharacterClass(), CombatInterface->GetCharacterLevel()`����ط���ͨ��Combat�ӿڻ�ȡ��ɫְҵ�����ǲ���ֱ�ӵ�������ԭʼ�汾�ĺ�����
			// ��Ϊ����ӿڱ�����Ϊ��ͼԭ���¼���BlueprintNative��,��������Ҫ���õ�����ִ�а汾��Execute_GetCharacterClass����
			// ��GetPlayerLevel()��һ����ͨ���麯���������������þ���
			const ECharacterClass CharacterClass = CombatInterface->Execute_GetCharacterClass(Props.SourceAvatarActor);	// ��ȡ��ɫְҵ
			// ��ȡNPC���齱��
			int32 EXP = UAuraAbilitySystemLibrary::GetMonsterEXPRewardByClassAndLv(this, CharacterClass, CombatInterface->GetCharacterLevel());

			const FAuraGameplayTags& AuraGameplayTags = FAuraGameplayTags::Get();	// ��ȡ��ǩ

			// ����Payload
			FGameplayEventData Payload;
			Payload.EventTag = AuraGameplayTags.Attributes_Meta_RewardExperience;
			Payload.EventMagnitude = EXP;

			// ������Ϸ�¼�
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Props.SourceCharacter, AuraGameplayTags.Attributes_Meta_RewardExperience, Payload);
		}

	
	}
}

void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	/*
	 * Primary Attributes
	 */
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Strength, COND_None, REPNOTIFY_Always);	// �������Եĸ��Ʒ�ʽ������������������������֪ͨ��ʽ
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Agility, COND_None, REPNOTIFY_Always);	// �������Եĸ��Ʒ�ʽ������������������������֪ͨ��ʽ
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Intelligence, COND_None, REPNOTIFY_Always);	// �������Եĸ��Ʒ�ʽ������������������������֪ͨ��ʽ
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Vigor, COND_None, REPNOTIFY_Always);	// �������Եĸ��Ʒ�ʽ������������������������֪ͨ��ʽ
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Resilience, COND_None, REPNOTIFY_Always);	// �������Եĸ��Ʒ�ʽ������������������������֪ͨ��ʽ
	
	
	/*
	 * Secondary Attributes (Derived Attributes)
	 */
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);	// �������Եĸ��Ʒ�ʽ������������������������֪ͨ��ʽ
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);	// �������Եĸ��Ʒ�ʽ������������������������֪ͨ��ʽ

	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Armor, COND_None, REPNOTIFY_Always);	// �������Եĸ��Ʒ�ʽ������������������������֪ͨ��ʽ
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ArmorPenetration, COND_None, REPNOTIFY_Always);	// �������Եĸ��Ʒ�ʽ������������������������֪ͨ��ʽ
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, BlockChance, COND_None, REPNOTIFY_Always);	// �������Եĸ��Ʒ�ʽ������������������������֪ͨ��ʽ
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitChance, COND_None, REPNOTIFY_Always);	// �������Եĸ��Ʒ�ʽ������������������������֪ͨ��ʽ
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitDamage, COND_None, REPNOTIFY_Always);	// �������Եĸ��Ʒ�ʽ������������������������֪ͨ��ʽ
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitResistance, COND_None, REPNOTIFY_Always);	// �������Եĸ��Ʒ�ʽ������������������������֪ͨ��ʽ
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, HealthRegeneration, COND_None, REPNOTIFY_Always);	// �������Եĸ��Ʒ�ʽ������������������������֪ͨ��ʽ
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ManaRegeneration, COND_None, REPNOTIFY_Always);	// �������Եĸ��Ʒ�ʽ������������������������֪ͨ��ʽ

	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, FireResistance, COND_None, REPNOTIFY_Always);	// �������Եĸ��Ʒ�ʽ������������������������֪ͨ��ʽ
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, IceResistance, COND_None, REPNOTIFY_Always);	// �������Եĸ��Ʒ�ʽ������������������������֪ͨ��ʽ
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, LightningResistance, COND_None, REPNOTIFY_Always);	// �������Եĸ��Ʒ�ʽ������������������������֪ͨ��ʽ
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, PoisonResistance, COND_None, REPNOTIFY_Always);	// �������Եĸ��Ʒ�ʽ������������������������֪ͨ��ʽ
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ArcaneResistance, COND_None, REPNOTIFY_Always);	// �������Եĸ��Ʒ�ʽ������������������������֪ͨ��ʽ
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, PhysicalResistance, COND_None, REPNOTIFY_Always);	// �������Եĸ��Ʒ�ʽ������������������������֪ͨ��ʽ

	/*
	 * Vital Attributes
	 */
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Health, COND_None, REPNOTIFY_Always);	// �������Եĸ��Ʒ�ʽ������������������������֪ͨ��ʽ
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Mana, COND_None, REPNOTIFY_Always);	// �������Եĸ��Ʒ�ʽ������������������������֪ͨ��ʽ
	
}

void UAuraAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Strength, OldStrength);	// ֪ͨ���Ա仯����������������ֵ�;�ֵ
}

void UAuraAttributeSet::OnRep_Agility(const FGameplayAttributeData& OldAgility) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Agility, OldAgility);	// ֪ͨ���Ա仯����������������ֵ�;�ֵ
}

void UAuraAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Intelligence, OldIntelligence);	// ֪ͨ���Ա仯����������������ֵ�;�ֵ
}

void UAuraAttributeSet::OnRep_Vigor(const FGameplayAttributeData& OldVigor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Vigor, OldVigor);	// ֪ͨ���Ա仯����������������ֵ�;�ֵ
}

void UAuraAttributeSet::OnRep_Resilience(const FGameplayAttributeData& OldResilience) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Resilience, OldResilience);	// ֪ͨ���Ա仯����������������ֵ�;�ֵ
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

void UAuraAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Armor, OldArmor);	// ֪ͨ���Ա仯����������������ֵ�;�ֵ
}

void UAuraAttributeSet::OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ArmorPenetration, OldArmorPenetration);	// ֪ͨ���Ա仯����������������ֵ�;�ֵ
}

void UAuraAttributeSet::OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, BlockChance, OldBlockChance);	// ֪ͨ���Ա仯����������������ֵ�;�ֵ
}

void UAuraAttributeSet::OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalHitChance, OldCriticalHitChance);	// ֪ͨ���Ա仯����������������ֵ�;�ֵ
}

void UAuraAttributeSet::OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalHitDamage, OldCriticalHitDamage);	// ֪ͨ���Ա仯����������������ֵ�;�ֵ
}

void UAuraAttributeSet::OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalHitResistance, OldCriticalHitResistance);	// ֪ͨ���Ա仯����������������ֵ�;�ֵ
}

void UAuraAttributeSet::OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, HealthRegeneration, OldHealthRegeneration);	// ֪ͨ���Ա仯����������������ֵ�;�ֵ
}

void UAuraAttributeSet::OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ManaRegeneration, OldManaRegeneration);	// ֪ͨ���Ա仯����������������ֵ�;�ֵ
}

void UAuraAttributeSet::OnRep_FireResistance(const FGameplayAttributeData& OldFireResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, FireResistance, OldFireResistance);	// ֪ͨ���Ա仯����������������ֵ�;�ֵ
}

void UAuraAttributeSet::OnRep_IceResistance(const FGameplayAttributeData& OldIceResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, IceResistance, OldIceResistance);	// ֪ͨ���Ա仯����������������ֵ�;�ֵ
}

void UAuraAttributeSet::OnRep_LightningResistance(const FGameplayAttributeData& OldLightningResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, LightningResistance, OldLightningResistance);	// ֪ͨ���Ա仯����������������ֵ�;�ֵ
}

void UAuraAttributeSet::OnRep_PoisonResistance(const FGameplayAttributeData& OldPoisonResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, PoisonResistance, OldPoisonResistance);	// ֪ͨ���Ա仯����������������ֵ�;�ֵ
}

void UAuraAttributeSet::OnRep_ArcaneResistance(const FGameplayAttributeData& OldArcaneResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ArcaneResistance, OldArcaneResistance);	// ֪ͨ���Ա仯����������������ֵ�;�ֵ
}

void UAuraAttributeSet::OnRep_PhysicalResistance(const FGameplayAttributeData& OldPhysicalResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, PhysicalResistance, OldPhysicalResistance);	// ֪ͨ���Ա仯����������������ֵ�;�ֵ
}

