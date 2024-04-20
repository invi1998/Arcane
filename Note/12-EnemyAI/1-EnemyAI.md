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



# Environment Query System（环境查询系统）

![image-20240420011658911](.\image-20240420011658911.png)

在虚幻引擎5（UE5）中，环境查询系统（Environment Query System，简称EQS）是一个强大的工具，用于帮助AI角色在游戏环境中做出决策。它允许AI角色根据一系列特定的规则和条件，从游戏环境中选择最佳的位置、方向或对象。环境查询系统可以用于实现复杂的AI行为，例如寻路、导航、资源搜索等。

环境查询系统的工作原理如下：

1. **创建环境查询**：在行为树中，开发者可以使用环境查询节点（Environment Query Node）来创建一个环境查询。环境查询节点需要指定一个环境查询类型（例如，找到最近的掩体），以及一组查询参数（例如，最大距离、最小遮挡角度等）。

2. **执行环境查询**：环境查询节点会根据指定的参数，从游戏环境中搜索符合要求的位置、方向或对象。搜索结果将存储在一个环境查询结果集（Environment Query Result Set）中。

3. **使用环境查询结果**：环境查询结果集可以用于驱动其他行为树节点的行为。例如，如果一个AI角色需要找到最近的掩体来躲避敌人的攻击，它可以通过环境查询节点找到最近的掩体，然后使用其他节点来控制AI角色的移动。

通过这种方式，环境查询系统可以为行为树提供动态的数据支持，使得行为树可以根据游戏中的实际情况做出相应的决策和动作。环境查询系统还可以与其他行为树组件（如行为树服务、黑板组件等）一起使用，以实现更复杂的行为。

![image-20240420012056902](.\image-20240420012056902.png)

图中展示了一个环境查询方案，用于寻找离玩家最近的敌人。在这个方案中，环境查询系统（Environment Query System，简称EQS）被用来搜索游戏环境中的特定对象，并根据某些条件对这些对象进行排序。

具体来说，这个环境查询方案包含以下几个部分：

1. **测试（Tests）**：在环境查询系统中，测试是用来评估对象是否满足特定条件的。在这个方案中，有两个测试：Trace和Distance。Trace测试用于检查对象是否在视线范围内，而Distance测试用于检查对象与玩家之间的距离。

2. **物品（Items）**：在环境查询系统中，物品是指要搜索的对象。在这个方案中，物品是敌人（Enemy）。

3. **环境查询（Environment Query）**：环境查询是环境查询系统的核心部分，它负责执行搜索并返回结果。在这个方案中，环境查询的目标是找到离玩家最近的敌人。

4. **结果集（Result Set）**：环境查询的结果会被存储在一个结果集中。在这个方案中，结果集包含所有满足条件的敌人，并且按照距离从近到远进行排序。

通过这个环境查询方案，开发人员可以轻松地在游戏环境中搜索特定的对象，并根据特定的条件对这些对象进行排序。这使得开发人员可以为游戏中的AI角色设计出更智能的行为，例如寻找最近的敌人进行攻击，或者寻找最近的掩体进行躲避。



# EQS 生成器 (Generators)

![image-20240420013345059](.\image-20240420013345059.png)

在虚幻引擎5（UE5）的环境查询系统（EQS）中，生成器（Generators）是一组用于生成环境查询结果的组件。生成器负责从游戏环境中搜索特定的对象，并将这些对象作为环境查询结果返回。不同的生成器类型适用于不同的搜索场景和需求。

以下是环境查询系统中常见的生成器类型及其功能和使用建议：

1. **Actors Of Class**：这个生成器用于搜索游戏环境中具有特定类别的对象。例如，你可以使用这个生成器来搜索所有的敌人（ActorOfClass(Enemy)）。这个生成器通常用于搜索特定类型的对象。

   ![image-20240420014451502](.\image-20240420014451502.png)

2. **Composite**：这个生成器用于组合多个生成器的结果。例如，你可以使用这个生成器来搜索离玩家最近的敌人和最近的掩体（Closest To Player(ActorOfClass(Enemy)) & Closest To Player(ActorOfClass(Shelter))）。这个生成器通常用于组合多个生成器的结果。

   

3. **Current Location**：这个生成器用于返回当前角色的位置。例如，你可以使用这个生成器来搜索当前位置周围的敌人（Current Location(ActorOfClass(Enemy))）。这个生成器通常用于基于角色当前位置的搜索。

4. **Perceived Actors**：这个生成器用于返回角色感知到的物体。例如，你可以使用这个生成器来搜索角色视野范围内的敌人（Perceived Actors(ActorOfClass(Enemy))）。这个生成器通常用于基于角色感知的搜索。

5. **Points: Circle**：这个生成器用于在圆形区域内生成点。例如，你可以使用这个生成器来搜索角色周围一定半径内的敌人（Points: Circle(ActorOfClass(Enemy), 100)）。这个生成器通常用于基于圆形区域的搜索。

   ![image-20240420014729850](.\image-20240420014729850.png)

6. **Points: Cone**：这个生成器用于在锥形区域内生成点。例如，你可以使用这个生成器来搜索角色前方一定角度范围内的敌人（Points: Cone(ActorOfClass(Enemy), 80, 45)）。这个生成器通常用于基于锥形区域的搜索。

   ![image-20240420014837224](.\image-20240420014837224.png)

7. **Points: Donut**：这个生成器用于在环形区域内生成点。例如，你可以使用这个生成器来搜索角色周围一定半径内的敌人，但不包括角色自身（Points: Donut(ActorOfClass(Enemy), 100, 50)）。这个生成器通常用于基于环形区域的搜索。

   ![image-20240420014945467](.\image-20240420014945467.png)

8. **Points: Grid**：这个生成器用于在网格区域内生成点。例如，你可以使用这个生成器来搜索角色周围一定半径内的敌人，但不包括角色自身（Points: Grid(ActorOfClass(Enemy), 100, 10, 10, 10)）。这个生成器通常用于基于网格区域的搜索。

   ![image-20240420015021103](.\image-20240420015021103.png)

9. **Points: Pathing Grid**：这个生成器用于在路径网格区域内生成点。例如，你可以使用这个生成器来搜索角色周围一定半径内的敌人，但不包括角色自身（Points: Pathing Grid(ActorOfClass(Enemy), 100, 10, 10, 10)）。这个生成器通常用于基于路径网格区域的搜索。

   ![image-20240420015120704](.\image-20240420015120704.png)

在使用环境查询系统时，选择合适的生成器类型取决于你的搜索需求。例如，如果你需要搜索角色周围一定半径内的敌人，那么你应该选择“Points: Circle”生成器。如果你需要搜索角色前方一定角度范围内的敌人，那么你应该选择“Points: Cone”生成器。





# EQS Test

![image-20240420125120836](.\image-20240420125120836.png)

在虚幻引擎5（UE5）的环境查询系统（EQS）中，添加到生成器节点的测试方式可以用来进一步筛选和过滤生成器产生的点。这些测试方式可以用来检查点与特定对象的交集、距离、向量点积、游戏标签、重叠、寻路、寻路批次、投影、追踪、体积等属性。以下是一些常用的测试方式及其功能和使用建议：

1. **Distance**：这个测试方式可以用来检查点与特定对象的距离。例如，你可以使用这个测试方式来检查点与敌人之间的距离。这个测试方式通常用于计算点与对象之间的距离。

2. **Dot**：这个测试方式可以用来检查两个向量之间的点积。例如，你可以使用这个测试方式来检查点与敌人之间的方向。这个测试方式通常用于计算点与对象之间的方向。

3. **Gameplay Tags**：这个测试方式可以用来检查点与特定游戏标签的关系。例如，你可以使用这个测试方式来检查点是否与特定的游戏标签相关联。这个测试方式通常用于检查点与特定游戏标签之间的关系。

4. **Overlap**：这个测试方式可以用来检查点与特定对象的重叠情况。例如，你可以使用这个测试方式来检查点是否与敌人重叠。这个测试方式通常用于检查点与对象之间的重叠情况。

5. **Pathfinding**：这个测试方式可以用来检查点与特定寻路网格的关系。例如，你可以使用这个测试方式来检查点是否在寻路网格内。这个测试方式通常用于检查点与寻路网格之间的关系。

6. **Pathfinding Batch**：这个测试方式可以用来检查点与特定寻路批次的关系。例如，你可以使用这个测试方式来检查点是否在特定的寻路批次内。这个测试方式通常用于检查点与寻路批次之间的关系。

7. **Project**：这个测试方式可以用来将点投影到特定的平面或曲面上。例如，你可以使用这个测试方式来将点投影到地面。这个测试方式通常用于将点投影到特定的平面或曲面上。

8. **Trace**：这个测试方式可以用来检查点与特定对象的碰撞情况。例如，你可以使用这个测试方式来检查点是否与敌人相撞。这个测试方式通常用于检查点与对象之间的碰撞情况。

   > 这里，我们给测试提供一个查询情景（父类：环境查询情景蓝图（EnvQueryContext_BlueprintBase）），然后在蓝图里，我们重写 `ProvidActorsSet`（返回Actor集合），里面就做一件事，就是查询场景中所有指定类名的Actors并返回
   >
   > ![image-20240420130801081](.\image-20240420130801081.png)
   >
   > 然后将重新情景装配给test节点
   >
   > ![image-20240420131125683](.\image-20240420131125683.png)
   >
   > 然后，回到我们的测试关卡中，查看查询结果。可以看到，我们查询情景中药找的是AuraCharacter，但是场景里我们没有放置这类角色，所以自然查询结果全部为空
   >
   > ![image-20240420130900499](.\image-20240420130900499.png)
   >
   > 那我们在场景中放置一些Aura角色，可以看到，成功查询到场景中的Aura了
   >
   > ![image-20240420131405394](.\image-20240420131405394.png)
   >
   > 然后注意：查询条件里有个设置 `布尔匹配`，这个值勾选与否，决定了我们查询的结果是`保留`还是`过滤`。，上图显示的结果是勾选状态下的得分情况，当我们取消勾选，再看结果，如下
   >
   > ![image-20240420133442575](.\image-20240420133442575.png)
   >
   > ![image-20240420132834914](.\image-20240420132834914.png)
   >
   > 为了更清晰的理解这个`过滤`还是`保留`的概念，这里我们拖拽一个立方体来辅助理解
   >
   > ![image-20240420133401955](.\image-20240420133401955.png)
   >
   > 上图是未勾选`布尔匹配`下的场景查询结果，我们可以读出如下信息，图中红色的点是被丢弃的，蓝色的点是保留的。什么意思？意思就是，在Aura的视角下，这些红色是路径可达（视线可达的点 `因为我们使用的查询方式是Trace射线检测`，但是我们取消勾选布尔匹配，表示，我们将过滤掉这些点，保留的是重新失败的点，也就是那些视线阻挡的点，蓝色点）。这就是这个`布尔匹配`的含义和用法。
   >
   > 那我们开启勾选，结果如下
   >
   > ![image-20240420133911681](.\image-20240420133911681.png)
   >
   > 可以看到，结果反过来了
   >
   > 我们进入到立方体内部，可以发现，模型里面这些点依旧可以查询到Aura，emmmm，这其实是我们不希望的，我们不希望有任何的点位于模型内部，这是因为这个模型我们还没有给他添加碰撞，所以射线可达可穿透。
   >
   > ![image-20240420134518670](.\image-20240420134518670.png)
   >
   > 在添加了碰撞后，我们还可以给场景添加导航网格
   >
   > ![image-20240420134843440](.\image-20240420134843440.png)
   >
   > ![image-20240420135304016](.\image-20240420135304016.png)
   >
   > ![image-20240420135422698](.\image-20240420135422698.png)
   >
   > 这样，模型内部就不在会有生成点了，因为这是一个路径网格，路径与寻路AI有关，AI用的是NAV网格边界体积，所以，路径网格不会给我们提供任何无法导航的点
   >
   > 

   

   

9. **Volume**：这个测试方式可以用来检查点是否在特定的体积内。例如，你可以使用这个测试方式来检查点是否在特定的体积内。这个测试方式通常用于检查点与体积之间的关系。

在使用环境查询系统时，选择合适的测试方式取决于你的搜索需求。例如，如果你需要检查点与敌人之间的距离，那么你应该选择“Distance”测试方式。如果你需要检查点与敌人之间的方向，那么你应该选择“Dot”测试方式。