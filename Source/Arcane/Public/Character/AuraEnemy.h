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

	/*begin Enemy 接口*/
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	/*end Enemy 接口*/

	/** CombatInterface begin */
	virtual int32 GetPlayerLevel() const override;
	/** CombatInterface end */

	UPROPERTY(BlueprintAssignable, Category="Combat")
	FOnAttributeChangeSignature OnHealthChanged;	// 怪物生命值改变

	UPROPERTY(BlueprintAssignable, Category = "Combat")
	FOnAttributeChangeSignature OnMaxHealthChanged;	// 怪物最大生命值改变

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Default")
	float LifeSpan = 5.0f;	// 生命周期

	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);	//	受击反应标签改变

	virtual void Die() override;

protected:
	virtual void BeginPlay() override;

	virtual void InitAbilityActorInfo() override;

	virtual void InitializeDefaultAttributes() const override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Default")
	int32 Level = 1;	// 敌人等级

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Default")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;	// 敌人职业

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;

private:
	
	
};
