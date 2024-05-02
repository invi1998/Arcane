// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "AuraPlayerState.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
class ULevelUpInfo;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnplayerStatChanged, int32/*״ֵ̬��exp��level..)*/);

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

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;	// ��ȡ�������ڸ�������

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;		// ��ȡ����ϵͳ���
	UAttributeSet* GetAttributeSet() const;												// ��ȡ���Լ�

	UPROPERTY(EditDefaultsOnly, Category="Attributes")
	TObjectPtr<ULevelUpInfo> LevelUpInfo;		// �ȼ�������Ϣ

	FOnplayerStatChanged OnExpChangedDelegate;		// ����ı��¼�
	FOnplayerStatChanged OnLevelChangedDelegate;	// �ȼ��ı��¼�
	FOnplayerStatChanged OnAttributePointChangedDelegate;	// ���Ե�ı��¼�
	FOnplayerStatChanged OnSkillPointChangedDelegate;		// ���ܵ�ı��¼�

	FORCEINLINE int32 GetPlayerLevel() const { return Level; }		// ��ȡ�ȼ�
	FORCEINLINE int32 GetPlayerEXP() const { return EXP; }			// ��ȡ����
	FORCEINLINE int32 GetAttributePoints() const { return AttributePoints; }	// ��ȡ���Ե�
	FORCEINLINE int32 GetSkillPoints() const { return SkillPoints; }		// ��ȡ���ܵ�

	void AddEXP(int32 Value);		// ���Ӿ���
	void SetEXP(int32 Value);		// ���þ���

	void AddLevel(int32 Value);		// ���ӵȼ�
	void SetLevel(int32 Value);		// ���õȼ�

	void AddAttributePoint(int32 Value);		// �������Ե�
	void SetAttributePoint(int32 Value);		// �������Ե�

	void AddSkillPoint(int32 Value);		// ���Ӽ��ܵ�
	void SetSkillPoint(int32 Value);		// ���ü��ܵ�

protected:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;		// ����ϵͳ���

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;							// ���Լ�

private:
	UPROPERTY(ReplicatedUsing= OnRep_Level, EditAnywhere, Category="Attributes")
	int32 Level = 1;		// ��ҵȼ�

	UFUNCTION()
	void OnRep_Level(int32 OldLevel);	// ����ҵȼ�������ʱ����

	UPROPERTY(ReplicatedUsing = OnRep_EXP, EditAnywhere, Category = "Attributes")
	int32 EXP = 0;		// ��Ҿ���

	UFUNCTION()
	void OnRep_EXP(int32 OldEXP);	// ����Ҿ��鱻����ʱ����

	UPROPERTY(ReplicatedUsing = OnRep_AttributePoint, EditAnywhere, Category = "PlayerStat")
	int32 AttributePoints = 0;		// ���Ե�

	UFUNCTION()
	void OnRep_AttributePoint(int32 OldAttributePoint);	// �����Ե㱻����ʱ����

	UPROPERTY(ReplicatedUsing = OnRep_SkillPoint, EditAnywhere, Category = "PlayerStat")
	int32 SkillPoints = 0;		// ���ܵ�

	UFUNCTION()
	void OnRep_SkillPoint(int32 OldSkillPoint);	// �����ܵ㱻����ʱ����

};
