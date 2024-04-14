// Copyright INVI1998


#include "AbilitySystem/AbilityTasks/TargetDataUnderMouse.h"

#include "AbilitySystemComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/AuraPlayerController.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	// 创建一个UTargetDataUnderMouse*类型的指针
	UTargetDataUnderMouse* MyObj = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);

	// 返回这个指针
	return MyObj;
}

void UTargetDataUnderMouse::Activate()
{
	const bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();
	if (bIsLocallyControlled)
	{
		SendMouseTargetData();
	}
	else
	{
		// TODO: 在服务端，监听客户端的MouseTargetData

		const FGameplayAbilitySpecHandle AbilitySpecHandle = GetAbilitySpecHandle();	// 获取AbilitySpecHandle, 用于标识Ability
		const FPredictionKey ActivationPredictionKey = GetActivationPredictionKey();	// 获取ActivationPredictionKey, 用于标识Ability的激活预测

		// 这里我们可以使用AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate()来监听客户端的MouseTargetData
		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(AbilitySpecHandle, ActivationPredictionKey).AddUObject(this, &UTargetDataUnderMouse::OnTargetDataReplicatedCallback);	// 添加一个委托，当客户端广播MouseTargetData时，就会调用OnTargetDataReplicatedCallback函数

		// 一旦服务器上调用了激活，服务器就可以将其回调绑定到这个委托上，这样当客户端广播MouseTargetData时，就会调用OnTargetDataReplicatedCallback函数

		// 但是如果我们已经来不及，目标数据已经被广播了，那么这种情况下，我们依然应该调用该回调
		// 所以，有一种方法可以检查或者至少可以在已经接收到目标数据时调用该目标的数据委托，所以我们可以通过CallReplicatedTargetDataDelegatesIfSet来检查是否已经接收到目标数据，如果我们没有调用这个委托，那就意味着它还没有到达服务器，所以我们需要继续等待
		const bool bCalledDelegate = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(AbilitySpecHandle, ActivationPredictionKey);		// 如果已经接收到目标数据，那么就调用该目标的数据委托
		if (!bCalledDelegate)
		{
			// 如果没有接收到目标数据，那么就继续等待
			SetWaitingOnRemotePlayerData();
		}
	}

}

void UTargetDataUnderMouse::SendMouseTargetData()
{
	// 创建一个FScopedPredictionWindow对象，用于管理预测窗口，这个对象会在作用域结束时自动销毁，他需要传递一个AbilitySystemComponent指针，以及一个是否使用预测的布尔值，这里我们使用预测，所以传递true（默认值）
	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get());

	// 当前作用域内的代码将使用预测，这意味着我们可以在客户端上执行这些代码，而不会等待服务器的响应，
	// 当服务端得知这个预测时，它会在服务端上执行相同的代码，然后比较结果，如果结果不同，那么服务端会纠正客户端的预测，这样就保证了客户端和服务端的一致性

	// 这个类继承自 FGameplayAbilityTargetData，它是用于描述 Ability 系统中目标选择和定位信息的基本结构
	FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();	// 创建一个新的FGameplayAbilityTargetData_SingleTargetHit对象

	FGameplayAbilityTargetDataHandle DataHandle;	// 创建一个FGameplayAbilityTargetDataHandle对象

	AAuraPlayerController* PC = Cast<AAuraPlayerController>(Ability->GetCurrentActorInfo()->PlayerController.Get());	// 获取玩家控制器
	Data->HitResult = PC->GetCursorHitResult();	// 获取玩家控制器的光标命中结果
	DataHandle.Add(Data);	// 将Data添加到DataHandle中

	FGameplayTag ApplicationTag;

	// 客户端调用ServerSetReplicatedTargetData，将DataHandle传递给服务端
	AbilitySystemComponent->ServerSetReplicatedTargetData(
		GetAbilitySpecHandle(),		// 获取AbilitySpecHandle, 用于标识Ability
		GetActivationPredictionKey(),	// 获取ActivationPredictionKey, 用于标识Ability的激活预测
		DataHandle,		// 传递DataHandle, 用于传递目标数据，这里是光标命中结果（所以上面的DataHandle.Add(Data);
		ApplicationTag,		// 传递ApplicationTag, 用于标识Ability的应用标签
		AbilitySystemComponent->ScopedPredictionKey	// 传递ScopedPredictionKey, 用于标识Ability的预测键，Scoped意味着这个预测键只在这个Ability中有效，它仅限于我们创建的这个ScopedPrediction对象的生命周期
	);

	// 广播能力系统的目标数据
	// 但是广播之前，需要先判断是否有合法的目标数据，比如如果能力已经不再激活，那么就不需要广播了
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		// 这里Broadcast需要传递一个FVector类型的参数，但是这里我想改为传递DataHandle，所以我在UTargetDataUnderMouse.h中改变了ValidData的类型为FGameplayAbilityTargetDataHandle
		// 这样我们就能继续通过广播传递整个DataHandle，可以获取命中结果和目标数据中包含的任何其他内容，然后在Ability中通过委托的回调函数中获取DataHandle
		ValidData.Broadcast(DataHandle);
	}
}

void UTargetDataUnderMouse::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle,
	FGameplayTag ActivationTag)
{
	// 一旦进入这个函数，就意味着我们已经接收到了 replicated 目标数据，而且我们知道，replicated 只会从服务端到客户端，
	// 但是在GAS中，你会发现，客户端也可以发送 replicated 目标数据，这里就是一个例子，我们在客户端发送了 replicated 目标数据，然后服务端接收到了这个数据
	// 所以，当接收到 replicated 目标数据时，此函数将在服务端调用

	// 所以，我们需要在这里广播目标数据，但是，我们同时还需要确保ASC（AbilitySystemComponent）已经知道这个数据被接收到了
	// 因为当服务器接收到Replicated目标数据时，他会将数据存储在ASC的AbilityTargetDataMap中，
	// 所以这个时候，我们就可以告诉ASC我们已经接收到了目标数据，调用ASC的ConsumeClientReplicatedTargetData函数，传递AbilitySpecHandle和ActivationPredictionKey
	// 让ASC不必再存储这个数据
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());	// 告诉ASC我们已经接收到了目标数据

	// 然后我们就可以广播目标数据了
	// 一样的，广播之前，还应该判断是否有合法的目标数据，比如如果能力已经不再激活，那么就不需要广播了
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}

}
