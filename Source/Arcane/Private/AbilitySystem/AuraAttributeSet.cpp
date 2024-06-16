// Copyright INVI1998


#include "AbilitySystem/AuraAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraAbilityTypes.h"
#include "AuraGameplayTags.h"
#include "GameFramework/Character.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interaction/CombatInterface.h"
#include "Interaction/PlayerInterface.h"
#include "Kismet/GameplayStatics.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"

#include "Net/UnrealNetwork.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"

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

	if (EffectProperties.TargetCharacter->Implements<UCombatInterface>())
	{
		// 判读角色是否死亡
		if (ICombatInterface::Execute_IsDead(EffectProperties.TargetCharacter))
		{
			return;
		}
	}

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
		HandleIncomingDamage(EffectProperties);
	}

	if (Data.EvaluatedData.Attribute == GetRewardExperienceAttribute())
	{
		HandleIncomingExp(EffectProperties);
	}

}

void UAuraAttributeSet::HandleIncomingDamage(const FEffectProperties& Props)
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
			Props.TargetASC->TryActivateAbilitiesByTag(FatalTagContainer);	// 尝试激活标签的能力
			if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(Props.TargetAvatarActor))
			{
				FVector DeathImpulse = UAuraAbilitySystemLibrary::GetDeathImpulse(Props.EffectContextHandle);	// 获取死亡冲量
				CombatInterface->Die(DeathImpulse);	// 死亡
			}
			SendEXPEvent(Props);	// 发送经验事件
		}
		else
		{
			// 创建一个TagContainer，将Effect_HitReact标签添加进去
			FGameplayTagContainer HitReactTagContainer;
			HitReactTagContainer.AddTag(FAuraGameplayTags::Get().Effect_HitReact);	// 添加受击标签
			Props.TargetASC->TryActivateAbilitiesByTag(HitReactTagContainer);	// 尝试激活标签的能力

			const FVector KnockbackImpulse = UAuraAbilitySystemLibrary::GetKnockbackImpulse(Props.EffectContextHandle);	// 获取击退冲量
			if (!KnockbackImpulse.IsZero())
			{
				// 如果击退冲量d不等于0，那么就击退
				Props.TargetCharacter->LaunchCharacter(KnockbackImpulse, true, true);
			}
		}

		ShowFloatingText(
			Props,
			LocalIncomingDamage,
			UAuraAbilitySystemLibrary::IsBlockedHit(Props.EffectContextHandle),
			UAuraAbilitySystemLibrary::IsCriticalHit(Props.EffectContextHandle));	// 显示浮动文字

	}
	if (UAuraAbilitySystemLibrary::IsSuccessfulDebuff(Props.EffectContextHandle))
	{
		Debuff(Props);	// Debuff
	}
}

void UAuraAttributeSet::HandleIncomingExp(const FEffectProperties& Props)
{
	const float LocalRewardExperience = GetRewardExperience();
	SetRewardExperience(0.0f);
	if (LocalRewardExperience > 0.0f)
	{
		if (Props.SourceCharacter->Implements<UPlayerInterface>())	// 如果施法者实现了IPlayerInterface接口
		{
			IPlayerInterface::Execute_AddToEXP(Props.SourceCharacter, LocalRewardExperience);
		}
	}
}

void UAuraAttributeSet::Debuff(const FEffectProperties& Props)
{
	FGameplayEffectContextHandle EffectContextHandle = Props.SourceASC->MakeEffectContext();	// 创建效果上下文，该上下文是专门用于创建Debuff效果的
	EffectContextHandle.AddSourceObject(Props.SourceAvatarActor);	// 添加施法者

	const FAuraGameplayTags& AuraTags = FAuraGameplayTags::Get();	// 获取标签

	const FGameplayTag DamageTypeTag = UAuraAbilitySystemLibrary::GetDamageType(Props.EffectContextHandle);	// 获取伤害标签
	const float DebuffDamage = UAuraAbilitySystemLibrary::GetDebuffDamage(Props.EffectContextHandle);	// 获取Debuff伤害
	const float DebuffDuration = UAuraAbilitySystemLibrary::GetDebuffDuration(Props.EffectContextHandle);	// 获取Debuff持续时间
	const float DebuffFrequency = UAuraAbilitySystemLibrary::GetDebuffFrequency(Props.EffectContextHandle);	// 获取Debuff频率

	const FString DebuffName = FString::Printf(TEXT("DynamicDebuff_%s"), *DamageTypeTag.ToString());	 // 获取Debuff名称
	UGameplayEffect* DebuffEffect = NewObject<UGameplayEffect>(GetTransientPackage(), FName(*DebuffName));	// 创建Debuff效果

	DebuffEffect->DurationPolicy = EGameplayEffectDurationType::HasDuration;	// 设置持续时间策略
	DebuffEffect->Period = DebuffFrequency;	// 设置周期
	DebuffEffect->DurationMagnitude = FScalableFloat(DebuffDuration);	// 设置持续时间

	// 同时，我希望Debuff效果能动态添加GameplayTag，因为我们希望Actor能够知道什么时候受到Debuff，什么时候结束Debuff
	// DebuffEffect->InheritableGameplayEffectTags.AddTag(AuraTags.DamageTypesToDebuff[DamageTypeTag]);	// 添加Debuff标签

	// 因为在UE5.4中，InheritedTagContainer已经被移除，所以我们需要手动添加Tag，代码如下（UE5.3下其实也不能使用InheritedTagContainer了，虽然不报错，但是也不生效）
	FInheritedTagContainer InheritedTags;	// 创建继承标签容器
	const FGameplayTag& DebuffTag = AuraTags.DamageTypesToDebuff[DamageTypeTag];	// 获取Debuff标签
	InheritedTags.Added.AddTag(DebuffTag);	// 添加Debuff标签
	UTargetTagsGameplayEffectComponent& Component = DebuffEffect->AddComponent<UTargetTagsGameplayEffectComponent>();	// 添加目标标签效果组件

	// 如果是StunDebuff，那么就继续添加BlockInput的标签，用来实现眩晕时禁止输入
	if (DebuffTag == AuraTags.Debuff_LightningStun)
	{
		InheritedTags.Added.AddTag(AuraTags.Player_Block_InputPressed);	// 添加BlockInput标签
		InheritedTags.Added.AddTag(AuraTags.Player_Block_CursorTrace);	// 添加BlockInput标签
		InheritedTags.Added.AddTag(AuraTags.Player_Block_InputHeld);	// 添加BlockInput标签
		InheritedTags.Added.AddTag(AuraTags.Player_Block_InputReleased);	// 添加BlockInput标签
	}

	Component.SetAndApplyTargetTagChanges(InheritedTags);	// 设置并应用目标标签更改

	// 设置Effect堆叠类型
	DebuffEffect->StackingType = EGameplayEffectStackingType::AggregateBySource;	// 设置堆叠类型，按来源聚合，也就是说，如果来源相同，那么就堆叠
	DebuffEffect->StackLimitCount = 1;	// 设置堆叠限制

	// 设置Effect修饰器
	const int32 ModIdx = DebuffEffect->Modifiers.Num();
	DebuffEffect->Modifiers.Add(FGameplayModifierInfo());
	FGameplayModifierInfo& ModInfo = DebuffEffect->Modifiers[ModIdx];

	ModInfo.ModifierMagnitude = FScalableFloat(DebuffDamage);		// 设置Modifier的大小, 也就是Debuff伤害
	ModInfo.ModifierOp = EGameplayModOp::Additive;	// 设置Modifier的操作，加法
	ModInfo.Attribute = GetIncomingDamageAttribute();	// 设置Modifier的属性，IncomingDamage

	// 创建EffectSpec
	if (const FGameplayEffectSpec* MutableEffectSpec = new FGameplayEffectSpec(DebuffEffect, EffectContextHandle, 1.f))
	{
		FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(MutableEffectSpec->GetContext().Get());	// 获取效果上下文
		const TSharedPtr<FGameplayTag> DamageTypeTagPtr = MakeShared<FGameplayTag>(DamageTypeTag);	// 创建伤害标签指针
		AuraEffectContext->SetDamageType(DamageTypeTagPtr);	// 设置伤害类型

		Props.TargetASC->ApplyGameplayEffectSpecToSelf(*MutableEffectSpec);	// 对自己应用效果规范
	}

}

void UAuraAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	// 如果是最大生命值或者最大法力值，同时该改变还是在升级节点引起的（也就是说，我们希望升级时回蓝回血），那么就更新生命值或者法力值
	if (Attribute == GetMaxHealthAttribute() && bTopOfHealth)
	{
		SetHealth(GetMaxHealth());
		bTopOfHealth = false;
	}
	else if (Attribute == GetMaxManaAttribute() && bTopOfMana)
	{
		SetMana( GetMaxMana());
		bTopOfMana = false;
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
		// 如果目标角色的控制器是AAuraPlayerController（玩家控制器）
		if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(Props.SourceCharacter->GetController()))
		{
			AuraPlayerController->ShowDamageText(Damage, Props.TargetCharacter, bBlockedHit, bCriticalHit);	// 显示伤害文本
			return;
		}

		// 如果目标角色的控制器是AAuraPlayerController ，NPC
		if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(Props.TargetCharacter->GetController()))
		{
			AuraPlayerController->ShowDamageText(Damage, Props.TargetCharacter, bBlockedHit, bCriticalHit);	// 显示伤害文本
			return;
		}
	}
}

void UAuraAttributeSet::SendEXPEvent(const FEffectProperties& Props) const
{
	if (Props.SourceCharacter && Props.TargetCharacter && Props.TargetCharacter->Implements<UCombatInterface>())
	{
		// 获取Combat
		if (const ICombatInterface* CombatInterface = Cast<ICombatInterface>(Props.TargetCharacter))
		{
			// `CombatInterface->GetCharacterClass(), CombatInterface->GetCharacterLevel()`这个地方，通过Combat接口获取角色职业，我们不能直接调用它的原始版本的函数，
			// 因为这个接口被定义为蓝图原生事件（BlueprintNative）,所以我们要调用的他的执行版本（Execute_GetCharacterClass），
			const ECharacterClass CharacterClass = CombatInterface->Execute_GetCharacterClass(Props.TargetCharacter);	// 获取角色职业
			const int32 Level = CombatInterface->Execute_GetCharacterLevel(Props.TargetCharacter);	// 获取角色等级
			// 获取NPC经验奖励
			int32 EXP = UAuraAbilitySystemLibrary::GetMonsterEXPRewardByClassAndLv(this, CharacterClass, Level);

			const FAuraGameplayTags& AuraGameplayTags = FAuraGameplayTags::Get();	// 获取标签

			// 创建Payload
			FGameplayEventData Payload;
			Payload.EventTag = AuraGameplayTags.Attributes_Meta_RewardExperience;
			Payload.EventMagnitude = EXP;

			// 发送游戏事件
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

	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, FireResistance, COND_None, REPNOTIFY_Always);	// 设置属性的复制方式，传入属性名，复制条件，通知方式
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, IceResistance, COND_None, REPNOTIFY_Always);	// 设置属性的复制方式，传入属性名，复制条件，通知方式
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, LightningResistance, COND_None, REPNOTIFY_Always);	// 设置属性的复制方式，传入属性名，复制条件，通知方式
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, PoisonResistance, COND_None, REPNOTIFY_Always);	// 设置属性的复制方式，传入属性名，复制条件，通知方式
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ArcaneResistance, COND_None, REPNOTIFY_Always);	// 设置属性的复制方式，传入属性名，复制条件，通知方式
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, PhysicalResistance, COND_None, REPNOTIFY_Always);	// 设置属性的复制方式，传入属性名，复制条件，通知方式

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

void UAuraAttributeSet::OnRep_FireResistance(const FGameplayAttributeData& OldFireResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, FireResistance, OldFireResistance);	// 通知属性变化，传入属性名，新值和旧值
}

void UAuraAttributeSet::OnRep_IceResistance(const FGameplayAttributeData& OldIceResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, IceResistance, OldIceResistance);	// 通知属性变化，传入属性名，新值和旧值
}

void UAuraAttributeSet::OnRep_LightningResistance(const FGameplayAttributeData& OldLightningResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, LightningResistance, OldLightningResistance);	// 通知属性变化，传入属性名，新值和旧值
}

void UAuraAttributeSet::OnRep_PoisonResistance(const FGameplayAttributeData& OldPoisonResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, PoisonResistance, OldPoisonResistance);	// 通知属性变化，传入属性名，新值和旧值
}

void UAuraAttributeSet::OnRep_ArcaneResistance(const FGameplayAttributeData& OldArcaneResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ArcaneResistance, OldArcaneResistance);	// 通知属性变化，传入属性名，新值和旧值
}

void UAuraAttributeSet::OnRep_PhysicalResistance(const FGameplayAttributeData& OldPhysicalResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, PhysicalResistance, OldPhysicalResistance);	// 通知属性变化，传入属性名，新值和旧值
}

