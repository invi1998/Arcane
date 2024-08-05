// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"

class UAuraGameplayAbility;
class UMenuSaveGame;

// 定义一个委托，用于在效果应用到目标时调用，该委托用于广播效果的标签
DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer& /* Asset Tags */);
DECLARE_MULTICAST_DELEGATE(FAbilitiesGiven);	// 定义一个委托，用于在给角色添加能力时调用
DECLARE_DELEGATE_OneParam(FForEachAbility, const FGameplayAbilitySpec&);	// 为每个能力定义一个委托
DECLARE_MULTICAST_DELEGATE_TwoParams(FAbilityInputTagChanged, const FGameplayTag& /* Input Tag */, bool /* Pressed */);	// 定义一个委托，用于在技能输入标签改变时调用
DECLARE_MULTICAST_DELEGATE_ThreeParams(FAbilityStatusChanged, const FGameplayTag& /* State Tag */, const FGameplayTag& /* Ability Tag */, const int32 /* Ability Level */);	// 定义一个委托，用于在技能状态改变时调用
DECLARE_MULTICAST_DELEGATE_FourParams(FAbilitySlotChanged, const FGameplayTag& /* Ability Tag */, const FGameplayTag& /* Status */, const FGameplayTag& /* Slot Tag */, const FGameplayTag& /* Previous Slot Tag */);	// 定义一个委托，用于在技能槽改变时调用

DECLARE_MULTICAST_DELEGATE_OneParam(FDeactivatePassiveAbilities, const FGameplayTag& /* Ability Tag */);	// 定义一个委托，用于在被动技能失效时调用

DECLARE_MULTICAST_DELEGATE_TwoParams(FActivatePassiveEffect, const FGameplayTag&, /* Ability Tag */bool /* 是否激活 */);		// 定义一个委托，用于在被动效果激活或者失效时调用

// 该委托用于广播技能状态（开始施法，施法中，施法结束）
DECLARE_MULTICAST_DELEGATE_OneParam(FAbilityCastStart, const FGameplayTag& /* Ability Tag */);
DECLARE_MULTICAST_DELEGATE_OneParam(FAbilityCastEnd, const FGameplayTag& /* Ability Tag */);
// 监听技能槽改变
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnAbilitySlotChangeDelegate, UAbilitySystemComponent*, const FGameplayTag& /*old*/, const FGameplayTag&/*new*/);


/**
 * 
 */
UCLASS()
class ARCANE_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	// 当调用这个函数时，我们知道这个角色的AbilityActorInfo已经被设置了
	void AbilityActorInfoSet();

	FEffectAssetTags EffectAssetTags;	// 定义一个委托，用于在效果应用到目标时调用，该委托用于广播效果的标签
	FAbilitiesGiven AbilitiesGivenDelegate;	// 定义一个委托，用于在给角色添加能力时调用
	FAbilityStatusChanged AbilityStatusChangedDelegate;	// 定义一个委托，用于在技能状态改变时调用
	FAbilitySlotChanged AbilitySlotChangedDelegate;	// 定义一个委托，用于在技能槽改变时调用

	FDeactivatePassiveAbilities DeactivatePassiveAbilitiesDelegate;	// 定义一个委托，用于在被动技能失效时调用
	
	FActivatePassiveEffect ActivatePassiveEffectDelegate;	// 定义一个委托，用于在被动效果激活或者失效时调用

	FAbilityCastStart AbilityCastStartDelegate;	// 定义一个委托，用于在技能释放开始时调用
	FAbilityCastEnd AbilityCastEndDelegate;	// 定义一个委托，用于在技能释放结束时调用

	FOnAbilitySlotChangeDelegate OnAbilitySlotChangeDelegate;	// 监听技能槽改变

	UFUNCTION(NetMulticast, Unreliable)		// 因为该函数只是用于特效的通知，所以不需要可靠性
	void MulticastActivatePassiveEffect(const FGameplayTag& AbilityTag, bool bActivate);	// 多播激活被动效果

	void AddCharacterAbilities(const TArray<TSubclassOf<UAuraGameplayAbility>>& StartupAbilities);		// 添加角色的能力，这些能力在角色创建时就会被添加

	void AddCharacterPassiveAbilities(const TArray<TSubclassOf<UAuraGameplayAbility>>& StartupAbilities);	// 添加角色的被动能力，这些能力在角色创建时就会被添加

	void AddCharacterAbilitiesFromSaveData(UMenuSaveGame* SaveGame);	// 从存档数据中添加角色的能力

	bool bStartupAbilitiesGiven = false;	// 是否已经添加了角色的能力

	void AbilityInputTagPressed(const FGameplayTag& InputTag);	// 技能输入标签按下
	void AbilityInputTagReleased(const FGameplayTag& InputTag);	// 技能输入标签释放
	void AbilityInputTagHeld(const FGameplayTag& InputTag);	// 技能输入标签按住
	TArray<FGameplayTag> GetAllInputTagsByAbilitySpec(const FGameplayAbilitySpec& Spec);	// 通过AbilitySpec获取所有的输入标签

	void ForEachAbility(const FForEachAbility& Delegate);	// 为每个能力调用一个委托)

	static FGameplayTag GetAbilityTagBySpec(const FGameplayAbilitySpec& Spec);	// 通过AbilitySpec获取能力标签
	static FGameplayTag GetAbilityInputTagBySpec(const FGameplayAbilitySpec& Spec);	// 通过AbilitySpec获取能力输入标签
	static FGameplayTag GetAbilityStateTag(const FGameplayAbilitySpec& Spec);	// 获取能力状态标签
	
	FGameplayTag GetStatusTagByAbilityTag(const FGameplayTag& AbilityTag);	// 通过能力标签获取状态标签
	FGameplayTag GetInputTagByAbilityTag(const FGameplayTag& AbilityTag);	// 通过能力标签获取输入标签
	FGameplayTag GetAbilityTagByInputTag(const FGameplayTag& InputTag);		// 通过输入标签获取能力标签
	FGameplayAbilitySpec* FindAbilitySpecByInputTag(const FGameplayTag& InputTag);	// 通过输入标签查找能力
	FGameplayAbilitySpec* FindAbilitySpecByTag(const FGameplayTag& AbilityTag);	// 通过标签查找能力
	bool IsEmptySlot(const FGameplayTag& SlotTag);	// 指定插槽是否为空
	bool IsPassiveAbility(const FGameplayAbilitySpec* Spec) const;	// 是否是被动技能
	static bool AbilityHasAnyInputTag(const FGameplayAbilitySpec* Spec);	// 技能是否有输入标签
	static void AssignSlotToAbility(FGameplayAbilitySpec& AbilitySpec, const FGameplayTag& SlotTag);	// 将技能分配到插槽

	// 通过AbilityTag获取技能的CoolDownTag
	FGameplayTag GetCoolDownTagByAbilityTag(const FGameplayTag& AbilityTag);

	int32 GetAbilityLevelByTag(const FGameplayTag& AbilityTag);	// 通过标签获取技能等级

	void UpgradeAttribute(const FGameplayTag& AttributeTag);	// 升级属性

	UFUNCTION(BlueprintCallable)
	float GetManaCostByAbilityTag(const FGameplayTag& AbilityTag);	// 通过标签获取技能的法力消耗

	UFUNCTION(Server, Reliable)
	void ServerUpgradeAttribute(const FGameplayTag& AttributeTag);	// 服务端升级属性

	// 依据等级更新能力状态标签
	void UpdateAbilityStateTags(int32 NewLevel);

	UFUNCTION(Server, Reliable)
	void ServerSpendSkillPoint(const FGameplayTag& AbilityTag);	// 服务端消耗技能点

	UFUNCTION(Server, Reliable)
	void ServerEquipAbility(const FGameplayTag& AbilityTag, const FGameplayTag& SlotTag);	// 服务端装备技能

	UFUNCTION(Client, Reliable)
	void ClientEquipAbility(const FGameplayTag& AbilityTag, const FGameplayTag& Status, const FGameplayTag& SlotTag, const FGameplayTag& PreviousSlotTag);	// 客户端装备技能

	static void ClearSlot(FGameplayAbilitySpec* Spec);	// 清除槽
	void ClearAbilityOnSlot(const FGameplayTag& SlotTag);	// 清除槽上的技能
	static bool AbilityHasSlot(const FGameplayAbilitySpec* Spec, const FGameplayTag& SlotTag);	// 技能是否有槽

	bool GetDescriptionByTag(const FGameplayTag& AbilityTag, FString& OutDescription, FString& OutNextLevelDescription);	// 通过标签获取技能描述

protected:
	// 在效果应用到目标时调用
	UFUNCTION(Client, Reliable)
	void ClientEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);
	
	virtual void OnRep_ActivateAbilities() override;	// 当激活能力时调用，用于在客户端和服务器之间同步激活的能力

	UFUNCTION(Client, Reliable)
	void ClientUpdateAbilityStateTags(const FGameplayTag& AbilityTag, const FGameplayTag& StateTag, const int32 AbilityLevel);	// 客户端更新能力状态标签

};

