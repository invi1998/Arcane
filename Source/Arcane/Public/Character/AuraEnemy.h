// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "AuraEnemy.generated.h"

class UWidgetComponent;
class UBehaviorTree;
class AAuraAIController;

/**
 * 
 */
UCLASS()
class ARCANE_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()

public:
	AAuraEnemy();

	virtual void PossessedBy(AController* NewController) override;	// 被控制器控制

	/*begin Enemy 接口*/
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	/*end Enemy 接口*/

	/** CombatInterface begin */
	virtual int32 GetCharacterLevel_Implementation() const override;
	/** CombatInterface end */

	UPROPERTY(BlueprintAssignable, Category="Combat")
	FOnAttributeChangeSignature OnHealthChanged;	// 怪物生命值改变

	UPROPERTY(BlueprintAssignable, Category = "Combat")
	FOnAttributeChangeSignature OnMaxHealthChanged;	// 怪物最大生命值改变

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Default")
	float LifeSpan = 3.0f;	// 生命周期

	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);	//	受击反应标签改变

	// void DeathTagChanged(const FGameplayTag CallbackTag, int32 NewCount);	//	死亡标签改变

	virtual void Die() override;

	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	TObjectPtr<AActor> CombatTarget;	// 攻击目标

	virtual void SetCombatTarget_Implementation(AActor* NewTarget) override;
	virtual AActor* GetCombatTarget_Implementation() const override;

protected:
	virtual void BeginPlay() override;

	virtual void InitAbilityActorInfo() override;

	virtual void InitializeDefaultAttributes() const override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Default")
	int32 Level = 1;	// 敌人等级

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;

	UPROPERTY(EditAnywhere, Category="AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<AAuraAIController> AuraAIController;

private:
	
	
};
