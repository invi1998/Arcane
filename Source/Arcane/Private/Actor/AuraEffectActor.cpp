// Copyright INVI1998


#include "Actor/AuraEffectActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/AuraAttributeSet.h"

// Sets default values
AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent")));	// 创建根组件, 场景组件。

}

void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
}

void AAuraEffectActor::ApplyEffectToTarget(AActor* Target, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	//IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(Target);	// 将目标转换为IAbilitySystemInterface接口

	//// 如果目标实现了IAbilitySystemInterface接口，表示目标是一个拥有能力系统的角色

	//if (AbilitySystemInterface)
	//{
	//	UAbilitySystemComponent* TargetAbilitySystemComponent = AbilitySystemInterface->GetAbilitySystemComponent();	// 获取目标的能力系统组件

	//	if (TargetAbilitySystemComponent)
	//	{
	//		FGameplayEffectContextHandle EffectContext = TargetAbilitySystemComponent->MakeEffectContext();	// 创建效果上下文
	//		EffectContext.AddSourceObject(this);	// 添加源对象

	//		FGameplayEffectSpecHandle NewHandle = TargetAbilitySystemComponent->MakeOutgoingSpec(GameplayEffectClass, 1.f, EffectContext);	// 创建新的效果规格
	//		if (NewHandle.IsValid())
	//		{
	//			TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*NewHandle.Data.Get());	// 将效果规格应用到自己
	//		}
	//	}
	//}

	// 当然，不是所有的目标都会实现IAbilitySystemInterface接口，所以我们可以直接获取目标的能力系统组件
	// 我们可以调用AbilitySystemBlueprintLibrary::GetAbilitySystemComponent函数来获取目标的能力系统组件
	// ，这个函数会自动判断目标是否实现了IAbilitySystemInterface接口，如果实现了，就会返回目标的能力系统组件，
	// 如果没有实现，他会查找目标的所有者是否实现了IAbilitySystemInterface接口，如果实现了，就会返回所有者的能力系统组件

	// 获取目标的能力系统组件
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
	if (TargetASC)
	{
		checkf(GameplayEffectClass, TEXT("GameplayEffectClass is nullptr!"));	// 检查GameplayEffectClass是否为空

		// 创建效果上下文，用于创建效果规格。
		// 什么是效果上下文呢？效果上下文是一个结构体，用于存储效果的来源，目标，施法者等信息。在创建效果规格时，我们需要传入效果上下文。
		FGameplayEffectContextHandle EffectContext = TargetASC->MakeEffectContext();
		EffectContext.AddSourceObject(this);	// 添加源对象，表示这个效果是由谁发出的。

		// 创建效果规格，用于应用效果。参数分别是效果类，等级，效果上下文。
		FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, 1.f, EffectContext);
		// 如果效果规格有效，就应用效果规格到自己。
		if (EffectSpecHandle.IsValid())
		{
			// 应用效果规格到自己。
			TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
		}
	}

}




