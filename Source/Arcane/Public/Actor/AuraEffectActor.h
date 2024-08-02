// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "AuraEffectActor.generated.h"

// 定义效果应用策略
UENUM(BlueprintType)
enum class EEffectApplicationPolicy
{
	ApplyOnOverlap,		// 在重叠时应用效果
	ApplyOnEndOverlap,	// 在结束重叠时应用效果
	DoNotApply			// 不应用效果
};

// 定义效果移除策略，实际上，效果移除一般只针对永久效果，因为瞬时效果和持续效果会在自身结束时自动移除
UENUM(BlueprintType)
enum class EEffectRemovePolicy
{
	RemoveOnEndOverlap,	// 在结束重叠时移除效果
	DoNotRemove			// 不移除效果
};

UCLASS()
class ARCANE_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AAuraEffectActor();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass);	// 将效果应用到目标，传入目标和GameplayEffect类

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Aura Effect")
	bool bDestroyOnEffectApplication = false;	// 是否在效果应用后销毁

	/*
	 * 游戏中的效果分为三种：瞬时效果、持续效果和永久效果
	 */

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	TSubclassOf<UGameplayEffect> InstantGameplayEffectClass;	// 瞬时类型的GameplayEffect类

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	TSubclassOf<UGameplayEffect> DurationGameplayEffectClass;	// 持续类型的GameplayEffect类

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	TSubclassOf<UGameplayEffect> InfiniteGameplayEffectClass;	// 无限类型的GameplayEffect类

	/*
	 * 3种游戏效果的应用策略
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	EEffectApplicationPolicy InstantEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;	// 瞬时效果应用策略

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	EEffectApplicationPolicy DurationEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;	// 持续效果应用策略

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	EEffectApplicationPolicy InfiniteEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;	// (永久类型)无限效果应用策略

	/*
	 * 游戏效果的移除策略 （只针对永久效果，其他效果如果想通过策略控制移除，也可自行添加）
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	EEffectRemovePolicy InfiniteEffectRemovePolicy = EEffectRemovePolicy::RemoveOnEndOverlap;		// (永久类型)无限效果移除策略

	/*
	 * 游戏效果和能力系统组件的映射Map，该Map后续用于移除效果
	 */
	TMap<FActiveGameplayEffectHandle, UAbilitySystemComponent*> ActiveGameplayEffectsMap;	// 活动效果句柄 用于存储游戏效果和能力系统组件的映射

	/*
	 * 策略函数
	 */
	UFUNCTION(BlueprintCallable)
	void OnOverlap(AActor* TargetActor);	// 重叠时的策略函数

	UFUNCTION(BlueprintCallable)
	void OnEndOverlap(AActor* TargetActor);	// 结束重叠时的策略函数

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Aura Effect")
	float ActorLevel = 1.f;	// Actor等级

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Aura Effect")
	bool bApplyEffectToEnemies = false;	// 是否将效果应用到敌人

private:
};
