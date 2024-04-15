// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "AuraEnemy.generated.h"

class UWidgetComponent;

/**
 * 
 */
UCLASS()
class ARCANE_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()

public:
	AAuraEnemy();

	/*begin Enemy �ӿ�*/
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	/*end Enemy �ӿ�*/

	/** CombatInterface begin */
	virtual int32 GetPlayerLevel() const override;
	/** CombatInterface end */

	UPROPERTY(BlueprintAssignable, Category="Combat")
	FOnAttributeChangeSignature OnHealthChanged;	// ��������ֵ�ı�

	UPROPERTY(BlueprintAssignable, Category = "Combat")
	FOnAttributeChangeSignature OnMaxHealthChanged;	// �����������ֵ�ı�

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Default")
	float LifeSpan = 5.0f;	// ��������

	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);	//	�ܻ���Ӧ��ǩ�ı�

	virtual void Die() override;

protected:
	virtual void BeginPlay() override;

	virtual void InitAbilityActorInfo() override;

	virtual void InitializeDefaultAttributes() const override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Default")
	int32 Level = 1;	// ���˵ȼ�

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Default")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;	// ����ְҵ

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;

private:
	
	
};
