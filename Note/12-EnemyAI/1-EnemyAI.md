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