// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Data/CharacterClassInfo.h"

#include "AuraAbilitySystemLibrary.generated.h"

class UAbilitySystemComponent;
struct FWidgetControllerParams;

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

	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|AbilitySystemComponent")
	static bool MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutParams, AAuraHUD*& OutAuraHUD);

	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|WidgetController") // BlueprintPure表示这是一个纯蓝图函数，不需要任何引脚，他只是执行某种操作，并返回结果
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|WidgetController")
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|WidgetController")
	static USpellMenuWidgetController* GetSpellMenuWidgetController(const UObject* WorldContextObject);

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

	// 获取半径内的所有存活玩家 （传入上下文对象，传出玩家数组，传入忽略的玩家数组，传入中心点，传入半径）
	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|GameplayMechanice")
	static void GetLivePlayerWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutPlayers, const TArray<AActor*>& IgnoreActors, const FVector& Origin, float Radius);

	// 获取射线检测内的所有存活玩家 （传入上下文对象，传出玩家数组，传入忽略的玩家数组，传入射线起点，射线终点）
	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|GameplayMechanice")
	static void GetLivePlayerWithinRaycast(const UObject* WorldContextObject, TArray<AActor*>& OutPlayers, const TArray<AActor*>& IgnoreActors, const FVector& Start, const FVector& End);

	// 获取立方体重叠检测内的所有存活玩家 （传入上下文对象，传出玩家数组，传入忽略的玩家数组，传入立方体中心，传入立方体尺寸）
	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|GameplayMechanice")
	static void GetLivePlayerWithinBoxOverlap(const UObject* WorldContextObject, TArray<AActor*>& OutPlayers, const TArray<AActor*>& IgnoreActors, const FVector& Origin, const FVector& BoxExtent);

	// 获取前方扇形内的所有存活玩家 （传入上下文对象，传出玩家数组，传入忽略的玩家数组，传入中心点，传入前方向，传入扇形角度，传入半径）
	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|GameplayMechanice")
	static void GetLivePlayerWithinForwardSector(const UObject* WorldContextObject, TArray<AActor*>& OutPlayers, const TArray<AActor*>& IgnoreActors, const FVector& Origin, const FVector& Forward, float Angle, float Radius);

	// 是否是友军（传入上下文对象，传入自己，传入目标）
	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|GameplayMechanice")
	static bool IsFriendly(AActor* FirstActor, AActor* SecondActor);

	// 获取怪物的经验奖励（传入上下文对象，传入怪物等级， 怪物职业）
	static int32 GetMonsterEXPRewardByClassAndLv(const UObject* WorldContextObject, ECharacterClass CharacterClass, int32 CharacterLv);

};
