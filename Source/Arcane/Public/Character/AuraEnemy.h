// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
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

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangeSignature OnHealthChanged;	// ��������ֵ�ı�

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangeSignature OnMaxHealthChanged;	// �����������ֵ�ı�

protected:
	virtual void BeginPlay() override;

	virtual void InitAbilityActorInfo() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Default")
	int32 Level = 1;	// ���˵ȼ�

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;

private:
	
	
};
