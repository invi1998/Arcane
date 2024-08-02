// Copyright INVI1998


#include "Actor/AuraEffectActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	// 设置Tick函数的时间间隔

	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent")));	// 创建根组件, 场景组件。

}

void AAuraEffectActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RunningTime += DeltaTime;	// 累加运行时间
	// RunningTime可能会溢出，所以我们需要对其进行取模运算，使其始终保持在0到2PI之间。
	RunningTime = FMath::Fmod(RunningTime, Frequency * TWO_PI);	// 取模运算

	ItemMovement(DeltaTime);	// 物品移动
}

void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
	InitialLocation = GetActorLocation();	// 获取初始位置
	CalculateLocation = InitialLocation;	// 设置计算位置
	CalculateRotation = GetActorRotation();	// 设置计算旋转

}

void AAuraEffectActor::ItemMovement(float DeltaTime)
{
	if (bRotate)	// 如果允许旋转
	{
		const FRotator NewRotation = FRotator(0.f, RotationRate * DeltaTime, 0.f);	// 计算新的旋转
		CalculateRotation = UKismetMathLibrary::ComposeRotators(CalculateRotation, NewRotation);	// 合成旋转
	}

	if (bFloating)	// 如果允许浮动
	{
		const FVector NewLocation = FVector(0.f, 0.f, Amplitude * FMath::Sin(Frequency * RunningTime));	// 计算新的位置
		CalculateLocation = InitialLocation + NewLocation;	// 设置新的位置
	}
}

void AAuraEffectActor::StartSinusoidalFloat()
{
	bFloating = true;	// 开启浮动
	InitialLocation = GetActorLocation();	// 获取初始位置
	CalculateLocation = InitialLocation;	// 设置计算位置
}

void AAuraEffectActor::StartRotating()
{
	bRotate = true;	// 开启旋转
	CalculateRotation = GetActorRotation();	// 设置计算旋转
}

void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	// 当然，不是所有的目标都会实现IAbilitySystemInterface接口，所以我们可以直接获取目标的能力系统组件
	// 我们可以调用AbilitySystemBlueprintLibrary::GetAbilitySystemComponent函数来获取目标的能力系统组件
	// ，这个函数会自动判断目标是否实现了IAbilitySystemInterface接口，如果实现了，就会返回目标的能力系统组件，
	// 如果没有实现，他会查找目标的所有者是否实现了IAbilitySystemInterface接口，如果实现了，就会返回所有者的能力系统组件

	if (TargetActor->ActorHasTag(FName("Enemy")) && !bApplyEffectToEnemies) return;	// 如果目标是敌人，但是我们不希望应用效果到敌人，就直接返回

	// 获取目标的能力系统组件
	if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor))
	{
		checkf(GameplayEffectClass, TEXT("GameplayEffectClass is nullptr!"));	// 检查GameplayEffectClass是否为空

		// 创建效果上下文，用于创建效果规格。
		// 什么是效果上下文呢？效果上下文是一个结构体，用于存储效果的来源，目标，施法者等信息。在创建效果规格时，我们需要传入效果上下文。
		FGameplayEffectContextHandle EffectContext = TargetASC->MakeEffectContext();
		EffectContext.AddSourceObject(this);	// 添加源对象，表示这个效果是由谁发出的。

		// 创建效果规格，用于应用效果。参数分别是效果类，等级，效果上下文。
		const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, ActorLevel, EffectContext);

		// 获取效果的持续时间类型，判断效果是否是无限持续的。
		const bool bIsInfinite = EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;

		// 如果效果规格有效，就应用效果规格到自己。
		if (EffectSpecHandle.IsValid())
		{
			// 应用效果规格到自己。
			const FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

			
			// 只有效果是无限持续的，并且我们希望在结束重叠时移除效果时，我们才将效果句柄和能力系统组件存储到映射表中。
			if (bIsInfinite && InfiniteEffectRemovePolicy == EEffectRemovePolicy::RemoveOnEndOverlap)
			{
				// 如果效果是无限持续的，就将效果句柄和能力系统组件存储到映射表中。
				ActiveGameplayEffectsMap.Add(ActiveEffectHandle, TargetASC);
			}
		}

		if (!bIsInfinite)
		{
			// 如果是在效果应用后销毁，并且效果是瞬时的，就销毁自己。
			Destroy();
		}

	}

}

void AAuraEffectActor::OnOverlap(AActor* TargetActor)
{
	if (TargetActor->ActorHasTag(FName("Enemy")) && !bApplyEffectToEnemies) return;	// 如果目标是敌人，但是我们不希望应用效果到敌人，就直接返回

	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}
}

void AAuraEffectActor::OnEndOverlap(AActor* TargetActor)
{
	if (TargetActor->ActorHasTag(FName("Enemy")) && !bApplyEffectToEnemies) return;	// 如果目标是敌人，但是我们不希望应用效果到敌人，就直接返回

	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}
	if (InfiniteEffectRemovePolicy == EEffectRemovePolicy::RemoveOnEndOverlap)
	{
		// 移除效果
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);	// 获取目标的能力系统组件
		if (IsValid(TargetASC))
		{
			TArray<FActiveGameplayEffectHandle> ActiveEffectsToRemove;	// 用于存储要移除的效果句柄
			for (const TTuple<FActiveGameplayEffectHandle, UAbilitySystemComponent*>& Elem : ActiveGameplayEffectsMap)
			{
				if (Elem.Value == TargetASC)	// 如果映射表中的能力系统组件和目标的能力系统组件相同，表示这个效果是目标的
				{
					TargetASC->RemoveActiveGameplayEffect(Elem.Key, 1);	// 移除效果
					ActiveEffectsToRemove.Add(Elem.Key);	// 将要移除的效果句柄存储到数组中
				}
			}
			for (const FActiveGameplayEffectHandle& ActiveEffectHandle : ActiveEffectsToRemove)
			{
				ActiveGameplayEffectsMap.Remove(ActiveEffectHandle);	// 从映射表中移除效果句柄
			}
		}
	}
}





