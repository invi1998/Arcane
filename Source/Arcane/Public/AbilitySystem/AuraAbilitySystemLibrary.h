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
	// 因为静态函数不能访问实例化对象，静态函数所属的类本身就不会被创建在World中，所以需要传入一个WorldContextObject，用于获取当前的World
	// 所以这也就是为什么很多函数库在引擎中都需要一个WorldContextObject的原因
	// 例如： 通过WorldContextObject获取当前的World，然后通过World获取GameInstance，再通过GameInstance获取自定义的GameInstance等等等

	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|WidgetController") // BlueprintPure表示这是一个纯蓝图函数，不需要任何引脚，他只是执行某种操作，并返回结果
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|WidgetController")
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);

	// 根据角色职业和角色等级初始化角色属性
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|CharacterClassDefault")
	static void InitCharacterAttributesByClassAndLevel(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC);

	// 初始化角色能力组
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|CharacterClassDefault")
	static void GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass);

	// 获取CharacterClassInfo
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|CharacterClassDefault")
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject);

	// 从AuraGampelayEffectContext中获取格挡信息
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static bool IsBlockedHit(const FGameplayEffectContextHandle& ContextHandle);

	// 从AuraGampelayEffectContext中获取暴击信息
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& ContextHandle);

	// 设置暴击信息 UPARAM(ref)表示传入的是引用，而不是值，而且这个参数在蓝图中是作为输入引脚的
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& ContextHandle, bool bCriticalHit);

	// 设置格挡信息 UPARAM(ref)表示传入的是引用，而不是值，而且这个参数在蓝图中是作为输入引脚的
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetBlockedHit(UPARAM(ref) FGameplayEffectContextHandle& ContextHandle, bool bBlockedHit);

};
