// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AuraWidgetController.generated.h"

USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()

	FWidgetControllerParams() {}
	FWidgetControllerParams(class APlayerController* InPlayerController, class APlayerState* InPlayerState, class UAbilitySystemComponent* InAbilitySystemComponent, class UAttributeSet* InAttributeSet)
		: PlayerController(InPlayerController), PlayerState(InPlayerState), AbilitySystemComponent(InAbilitySystemComponent), AttributeSet(InAttributeSet) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class APlayerController> PlayerController = nullptr;		// ��ҿ�����

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class APlayerState> PlayerState = nullptr;		// ���״̬

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UAbilitySystemComponent> AbilitySystemComponent = nullptr;		// ����ϵͳ���

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UAttributeSet> AttributeSet = nullptr;		// ���Լ�
};

/**
 * 
 */
UCLASS()
class ARCANE_API UAuraWidgetController : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetControllerParams(const FWidgetControllerParams& InParams);	// ���ÿ���������

	UFUNCTION(BlueprintCallable)
	virtual void BroadcastInitialValues(const FGameplayTag& Tag);		// �㲥��ʼֵ
	virtual void BindCallbacksToDependencies();	// �󶨻ص���������


protected:
	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<class APlayerController> PlayerController;		// ��ҿ�����

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<class APlayerState> PlayerState;		// ���״̬

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<class UAbilitySystemComponent> AbilitySystemComponent;		// ����ϵͳ���

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<class UAttributeSet> AttributeSet;		// ���Լ�

};
