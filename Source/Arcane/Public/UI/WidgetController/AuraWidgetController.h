// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AuraWidgetController.generated.h"

class UAbilityInfo;

class APlayerController;
class APlayerState;
class UAbilitySystemComponent;
class UAttributeSet;

class AAuraPlayerController;
class AAuraPlayerState;
class UAuraAbilitySystemComponent;
class UAuraAttributeSet;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangeSignature, float, NewValue);		// ���Ըı�ί�У�����һ��float���͵���ֵ
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChangeSignatureInt, int32, NewValue);		// ���״̬�ı�ί�У�����һ��int32���͵���ֵ
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityInfoSignature, const FAuraAbilityInfo&, AbilityInfo);		// ������Ϣί�У�һ��������������Ϣ)

USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()

	FWidgetControllerParams() {}
	FWidgetControllerParams(APlayerController* InPlayerController, APlayerState* InPlayerState, UAbilitySystemComponent* InAbilitySystemComponent, UAttributeSet* InAttributeSet)
		: PlayerController(InPlayerController), PlayerState(InPlayerState), AbilitySystemComponent(InAbilitySystemComponent), AttributeSet(InAttributeSet) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerController> PlayerController = nullptr;		// ��ҿ�����

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerState> PlayerState = nullptr;		// ���״̬

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;		// ����ϵͳ���

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAttributeSet> AttributeSet = nullptr;		// ���Լ�
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

	UPROPERTY(BlueprintAssignable, Category = "GAS|Abilities")		// ����Ϊ��ͼ�ɷ��䣬����ΪGAS�µ�Abilities
	FAbilityInfoSignature AbilityInfoDelegate;		// ������Ϣί��

	void BroadcastAbilityInfo();		// �㲥������Ϣ

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widget Data")		// ����Ϊ�ɱ༭���κεط�����ͼ�ɶ�
	TObjectPtr<UAbilityInfo> AbilityInformation;		// ������Ϣ

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<APlayerController> PlayerController;		// ��ҿ�����

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<APlayerState> PlayerState;		// ���״̬

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;		// ����ϵͳ���

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<UAttributeSet> AttributeSet;		// ���Լ�

	// ��Ŀ���͵���ҿ����������״̬������ϵͳ��������Լ�
	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<AAuraPlayerController> AuraPlayerController;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<AAuraPlayerState> AuraPlayerState;		// ���״̬

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UAuraAttributeSet> AuraAttributeSet;		// ���Լ�

	AAuraPlayerController* GetAuraPC();		// ��ȡAura��ҿ�����
	AAuraPlayerState* GetAuraPS();		// ��ȡAura���״̬
	UAuraAbilitySystemComponent* GetAuraASC();		// ��ȡAura����ϵͳ���
	UAuraAttributeSet* GetAuraAS();		// ��ȡAura���Լ�


};
