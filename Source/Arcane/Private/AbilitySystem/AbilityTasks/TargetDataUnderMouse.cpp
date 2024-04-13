// Copyright INVI1998


#include "AbilitySystem/AbilityTasks/TargetDataUnderMouse.h"

#include "AbilitySystemComponent.h"
#include "Player/AuraPlayerController.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	// 创建一个UTargetDataUnderMouse*类型的指针
	UTargetDataUnderMouse* MyObj = NewObject<UTargetDataUnderMouse>(OwningAbility);
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
