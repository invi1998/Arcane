# Cooldown

技能冷却，设置冷却部分和前面设置蓝耗都差不多，GA里添加这个就行

![image-20240428212506510](.\image-20240428212506510.png)

现在，比较复杂的是，我们想在控件里显示技能冷却。

为了实现这个功能，首先我们需要创建一个异步任务，该异步任务会在蓝图中创建一个异步任务节点（联想一下WaitPlayMontage，播放蒙太奇动画就有很多异步任务节点，比如播放完成啊，混入混出啊等等），对于技能冷却，我们需要的节点就两个，一个冷却开始，一个冷却结束。

同时，因为我们希望该任务是一个蓝图节点，所以，我们需要一个静态函数，来创建该任务对象，它接收两个参数，一个ASC，用来和我们Ability进行通信（在技能进入冷区的时候，需要通过ASC去订阅相关的代理），还有一个就是当前冷却的Tag。所以这个类可以这样设计

```c++
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
 * 技能冷却时间异步任务
 */
UCLASS(BlueprintType, meta=(ExposedAsyncProxy="AsyncTask"))	// 蓝图类型，暴露异步代理
class ARCANE_API UWaitCooldownChange : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)	// 蓝图绑定事件
	FOnCooldownChangeSignature CooldownStart;	// 冷却开始

	UPROPERTY(BlueprintAssignable)	// 蓝图绑定事件
	FOnCooldownChangeSignature CooldownEnd;	// 冷却结束

	// 该函数必须是蓝图调用的，而且因为这是一个异步任务，所以这个函数是一个内部函数，这与AbilityTask一样，会在蓝图中创建一个异步任务节点
	// 该函数执行该节点的时候将会创建一个实例，所以，为了说明这个函数是一个内部函数，我们需要使用meta标签
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))	// 蓝图调用
	static UWaitCooldownChange* WaitCooldownChange(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& InCooldownTag);	// 创建任务

	UFUNCTION(BlueprintCallable)
	void EndTask();	// 结束任务时调用，清理事件绑定


protected:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> ASC;

	UPROPERTY()
	FGameplayTag CooldownTag;

	void CooldownChanged(const FGameplayTag InCooldownTag, int32 NewCount);	// 冷却时间改变时调用

	void OnActiveEffectAdded(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveHandle);	// 添加冷却效果时调用

};

```

现在，我们需要知道，什么时候冷却开始？什么时候冷却结束？

这个很重要，我们可以通过`OnActiveGameplayEffectAddedDelegateToSelf`委托就知道冷却开始，

`OnActiveGameplayEffectAddedDelegateToSelf` 每当添加一个新的基于持续时间的GameplayEffect时，客户端和服务器都会调用此委托

> `OnActiveGameplayEffectAddedDelegateToSelf` 是一个委托（delegate），它直接绑定到`UAbilitySystemComponent`实例上，专门用于监听该组件自身上新增的`Active Gameplay Effect`（活跃的游戏效果）。当你想要在自己的角色或对象上新增Gameplay Effect时立即得到通知，可以使用这个委托。这个委托的使用较为直接，直接附加在想要监听的ASC实例上，无需通过Gameplay Tag进行过滤。
>
> 相比之下，`RegisterGameplayTagEvent` 提供了一种更加灵活的方式来监听与特定Gameplay Tags相关的事件。使用这个方法，你可以为ASC注册监听器，以便在任何带有指定Gameplay Tag的Gameplay Effect被添加、移除或更新时接收通知。这意味着你可以基于不同的Gameplay Tags来分别处理不同的逻辑，而不仅仅局限于ASC自身的Gameplay Effects。
>
> 总结来说：
> - `OnActiveGameplayEffectAddedDelegateToSelf` 更加专注于ASC实例本身新增的Gameplay Effect，直接绑定，不涉及Gameplay Tag筛选。
> - `RegisterGameplayTagEvent` 提供了一个基于Gameplay Tag的事件监听机制，适用于更广泛的情景，可以针对特定标签的Gameplay Effects在整个ASC的管理范围内外进行监听，不仅限于自身。
>
> 根据你的具体需求，如果需要针对特定状态变化进行精细控制和响应，使用`RegisterGameplayTagEvent`可能更为合适；如果只关心作用于ASC自身的Gameplay Effect添加，使用`OnActiveGameplayEffectAddedDelegateToSelf`则更为直接简单。

我们可以通过订阅ASC的RegiterGameplayTagEvent来监听我们指定的CooldownTag的变化知道冷却结束，我们监听什么事件呢？

`EGameplayTagEventType::NewOrRemoved`，Tag创建或移除的事件，很简单，CooldownTag创建或者移除，都表示我们可以进行冷区任务的相关收尾工作，我们就可以去处理相关表现了，在这里就是去让开始冷却的代理进行相关广播了，移除代理，释放内存。

> 在Unreal Engine 5 (UE5) 中，Attribute System Component (ASC) 是管理角色属性和Gameplay Effects的核心组件。Gameplay Tags是Unreal Engine中一种灵活的标签系统，用于标记和分类游戏中的各种元素，比如技能、状态效果等。通过注册Gameplay Tag事件，开发者可以监听特定标签相关的事件，从而在这些事件发生时执行特定的逻辑。
>
> `RegisterGameplayTagEvent` 函数允许你订阅与Gameplay Tags相关的事件。这意味着你可以设置一个回调函数，当带有指定Gameplay Tag的Gameplay Effect被应用、移除或者有其他相关变化时，这个回调函数会被触发。这对于实现状态改变的响应逻辑非常有用，比如当角色获得“燃烧”状态时播放火焰特效，或者当角色进入“隐身”状态时调整视觉表现。
>
> 基本使用方法如下：
>
> ```cpp
> // 假设ASC实例已经存在并可以访问，例如通过UAbilitySystemComponent指针
> UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
> 
> // 定义一个要监听的Gameplay Tag
> FGameplayTag MyTag = FGameplayTag::RequestGameplayTag("Status.Burning");
> 
> // 注册事件处理函数
> FOnActiveGameplayEffectRemovedDelegate OnEffectRemovedDelegate;
> OnEffectRemovedDelegate.BindUObject(this, &YourClass::OnBurningEffectRemoved);
> 
> // 订阅Gameplay Effect移除事件
> FActiveGameplayEffectHandle Handle = ASC->RegisterGameplayTagEvent(MyTag, EGameplayTagEventType::Removed).AddUObject(this, &YourClass::OnBurningEffectRemoved);
> 
> // 注意：需要妥善管理这些事件的订阅，避免内存泄漏。在不再需要时应取消订阅。
> Handle.RemoveTagFromRegistration();
> 
> // 示例回调函数
> void YourClass::OnBurningEffectRemoved(UAbilitySystemComponent* TargetASC, const FGameplayEffectRemovalInfo& RemovalInfo)
> {
>     // 在这里执行当“燃烧”效果被移除时的逻辑
> }
> ```
>
> 这里展示了如何注册一个事件来监听当带有 `Status.Burning` 标签的Gameplay Effect从ASC中移除时的回调。你可以根据需求注册不同的事件类型，如 `EGameplayTagEventType::NewApplication`（新效果应用时）、`EGameplayTagEventType::AnyChange`（任何变化时）等。记得在适当的时候取消订阅以避免内存泄漏。

```c++
// Copyright INVI1998


#include "AbilitySystem/AsyncTasks/WaitCooldownChange.h"

#include "AbilitySystemComponent.h"

UWaitCooldownChange* UWaitCooldownChange::WaitCooldownChange(UAbilitySystemComponent* AbilitySystemComponent,
                                                             const FGameplayTag& InCooldownTag)
{
	UWaitCooldownChange* WaitForCooldownChange = NewObject<UWaitCooldownChange>();
	WaitForCooldownChange->ASC = AbilitySystemComponent;
	WaitForCooldownChange->CooldownTag = InCooldownTag;

	if (!IsValid(AbilitySystemComponent) || !InCooldownTag.IsValid())
	{
		WaitForCooldownChange->EndTask();
		return nullptr;
	}

	// 冷却何时结束（CooldownTag何时被移除）
	// 我们可以通过订阅RegiterGameplayTagEvent来监听CooldownTag的变化
	// 注意：以为这是一个静态函数，没有this指针，所以我们需要使用上面创建的WaitForCooldownChange对象来AddUObject
	AbilitySystemComponent->RegisterGameplayTagEvent(InCooldownTag, EGameplayTagEventType::NewOrRemoved).AddUObject(WaitForCooldownChange, &UWaitCooldownChange::CooldownChanged);

	// 冷却何时开始，（当添加一个新的基于持续时间的GameplayEffect时）
	// 每当添加一个新的基于持续时间的GameplayEffect时，客户端和服务器都会调用此委托
	AbilitySystemComponent->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(WaitForCooldownChange, &UWaitCooldownChange::OnActiveEffectAdded);

	return WaitForCooldownChange;
}

void UWaitCooldownChange::EndTask()
{
	// 移除监听
	if (IsValid(ASC))
	{
		ASC->RegisterGameplayTagEvent(CooldownTag, EGameplayTagEventType::NewOrRemoved).RemoveAll(this);
	}

	SetReadyToDestroy();	// 设置为准备销毁，动作完全结束后调用，这样就能自由删除该操作，并且取消在游戏实例中的注册
	MarkAsGarbage();		// 标记为垃圾，等待GC回收
}

void UWaitCooldownChange::CooldownChanged(const FGameplayTag InCooldownTag, int32 NewCount)
{
	if (NewCount == 0)	// 如果冷却时间为0，结束任务
	{
		CooldownEnd.Broadcast(0);
	}
}

void UWaitCooldownChange::OnActiveEffectAdded(UAbilitySystemComponent* AbilitySystemComponent,
	const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveHandle)
{
	FGameplayTagContainer AssetTags;
	EffectSpec.GetAllAssetTags(AssetTags);

	FGameplayTagContainer GrantedTags;
	EffectSpec.GetAllGrantedTags(GrantedTags);

	if (AssetTags.HasTagExact(CooldownTag) || GrantedTags.HasTagExact(CooldownTag))
	{
		FGameplayEffectQuery Query = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CooldownTag.GetSingleTagContainer());	// 查询是否有CooldownTag

		// 为什么GetActiveEffectsTimeRemaining返回的是一个float数组？
		// 因为该函数返回所有匹配的GameplayEffect的剩余时间，而不是只返回一个
		TArray<float> TimeRemainingArray = AbilitySystemComponent->GetActiveEffectsTimeRemaining(Query);	// 获取剩余时间
		// 冷却开始
		if (TimeRemainingArray.Num() > 0)
		{
			// 返回冷却数组里的最大值作为我们的冷却时间
			float CooldownTime = TimeRemainingArray[0];
			for (int i = 1; i < TimeRemainingArray.Num(); i++)
			{
				CooldownTime = FMath::Max(CooldownTime, TimeRemainingArray[i]);
			}
			CooldownEnd.Broadcast(CooldownTime);
		}
	}
}

```

同时，该任务类应该被写为蓝图类型，同时，暴露异步代理，因为我们希望在蓝图中能得到该类对象，我们希望在执行节点之前，先在蓝图中可以检查一下对象是否存在，任务是否存在，如果存在，可以进行结束，然后开启新的异步任务等相关操作，所以在类声明这里，这个UE宏就很有必要

```c++
UCLASS(BlueprintType, meta=(ExposedAsyncProxy="AsyncTask"))	// 蓝图类型，暴露异步代理
class ARCANE_API UWaitCooldownChange : public UBlueprintAsyncActionBase
{
```

现在，我们进入蓝图查看节点如何使用。如图，我们在开始任务之前，先检查这个任务对象是否存在，如果存在，先调用它的成员函数EndTask，我们在上面可以看到EndTask就是做些清除代理和内存释放功能，这样能避免内存泄漏

![image-20240428215123952](.\image-20240428215123952.png)