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

