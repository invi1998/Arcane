# Ability Tasks



在 Unreal Engine 5 (UE5) 的游戏能力系统 (Gameplay Abilities System, GAS) 中，AbilityTasks 是一种用于实现能力系统功能的异步任务系统。AbilityTasks 是一种轻量级的任务系统，用于异步地执行与能力相关的工作，比如冷却时间的计时、能力的激活和取消等。

AbilityTasks 是通过 AbilityTaskBase 类来实现的。AbilityTaskBase 类是一个抽象基类，定义了 AbilityTask 的基本接口。AbilityTaskBase 类的定义如下：

```cpp
class UAbilityTaskBase : public UTaskGraphInterface
{
    GENERATED_BODY()

public:
    virtual void OnTaskFinished() = 0;
};
```

在 AbilityTaskBase 类中，定义了一个纯虚函数 OnTaskFinished()，用于在任务完成时调用。子类需要实现这个函数，以处理任务完成后的逻辑。

在 Unreal Engine 5 的游戏能力系统中，AbilityTasks 通常用于实现以下功能：

1. **冷却时间计时**：AbilityTasks 可以用来计时某个能力的冷却时间，当冷却时间结束时，可以自动激活该能力。

2. **能力激活和取消**：AbilityTasks 可以用来激活和取消某个能力，当能力激活或取消时，可以自动更新游戏状态。

3. **能力触发器**：AbilityTasks 可以用来触发某个能力的激活，当满足某个条件时，可以自动激活该能力。

4. **能力状态管理**：AbilityTasks 可以用来管理能力的状态，比如激活、冷却、可用等。

在游戏逻辑中，可以使用 AbilityTaskBase 类来创建和管理 AbilityTasks。以下是一个创建和使用 AbilityTask 的示例代码：

```cpp
// 创建一个 AbilityTask
UAbilityTask* MyAbilityTask = NewAbilityTask<MyAbilityTaskClass>();

// 启动 AbilityTask
MyAbilityTask->StartTask();

// 等待 AbilityTask 完成
MyAbilityTask->WaitForTaskCompletion();
```

在这个示例中，我们首先创建了一个 AbilityTask 类的实例 MyAbilityTask，然后启动该任务。接着，我们等待该任务完成。当任务完成后，我们可以调用 OnTaskFinished() 函数来处理任务完成后的逻辑。

# PlayMontageAndWait 

在 Unreal Engine 5 (UE5) 的游戏能力系统 (Gameplay Abilities System, GAS) 中，PlayMontageAndWait 是一个 AbilityTask 类，用于播放指定的蒙太奇动画并等待其完成。这个任务通常用于播放角色的动作动画，比如攻击、跳跃、移动等。

PlayMontageAndWait 类的定义如下：

```cpp
UAbilityTask* UAbilityTask_WaitForMontageToFinish::PlayMontageAndWait(UAnimMontage* Montage, float EndOffset)
```

PlayMontageAndWait 类的构造函数接受两个参数：一个是 UAnimMontage 类的指针，表示要播放的蒙太奇动画；另一个是 float 类型的 EndOffset，表示播放蒙太奇动画时的结束偏移量。

在 PlayMontageAndWait 类中，实现了一个 StartTask() 函数，用于启动任务。这个函数首先调用 UAbilityTaskBase 类的 StartTask() 函数，然后调用 UAnimInstance::Montage_Play() 函数来播放指定的蒙太奇动画。接着，它设置一个定时器，用于等待蒙太奇动画的完成。

在 PlayMontageAndWait 类中，还有一个 OnTaskFinished() 函数，用于处理任务完成后的逻辑。这个函数首先调用 UAbilityTaskBase 类的 OnTaskFinished() 函数，然后调用 UAnimInstance::Montage_Stop() 函数来停止播放蒙太奇动画。

在游戏逻辑中，可以使用 PlayMontageAndWait 类来播放蒙太奇动画并等待其完成。以下是一个使用 PlayMontageAndWait 类的示例代码：

```cpp
// 创建一个 PlayMontageAndWait 类的实例
UAbilityTask* MyPlayMontageAndWaitTask = NewAbilityTask<UPlayMontageAndWait>();

// 设置要播放的蒙太奇动画和结束偏移量
MyPlayMontageAndWaitTask->Montage = MyMontage;
MyPlayMontageAndWaitTask->EndOffset = 0.5f;

// 启动 PlayMontageAndWait 任务
MyPlayMontageAndWaitTask->StartTask();

// 等待 PlayMontageAndWait 任务完成
MyPlayMontageAndWaitTask->WaitForTaskCompletion();
```

在这个示例中，我们首先创建了一个 PlayMontageAndWait 类的实例 MyPlayMontageAndWaitTask，然后设置要播放的蒙太奇动画和结束偏移量。接着，我们启动 PlayMontageAndWait 任务，并等待其完成。当任务完成后，我们可以调用 OnTaskFinished() 函数来处理任务完成后的逻辑。



回到项目中，GA_FireBolt 这个游戏能力蓝图中，在我们的能力激活节点，我们希望播放一个施法动作，如下

![image-20240412155304262](.\image-20240412155304262.png)

# Custom Ability Tasks 自定义游戏任务

对于这样一个动画蒙太奇，我们希望在角色举起法杖的时候，再去执行释放火球技能。

![image-20240412155128937](.\image-20240412155128937.png)

所以，我们需要先去创建一个动画通知，因为这个动画通知，需要附带上我们的AbilityTag作为参数传递给其他节点调用，所以在这个动画通知里，我们新建一个变量，并暴露出去。同时为什么我们要单独创建一个动画通知，而不是直接在动画蒙太奇中右键点击创建一个Notify呢？

是因为我们要重写接收到Notify这个事件，在这个事件里，我们还要做些参数传递的动作，所以在这个动画通知里，我们先重写 通知接收 事件。如下：

![image-20240412155957083](.\image-20240412155957083.png)

注意，这里在中间节点调用的 Send Gameplay Event Actor 函数，它可以用来触发一些Ability，简单来说，比如我们希望在收到这个动画通知这里（举起法杖这里），进行火球术的释放，那么，这里我们就可以把火球术的施法事件作为Tag传递给角色，这个时候就能调用这个事件去执行火球释放逻辑。

然后继续，我们回到东湖蒙太奇，去创建通知，通知类型选择我们新建的这个animNotify，然后指定TAG

![image-20240412160454863](.\image-20240412160454863.png)

现在。有个问题是，因为我们的火球生成是在C++ 的ActivateAbility就创建的，这个正式我们蓝图中EventActivedAbility这个事件节点。所以，为了通过动画通知，我们将火球生成新建一个蓝图可调用函数里

```c++
void UAuraProjectileSpell::SpawnProjectile()
{
	
	// 投射物生成，我们希望他是在服务端生成，然后在客户端同步
	const bool bIsServer = GetOwningActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;

	// 生成位置，我不希望简单使用角色的位置，而是使用施法者武器上的插槽位置
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());

	// TODO: 设置投射物旋转，比如朝向目标

	if (CombatInterface)
	{
		const FVector SocketLocation = CombatInterface->GetCombatSocketLocation();

		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);		// 使用武器插槽位置

		// SpawnActorDeferred 异步生成Actor 是因为我们希望在生成之前设置一些属性，比如伤害，速度等
		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
			ProjectileClass,	// 投射物类
			SpawnTransform,		// 生成位置
			GetOwningActorFromActorInfo(),	// 拥有者
			Cast<APawn>(GetOwningActorFromActorInfo()),	// 控制者
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn	// 碰撞处理方式, 总是生成
		);

		// TODO: 设置投射物属性，比如伤害，速度等

		Projectile->FinishSpawning(SpawnTransform);	// 完成生成
	}

}

```

然后再蓝图的通知到达节点，调用它即可

![image-20240412162239871](.\image-20240412162239871.png)

![image-20240412162144785](.\image-20240412162144785.png)

![image-20240413012219326](.\image-20240413012219326.png)



# Target Data

![image-20240413013438278](.\image-20240413013438278.png)

当前的 Ability Task 在多人游戏环境下存在以下问题：

1. **客户端激活问题**：在客户端（Client）激活 Ability Task 时，由于网络延迟的存在，客户端发出的激活信号可能会比服务器（Server）接收到的激活信号慢。这可能导致客户端认为 Ability Task 已经激活，但实际上服务器还没有接收到激活信号。

2. **数据同步问题**：在客户端激活 Ability Task 后，客户端会向服务器发送 RPC 数据，但服务器接收到的客户端数据可能比客户端实际发送的数据晚。这可能导致服务器接收到的数据与客户端实际发送的数据不同步。

3. **数据验证问题**：由于网络延迟和数据同步问题的存在，服务器接收到的客户端激活信号和数据可能与客户端实际发送的信号和数据不同。服务器需要对这些数据进行验证，以确保数据的一致性和有效性。

![image-20240413013932288](.\image-20240413013932288.png)

面对这种情况，GAS（GameplayAbilitySystem）通过以下方式解决了Target Data的问题：

1. **Target Set Delegates**: 在服务器端，当Target Set发生变化时，会触发一个名为`FAbilityTargetDataSetDelegate`的Delegate。这个Delegate会广播到所有已连接的客户端，通知他们Target Set发生了变化。

2. **Target Data Map**: 在服务器端，有一个`AbilityTargetDataMap`，用于存储所有的Target Data。每个Target Data都有一个对应的Ability Spec，用于关联到具体的Ability。

3. **Server Set Replicated Target Data**: 当服务器需要将Target Data复制到客户端时，会调用`ServerSetReplicatedTargetData()`函数。这个函数会将Target Data从服务器复制到客户端，并更新客户端的Ability Target Data Map。

4. **Ability Target Data Map**: 在客户端，也有一个`AbilityTargetDataMap`，用于存储所有的Target Data。当服务器将Target Data复制到客户端时，客户端的`AbilityTargetDataMap`会被更新。

5. **Ability Spec to Target Data**: 在客户端，每个Ability Spec都关联着一个Target Data。当客户端的`AbilityTargetDataMap`被更新时，客户端的Ability Spec会自动更新其关联的Target Data。

通过这种方式，GAS实现了服务器和客户端之间Target Data的同步。当服务器的Target Data发生变化时，客户端会通过Delegate通知及时更新自己的Target Data Map，从而保证了Target Data的一致性。

![image-20240413014431044](.\image-20240413014431044.png)

在图中所示的Target Data方案中，Ability Task使用Target Data来确定目标。为了处理服务器Activate花费的时间epsilon小于delta（RPC到达的时间）以及RPC先到达这两种情况，可以采取以下策略：

1. **Activate和RPC的顺序**：
   - 如果服务器Activate花费的时间epsilon小于delta，这意味着服务器Activate指令先于RPC到达客户端。
   - 如果RPC先到达，这意味着客户端已经收到了服务器Activate指令，但是服务器Activate指令还未到达。

2. **处理策略**：
   - 对于服务器Activate花费的时间epsilon小于delta的情况，可以在服务器Activate后立即调用`CallReplicatedTargetDataDelegateIfSet()`函数，将Target Data广播给所有已连接的客户端。
   - 对于RPC先到达的情况，可以在客户端接收到RPC后，调用`ServerSetReplicatedTargetData()`函数，将Target Data复制到客户端的Ability Target Data Map中。

3. **时间差处理**：
   - 时间差处理主要涉及如何确保客户端和服务器之间的Target Data保持同步。
   - 一种方法是在服务器Activate后立即调用`CallReplicatedTargetDataDelegateIfSet()`函数，这样可以确保客户端在接收到Activate指令的同时也接收到了最新的Target Data。
   - 另一种方法是在客户端接收到Activate指令后，立即调用`ServerSetReplicatedTargetData()`函数，将Target Data复制到客户端的Ability Target Data Map中。

通过上述策略，可以有效地处理服务器Activate花费的时间epsilon小于delta以及RPC先到达这两种情况，确保客户端和服务器之间的Target Data保持同步。

# FGameplayAbilityTargetData_SingleTargetHit

在Unreal Engine 5 (UE5) 中，FGameplayAbilityTargetData_SingleTargetHit 是一个特定类型的 Target Data 类型，它表示对单个目标进行命中检测的结果。这个类继承自 FGameplayAbilityTargetData，它是用于描述 Ability 系统中目标选择和定位信息的基本结构。

FGameplayAbilityTargetData_SingleTargetHit 包含以下关键属性：

- `HitResult`: 这是一个 FHitResult 结构体，包含了命中检测的结果。它包括了被命中的物体、碰撞点、碰撞法线等信息。

- `HitActor`: 命中检测中所击中的 Actor。

- `HitComponent`: 命中检测中所击中的 Component。

- `HitLocation`: 命中检测中所击中的位置。

- `HitNormal`: 命中检测中所击中的法线方向。

- `HitTime`: 命中检测发生的时间。

- `HitChannel`: 命中检测发生的 Channel。

- `HitProxy`: 命中检测的 Proxy。

- `HitProxyId`: 命中检测的 Proxy 的 ID。

- `HitProxyWorldPosition`: 命中检测的 Proxy 在世界空间的位置。

- `HitProxyWorldNormal`: 命中检测的 Proxy 在世界空间的法线方向。

- `HitProxyWorldTangent`: 命中检测的 Proxy 在世界空间的切线方向。

- `HitProxyWorldBinormal`: 命中检测的 Proxy 在世界空间的副切线方向。

- `HitProxyLocalPosition`: 命中检测的 Proxy 在本地空间的位置。

- `HitProxyLocalNormal`: 命中检测的 Proxy 在本地空间的法线方向。

- `HitProxyLocalTangent`: 命中检测的 Proxy 在本地空间的切线方向。

- `HitProxyLocalBinormal`: 命中检测的 Proxy 在本地空间的副切线方向。

FGameplayAbilityTargetData_SingleTargetHit 主要用于描述对单个目标进行命中检测的结果，通常在需要精确打击或技能命中反馈的 Ability 中使用。例如，一个角色释放技能攻击敌人时，可以通过这个 Target Data 类型来获取命中检测的结果，以便在游戏中正确地显示命中效果和伤害计算。



# FScopedPredictionWindow

在 Unreal Engine 5 (UE5) 中，FScopedPredictionWindow 是一个用于控制预测窗口的类。预测窗口是指在客户端模拟游戏行为时，允许客户端提前模拟一段时间，以减少网络延迟对游戏体验的影响。

FScopedPredictionWindow 提供了一种方便的方法来管理预测窗口，使得在代码中可以轻松地指定预测窗口的开始和结束。它的工作原理如下：

1. **创建预测窗口**：当你创建一个新的 FScopedPredictionWindow 对象时，它会记录下当前的时间戳，作为预测窗口的开始时间。

2. **模拟预测**：在预测窗口内，客户端可以模拟游戏行为，例如移动、攻击等。这些操作会在本地进行模拟，然后在预测窗口结束后再与服务器进行同步。

3. **关闭预测窗口**：当你销毁 FScopedPredictionWindow 对象时，它会计算出预测窗口的持续时间，并将这个时间传递给 Ability System，以便 Ability System 能够正确地处理预测窗口内的操作。

通过使用 FScopedPredictionWindow，你可以更方便地控制预测窗口的开始和结束，从而更好地优化游戏的网络性能和游戏体验。



所以，使用客户端预测，我们将代码改为如下形式

```c++
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

```



现在，客户端已经能自行预测Ability，下一步就是让服务端去处理好客户端监听任务



# Receiving Target Data

当TargetData到达服务器时，会有一个委托广播（FAbilityTargetDataSetDelegate），我们可以通过AbilitySystemComponent来获取。

确切的说，是AbilitySystemComponent->Get().AbilityTargetDataSetDelegate()

AbilityTargetDataSetDelegate 是一个事件委托，用于在 Ability System 中设置 Target Data。当 Ability System 设置了新的 Target Data 时，它会触发这个委托。

这个委托的签名如下：

```cpp
1DECLARE_DELEGATE_OneParam(FAbilityTargetDataSetDelegate, const FGameplayAbilityTargetData* const)
```

他会返回目标数据集代理，但是，正如我们上面写的，我们广播的是 {给定能力：预测} 的键值对代理，换句话说，这里需要知道与这个TargetData相光联的预测键，因此，为了获取这些信息，我们在此调用的函数需要改特定任务的相关的SpecHandle.

## 

```c++
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

```



# UAbilitySystemGlobals::Get().InitGlobalData() 

在 Unreal Engine 5 (UE5) 中，UAbilitySystemGlobals::Get().InitGlobalData() 是一个函数，用于初始化 Ability System 的全局数据。这个函数通常在游戏启动时调用，以确保 Ability System 的全局数据被正确地初始化。

具体来说，UAbilitySystemGlobals::Get().InitGlobalData() 函数主要做了以下几件事：

1. **创建 Ability System 组件**：在 UAbilitySystemGlobals 类中，创建一个 Ability System 组件，并将其设置为全局的。

2. **初始化 Ability System 组件**：对全局的 Ability System 组件进行初始化，包括设置一些默认的属性和状态。

3. **创建 Ability System 资源**：创建一些 Ability System 所需的资源，例如 Ability System 的事件蓝图、事件蓝图的实例等。

4. **设置 Ability System 的默认属性**：设置一些 Ability System 的默认属性，例如 Ability System 的默认属性集、默认属性集的实例等。

5. **设置 Ability System 的默认状态**：设置一些 Ability System 的默认状态，例如 Ability System 的默认状态集、默认状态集的实例等。

6. **设置 Ability System 的默认组件**：设置一些 Ability System 的默认组件，例如 Ability System 的默认组件集、默认组件集的实例等。

7. **设置 Ability System 的默认 Target Data**：设置一些 Ability System 的默认 Target Data，例如 Ability System 的默认 Target Data 集、默认 Target Data 集的实例等。

总之，UAbilitySystemGlobals::Get().InitGlobalData() 函数主要用于初始化 Ability System 的全局数据，包括 Ability System 的组件、资源、属性、状态、组件和 Target Data。这个函数通常在游戏启动时调用，以确保 Ability System 的全局数据被正确地初始化。



所以，注意：因为我们上面的程序用到了 ASC的AbilityTargetDataMap，所以，需要再程序启动之前，就对这个进行初始化，调用的也正式 UAbilitySystemGlobals::Get().InitGlobalData() ，所以，我们在我们自己写的资产管理的初始化函数里，去调用这个函数

```c++

void UAuraAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FAuraGameplayTags::InitializeNativeGameplayTags();	// 初始化游戏标签
	
    // 对于使用AbilitySystem（Target Data）的项目，我们需要初始化全局数据，这是很有必要的
	UAbilitySystemGlobals::Get().InitGlobalData();	// 初始化全局数据，

	// 现在要做的最后一步，就是将这个AssetManager注册到GEngine中，设置为我们的项目的资产管理器
}

```

这点，很重要，否则在游戏进程运行时，你会遇到`ScriptStructCache`相关的错误且客户端将从服务器断开连接。在项目中这个方法只需要调用一次。

>实际上，在 5.3 版本中不需要。5.3及后续版本，已经自动调用了