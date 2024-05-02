// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "AuraPlayerState.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
class ULevelUpInfo;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnplayerStatChanged, int32/*状态值（exp，level..)*/);

/**
 * 
 */
UCLASS()
class ARCANE_API AAuraPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AAuraPlayerState();
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;	// 获取生命周期复制属性

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;		// 获取能力系统组件
	UAttributeSet* GetAttributeSet() const;												// 获取属性集

	UPROPERTY(EditDefaultsOnly, Category="Attributes")
	TObjectPtr<ULevelUpInfo> LevelUpInfo;		// 等级升级信息

	FOnplayerStatChanged OnExpChangedDelegate;		// 经验改变事件
	FOnplayerStatChanged OnLevelChangedDelegate;	// 等级改变事件
	FOnplayerStatChanged OnAttributePointChangedDelegate;	// 属性点改变事件
	FOnplayerStatChanged OnSkillPointChangedDelegate;		// 技能点改变事件

	FORCEINLINE int32 GetPlayerLevel() const { return Level; }		// 获取等级
	FORCEINLINE int32 GetPlayerEXP() const { return EXP; }			// 获取经验
	FORCEINLINE int32 GetAttributePoints() const { return AttributePoints; }	// 获取属性点
	FORCEINLINE int32 GetSkillPoints() const { return SkillPoints; }		// 获取技能点

	void AddEXP(int32 Value);		// 增加经验
	void SetEXP(int32 Value);		// 设置经验

	void AddLevel(int32 Value);		// 增加等级
	void SetLevel(int32 Value);		// 设置等级

	void AddAttributePoint(int32 Value);		// 增加属性点
	void SetAttributePoint(int32 Value);		// 设置属性点

	void AddSkillPoint(int32 Value);		// 增加技能点
	void SetSkillPoint(int32 Value);		// 设置技能点

protected:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;		// 能力系统组件

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;							// 属性集

private:
	UPROPERTY(ReplicatedUsing= OnRep_Level, EditAnywhere, Category="Attributes")
	int32 Level = 1;		// 玩家等级

	UFUNCTION()
	void OnRep_Level(int32 OldLevel);	// 当玩家等级被复制时调用

	UPROPERTY(ReplicatedUsing = OnRep_EXP, EditAnywhere, Category = "Attributes")
	int32 EXP = 0;		// 玩家经验

	UFUNCTION()
	void OnRep_EXP(int32 OldEXP);	// 当玩家经验被复制时调用

	UPROPERTY(ReplicatedUsing = OnRep_AttributePoint, EditAnywhere, Category = "PlayerStat")
	int32 AttributePoints = 0;		// 属性点

	UFUNCTION()
	void OnRep_AttributePoint(int32 OldAttributePoint);	// 当属性点被复制时调用

	UPROPERTY(ReplicatedUsing = OnRep_SkillPoint, EditAnywhere, Category = "PlayerStat")
	int32 SkillPoints = 0;		// 技能点

	UFUNCTION()
	void OnRep_SkillPoint(int32 OldSkillPoint);	// 当技能点被复制时调用

};
