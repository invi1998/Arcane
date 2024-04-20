// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Data/CharacterClassInfo.h"

#include "AuraAbilitySystemLibrary.generated.h"

class UAbilitySystemComponent;

/**
 * 
 */
UCLASS()
class ARCANE_API UAuraAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// ��Ϊ��̬�������ܷ���ʵ�������󣬾�̬�����������౾��Ͳ��ᱻ������World�У�������Ҫ����һ��WorldContextObject�����ڻ�ȡ��ǰ��World
	// ������Ҳ����Ϊʲô�ܶຯ�����������ж���Ҫһ��WorldContextObject��ԭ��
	// ���磺 ͨ��WorldContextObject��ȡ��ǰ��World��Ȼ��ͨ��World��ȡGameInstance����ͨ��GameInstance��ȡ�Զ����GameInstance�ȵȵ�

	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|WidgetController") // BlueprintPure��ʾ����һ������ͼ����������Ҫ�κ����ţ���ֻ��ִ��ĳ�ֲ����������ؽ��
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|WidgetController")
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);

	// ���ݽ�ɫְҵ�ͽ�ɫ�ȼ���ʼ����ɫ����
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|CharacterClassDefault")
	static void InitCharacterAttributesByClassAndLevel(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC);

	// ��ʼ����ɫ������
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|CharacterClassDefault")
	static void GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass);

	// ��ȡCharacterClassInfo
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|CharacterClassDefault")
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject);

	// ��AuraGampelayEffectContext�л�ȡ����Ϣ
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static bool IsBlockedHit(const FGameplayEffectContextHandle& ContextHandle);

	// ��AuraGampelayEffectContext�л�ȡ������Ϣ
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& ContextHandle);

	// ���ñ�����Ϣ UPARAM(ref)��ʾ����������ã�������ֵ�����������������ͼ������Ϊ�������ŵ�
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& ContextHandle, bool bCriticalHit);

	// ���ø���Ϣ UPARAM(ref)��ʾ����������ã�������ֵ�����������������ͼ������Ϊ�������ŵ�
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetBlockedHit(UPARAM(ref) FGameplayEffectContextHandle& ContextHandle, bool bBlockedHit);

};
