// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Data/CharacterClassInfo.h"

#include "AuraAbilitySystemLibrary.generated.h"

class UAbilitySystemComponent;
struct FWidgetControllerParams;
struct FDamageEffectParams;
struct FGameplayEffectContextHandle;

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

	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|AbilitySystemComponent", meta=(DefaultToSelf="WorldContextObject"))
	static bool MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutParams, AAuraHUD*& OutAuraHUD);

	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject")) // BlueprintPure表示这是一个纯蓝图函数，不需要任何引脚，他只是执行某种操作，并返回结果
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
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

	// 获取AbilityInfo
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|AbilityInfo")
	static UAbilityInfo* GetAbilityInfo(const UObject* WorldContextObject);

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

	/*
	 * Debuff Start
	 */
	// 从AuraGampelayEffectContext中获取是否成功施加Debuff
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static bool IsSuccessfulDebuff(const FGameplayEffectContextHandle& ContextHandle);

	// 从AuraGampelayEffectContext中获取Debuff伤害
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static float GetDebuffDamage(const FGameplayEffectContextHandle& ContextHandle);

	// 从AuraGampelayEffectContext中获取Debuff频率
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static float GetDebuffFrequency(const FGameplayEffectContextHandle& ContextHandle);

	// 从AuraGampelayEffectContext中获取Debuff持续时间
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static float GetDebuffDuration(const FGameplayEffectContextHandle& ContextHandle);

	// 从AuraGampelayEffectContext中获取伤害类型
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static FGameplayTag GetDamageType(const FGameplayEffectContextHandle& ContextHandle);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static FVector GetDeathImpulse(const FGameplayEffectContextHandle& ContextHandle);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static FVector GetKnockbackImpulse(const FGameplayEffectContextHandle& ContextHandle);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetSuccessfulDebuff(UPARAM(ref) FGameplayEffectContextHandle& ContextHandle, bool bSuccessfulDebuff);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetDebuffDamage(UPARAM(ref) FGameplayEffectContextHandle& ContextHandle, float DebuffDamage);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetDebuffFrequency(UPARAM(ref) FGameplayEffectContextHandle& ContextHandle, float DebuffFrequency);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetDebuffDuration(UPARAM(ref) FGameplayEffectContextHandle& ContextHandle, float DebuffDuration);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetDamageType(UPARAM(ref) FGameplayEffectContextHandle& ContextHandle, FGameplayTag DamageType);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetDeathImpulse(UPARAM(ref) FGameplayEffectContextHandle& ContextHandle, const FVector& DeathImpulse);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetKnockbackImpulse(UPARAM(ref) FGameplayEffectContextHandle& ContextHandle, const FVector& KnockbackImpulse);

	/*
	 * Debuff End
	 */

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

	// 传入任意Widget，通过Slate获取该Widget在屏幕上的位置和大小
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayMechanice")
	static void GetWidgetPositionAndSize(const UObject* WorldContextObject, class UUserWidget* MyWidgetInstance, FVector2D& OutPosition, FVector2D& OutSize);

	// 传入AbilityTag，获取技能描述
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|AbilityInfo")
	static void GetAbilityDescriptionByTag(const UObject* WorldContextObject, const FGameplayTag& AbilityTag, FString& OutDescription, FString& OutNextLevelDescription);

	// 传入AbilityTag，获取技能等级
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|AbilityInfo")
	static int32 GetAbilityLevelByTag(const UObject* WorldContextObject, const FGameplayTag& AbilityTag);

	// 传入FDamageEffectParams, 获取伤害效果
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static FGameplayEffectContextHandle ApplyDamageEffect(const FDamageEffectParams& Params);

	// 传入前向向量，以及旋转轴，以及展开角度，以及角度数量，获取一堆向量
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayMechanice")
	static TArray<FVector> EvenlySpacedVectors(const FVector& Forward, const FVector& Axis, float SpreadAngle, int32 VectorNum);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayMechanice")
	static TArray<FRotator> EvenlySpacedRotators(const FVector& Forward, const FVector& Axis, float SpreadAngle, int32 RotatorNum);

	// 获取最近的目标
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayMechanice")
	static void GetClosestTargets(int32 MaxTargets, const TArray<AActor*>& TargetActors, const FVector& Origin, TArray<AActor*>& OutClosestTargets);

};
