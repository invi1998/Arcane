// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "WaitCooldownChange.generated.h"

struct FGameplayEffectSpec;
class UAbilitySystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCooldownChangeSignature, float, CooldownTimeRemaining);

/**
 * ������ȴʱ���첽����
 */
UCLASS(BlueprintType, meta=(ExposedAsyncProxy="AsyncTask"))	// ��ͼ���ͣ���¶�첽����
class ARCANE_API UWaitCooldownChange : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)	// ��ͼ���¼�
	FOnCooldownChangeSignature CooldownStart;	// ��ȴ��ʼ

	UPROPERTY(BlueprintAssignable)	// ��ͼ���¼�
	FOnCooldownChangeSignature CooldownEnd;	// ��ȴ����

	// �ú�����������ͼ���õģ�������Ϊ����һ���첽�����������������һ���ڲ�����������AbilityTaskһ����������ͼ�д���һ���첽����ڵ�
	// �ú���ִ�иýڵ��ʱ�򽫻ᴴ��һ��ʵ�������ԣ�Ϊ��˵�����������һ���ڲ�������������Ҫʹ��meta��ǩ
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))	// ��ͼ����
	static UWaitCooldownChange* WaitCooldownChange(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& InCooldownTag);	// ��������

	UFUNCTION(BlueprintCallable)
	void EndTask();	// ��������ʱ���ã������¼���


protected:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> ASC;

	UPROPERTY()
	FGameplayTag CooldownTag;

	void CooldownChanged(const FGameplayTag InCooldownTag, int32 NewCount);	// ��ȴʱ��ı�ʱ����

	void OnActiveEffectAdded(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveHandle);	// �����ȴЧ��ʱ����

};

