# Enemy AI



![image-20240419174052941](.\image-20240419174052941.png)

在虚幻引擎5（UE5）中，行为树是一种强大的工具，用于实现敌人的AI行为。行为树由一系列节点组成，每个节点代表一种特定的行为或决策。这些节点按照一定的顺序排列，形成一棵树状结构，从而定义了敌人的行为逻辑。

行为树的工作方式如下：

1. **行为树组件**：行为树组件是一个附加在敌人AI控制器上的组件。它负责运行行为树，并根据行为树的逻辑控制敌人的行为。

2. **行为树实例化**：在敌人AI控制器中，通过实例化一个行为树来定义敌人的行为。行为树实例化后，会生成一个行为树对象，该对象包含了行为树的结构和逻辑。

3. **行为树节点**：行为树由一系列节点组成。每个节点代表一种特定的行为或决策。节点可以是简单的动作，如移动或攻击，也可以是复杂的决策过程，如寻找目标或避免障碍物。

4. **节点之间的连接**：节点之间通过连接线相连，表示它们之间的逻辑关系。连接线定义了行为树的执行流程，即哪些节点应该先执行，哪些节点应该后执行。

5. **Blackboard组件**：Blackboard组件是一个附加在敌人AI控制器上的组件，用于存储和管理行为树所需的数据。例如，Blackboard可以存储敌人的当前位置、目标位置、当前任务等信息。

6. **行为树运行**：行为树的运行由行为树组件负责。行为树组件会根据行为树的逻辑，控制敌人AI控制器的行为。例如，如果敌人发现目标，行为树可能会让敌人向目标移动；如果敌人受到攻击，行为树可能会让敌人进行反击。

通过这种方式，行为树能够为敌人AI控制器提供灵活的行为逻辑，使得敌人在游戏世界中表现出更加智能和自然的行为。



# AI

为了让我们的怪物更加智能，现在需要给怪物添加AI组件。（使用AAIController需要再项目构建CS中包含AI模块 `AIModule`）

> 首先，需要一个AIController，按照惯例，需要重写一个基于AIController的控制（AuraAIController)。该类需要添加一个成员，行为树组件（BehaviorTreeComponent）
>
> ```c++
> 
> #pragma once
> 
> #include "CoreMinimal.h"
> #include "AIController.h"
> #include "AuraAIController.generated.h"
> 
> class UBlackboardComponent;
> class UBehaviorTreeComponent;
> 
> /**
>  * 
>  */
> UCLASS()
> class ARCANE_API AAuraAIController : public AAIController
> {
> 	GENERATED_BODY()
> 
> public:
> 	AAuraAIController();
> 
> protected:
> 
> 	UPROPERTY()
> 	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;
> 	
> };
> 
> ```
>
> 其次，我们EnemyCharacter需要有这个AuraAiController，还需要有一个行为树（UBehaviorTree
>
> ```c++
> 
> 	UPROPERTY(EditAnywhere, Category="AI")
> 	TObjectPtr<UBehaviorTree> BehaviorTree;
> 
> 	UPROPERTY()
> 	TObjectPtr<AAuraAIController> AuraAIController;
> 
> ```
>
> 然后在PossessedBy函数下初始化控制器（AI相关的只能在服务进行，所以需要判断是否是服务端）
>
> ```c++
> 
> void AAuraEnemy::PossessedBy(AController* NewController)
> {
> 	Super::PossessedBy(NewController);
> 
> 	if (HasAuthority())
> 	{
> 		AuraAIController = Cast<AAuraAIController>(NewController);	// 设置AI控制器
> 
> 		AuraAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);	// 初始化黑板
> 
> 		AuraAIController->RunBehaviorTree(BehaviorTree);	// 运行行为树
> 	}
> 	
> }
> ```
>
> 
>
> 然后回到项目，在蓝图文件夹下，新建如下3个基础蓝图（AIController用我们新写的AAuraAIController做基类）
>
> ![image-20240419191650678](.\image-20240419191650678.png)
>
> 然后，将BP_EnemyCharacyterBase中的各项AI配置绑定到我们新创建的这几个上（AIController和BehaviorTree）
>
> 

# Behavior Tree Service （行为树服务）

Behavior Tree Service 是一个用于实现复杂 AI 行为的框架，它提供了一种结构化的方式来组织和执行 AI 的行为。在虚幻引擎中，Behavior Tree Service 被广泛应用于游戏中的各种 AI 行为，包括敌人的 AI 控制器、NPC 的行为等。

Behavior Tree Service 的核心思想是将复杂的 AI 行为分解为一系列简单的决策和动作，然后通过行为树（Behavior Tree）的形式来组织和执行这些决策和动作。行为树由一系列节点组成，每个节点代表一个决策或动作。节点之间通过连接线相连，表示它们之间的逻辑关系。这种结构化的组织方式使得 AI 的行为逻辑更加清晰易懂，同时也使得 AI 的行为更加灵活和可定制。

在虚幻引擎中，Behavior Tree Service 提供了一系列内置的节点，包括条件节点、动作节点、复合节点等。开发者可以根据需要，通过组合这些节点来实现复杂的 AI 行为。同时，Behavior Tree Service 还提供了可视化编辑器，使得开发者可以方便地创建和编辑行为树。

总之，Behavior Tree Service 是一个强大的工具，可以帮助开发者实现复杂 AI 行为，提高游戏 AI 的表现力和智能程度。

```c++
// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlueprintBase.h"
#include "BTService_FindNearestPlayer.generated.h"

/**
 * 寻找最近的玩家
 */
UCLASS()
class ARCANE_API UBTService_FindNearestPlayer : public UBTService_BlueprintBase
{
	GENERATED_BODY()

public:

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};

```

然后，在行为树的Selector节点中，添加上这个服务（寻找最近的玩家），这样，我们就为我们的行为树添加一个功能节点，接下来就是要具体实现这个寻敌逻辑了

![image-20240419202049942](.\image-20240419202049942.png)



# Blackboard Keys

在虚幻引擎中，Blackboard Keys 是行为树组件中 Blackboard 组件的一部分，用于存储和管理行为树所需的数据。Blackboard Keys 是一组键值对，用于在 Blackboard 组件中存储和检索数据。

Blackboard Keys 的工作原理如下：

1. **创建 Blackboard Keys**：在 Blackboard 组件中，开发者可以创建任意数量的 Blackboard Keys。每个 Blackboard Key 都有一个唯一的键名和一个数据类型。

2. **设置 Blackboard Keys 的值**：在行为树中，开发者可以使用 Blackboard Set Value 节点来设置 Blackboard Keys 的值。Blackboard Set Value 节点需要指定一个 Blackboard Key 的键名和一个数据值。

3. **读取 Blackboard Keys 的值**：在行为树中，开发者可以使用 Blackboard Get Value 节点来读取 Blackboard Keys 的值。Blackboard Get Value 节点需要指定一个 Blackboard Key 的键名，并返回该键的值。

4. **使用 Blackboard Keys 的值**：在行为树中，开发者可以使用其他节点来使用 Blackboard Keys 的值。例如，如果一个节点需要知道敌人的当前位置，它可以使用 Blackboard Get Value 节点来获取该位置的值。

通过这种方式，Blackboard Keys 可以为行为树提供动态的数据支持，使得行为树可以根据游戏中的实际情况做出相应的决策和动作。



![image-20240419204211519](.\image-20240419204211519.png)

这里我们在行为树的黑板里添加两个变量，一个用来存储我们要NPC跟随（寻找）的Actor，一个是跟随距离。

TargetToFollow 本是一个Object类型的变量，因为我们本意是想寻找Actor，所以将基类改为Actor更合适

然后，回到C++中，我们为这两个变量创建两个选择器

```c++

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FBlackboardKeySelector TargetToFollowSelector;		// 要跟随的目标

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FBlackboardKeySelector DistanceToTargetSelector;	// 距离目标的距离
```

然后，我们可以在编辑器中，将这些选择器和蓝图里的变量光临起来

![image-20240419211010592](.\image-20240419211010592.png)

为了区分玩家和怪物，这里我们给Character添加两个标签，就简单的在蓝图中设置就行

![image-20240419205438844](.\image-20240419205438844.png)

![image-20240419205507713](.\image-20240419205507713.png)

然后通过Pawn 的 ActorHasTag 成员函数就能获取到标签内容

`UGameplayStatics::GetAllActorsWithTag`能获取到场景中所有拥有指定Tag的Actor

```c++
void UBTService_FindNearestPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	const APawn* OwnerPawn = AIOwner->GetPawn();

	// 获取所有玩家, 通过Tag来区分敌人和玩家
	const FName TargetTag = OwnerPawn->ActorHasTag("Player") ? FName("Enemy") : FName("Player");

	TArray<AActor*> ActorsWithTag;	// 用来存储所有拥有Tag的Actor
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TargetTag, ActorsWithTag);	// 获取所有拥有Tag的Actor

	float ClosestDistance = FLT_MAX;	// 最近的距离
	AActor* ClosestActor = nullptr;		// 最近的Actor

	// 遍历所有拥有Tag的Actor，找到最近的一个
	for (AActor* Actor : ActorsWithTag)
	{
		if (!IsValid(Actor) || Actor == OwnerPawn)
		{
			continue;
		}
		const float Distance = OwnerPawn->GetDistanceTo(Actor);

		if (Distance < ClosestDistance)
		{
			ClosestDistance = Distance;
			ClosestActor = Actor;
		}
	}

	// 设置黑板值（将最近的Actor设置为要跟随的目标）
	UBTFunctionLibrary::SetBlackboardValueAsObject(this, TargetToFollowSelector, ClosestActor);
	UBTFunctionLibrary::SetBlackboardValueAsFloat(this, DistanceToTargetSelector, ClosestDistance);

}
```

然后，让怪物简单的做个向目标移动

![image-20240419213151467](.\image-20240419213151467.png)

![image-20240419213235726](.\image-20240419213235726.png)

> 我发现,NPC在旋转的时候，很生硬，这里有个解决办法是，关闭NPC的Pawn转身，使用MovementComponet的控制器来控制转身，同时调整转身速率即可
>
> ![image-20240419215214450](.\image-20240419215214450.png)
>
> 当然，这种基本的东西，在C++里写好最好，在EnemyCharacter类中
>
> ```c++
> 
> 	bUseControllerRotationYaw = false;	// 关闭控制器旋转偏航
> 	bUseControllerRotationPitch = false;	// 关闭控制器旋转俯仰
> 	bUseControllerRotationRoll = false;	// 关闭控制器旋转翻滚
> 
> 	GetCharacterMovement()->bUseControllerDesiredRotation = true;	// 开启控制器期望旋转
> 
> ```
>
> 

# Behavior Tree Decorators（行为树装饰器）

在虚幻引擎中，行为树装饰器（Behavior Tree Decorators）是一类特殊的节点，用于控制行为树的执行流程。装饰器节点通常位于行为树的顶部，用于决定是否执行下面的行为树节点。

装饰器节点有以下几种类型：

1. **条件装饰器**（Condition Decorator）：条件装饰器用于检查某个条件是否满足。如果条件满足，则执行下面的行为树节点；否则，跳过下面的行为树节点。

2. **循环装饰器**（Loop Decorator）：循环装饰器用于重复执行下面的行为树节点，直到满足某个条件为止。常见的循环装饰器包括 While Loop Decorator 和 Until Loop Decorator。

3. **优先级装饰器**（Priority Decorator）：优先级装饰器用于选择下面的哪个行为树节点来执行。常见的优先级装饰器包括 First Child Succeeds Decorator 和 First Child Fails Decorator。

4. **中断装饰器**（Interruptable Decorator）：中断装饰器用于允许下面的行为树节点在运行时被中断。常见的中断装饰器包括 Allow Interrupting Decorator 和 Prevent Interrupting Decorator。

通过使用装饰器节点，开发者可以控制行为树的执行流程，使得行为树能够根据游戏中的实际情况做出相应的决策和动作。



# Attack Behavior Tree Task （自定义行为树任务-攻击行为）

![image-20240420000720770](.\image-20240420000720770.png)

如上，我们已经可以利用行为树做到简单的AI跟随效果，但是，对于像攻击这类复杂行为，是没有现成的节点，所以，接下来我们需要自定义Behavior Task

蓝图实现的话，可以通过这里新建任务

![image-20240420001105884](.\image-20240420001105884.png)

### C++添加

```c++
// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "BTTask_Attack.generated.h"

/**
 * 
 */
UCLASS()
class ARCANE_API UBTTask_Attack : public UBTTask_BlueprintBase
{
	GENERATED_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;	// 执行任务
	
};

```

> TIPS：我们可以加大动画混合空间中的平滑权重来减少动画切换时的角色抖动问题
>
> ![image-20240420004701160](.\image-20240420004701160.png)



# Find New Location Around Target（寻找一个随机的寻路可达的坐标点）

任务蓝图，其实原理很简单，我们现在在行为树里，已经可以获取到跟随目标的位置了，这里直接传入目标位置，使用导航系统的 GetRandomLocationInNavigableRadius 函数在指定位置以一个指定半径内获得一个随机的导航路径可达的坐标点，然后，将点保存到黑板中

![image-20240420010756642](.\image-20240420010756642.png)

然后在行为树中，当怪物移动到攻击范围内，我们执行一次攻击，然后生成一个随机点，然后让怪物移动到点（为了防止卡死，我们给移动任务添加一个时间限制修饰符，超时就取消任务，进入下次行为循环）

![image-20240420011343741](.\image-20240420011343741.png)

