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

