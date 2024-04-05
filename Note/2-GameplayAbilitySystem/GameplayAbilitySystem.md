# Gameplay Ability System

UE5（虚幻引擎5）的Gameplay Ability System（简称GAS）是一个强大且灵活的游戏内角色技能和属性管理系统。GAS设计用于帮助开发者更方便地实现和维护游戏角色的各种能力，如攻击、移动、施法、buff/debuff应用以及相关的冷却时间、消耗资源（如能量、法力）等功能。以下是GAS的一些核心组件和概念：

1. **Gameplay Ability (GA)**:
   - Gameplay Abilities是GAS的核心单元，代表游戏中可执行的动作或效果。每个Ability都可以定义自己的激活条件、执行逻辑、消耗资源、冷却时间和关联的效果。

2. **Ability System Component (ASC)**:
   - ASC是所有游戏对象（通常是Actor）上用于集成Gameplay Ability系统的组件。它负责处理Ability的激活、取消、冷却管理以及与属性系统之间的交互。

3. **Attribute Set**:
   - 属性集（AttributeSet）用于存储角色或游戏对象的状态属性，如生命值、魔法值、攻击力等。这些属性可以由Gameplay Abilities读取、修改或作为激活条件。

4. **Gameplay Effects (GE)**:
   - Gameplay Effects是应用于目标Actor的临时或永久性状态变化，可以改变属性值、添加状态标签或触发其他游戏行为。

5. **Gameplay Effect Aggregator (GEA)**:
   - Gameplay Effect Aggregator负责聚合多个同类型Gameplay Effects，确保它们按照预期正确叠加或冲突解决。

6. **Gameplay Tags**:
   - 游戏标签是一种灵活的分类和过滤机制，用来标记和筛选Ability、Actor或其他游戏元素，以便在游戏中进行动态交互和决策。

7. **Gameplay Cue**:
   - 游戏提示（Cue）是一种事件通知机制，用于触发视觉、听觉和其他反馈，例如播放动画、粒子效果或声音。

8. **Gameplay Ability Tasks (GATs)**:
   - Gameplay Ability Tasks是预定义的任务节点，可用于构建异步Ability逻辑，比如等待动画播放完毕后执行某个动作。

GAS设计的目标是模块化和可扩展性，允许开发者通过蓝图和/或C++自定义和扩展系统，实现复杂的战斗系统、角色成长体系以及其他基于技能的游戏玩法。此外，GAS还支持网络同步和预测，确保多人在线游戏中技能的正确和一致执行。



![image-20240405231019194](.\image-20240405231019194.png)

对于这个项目来说，我们的怪物角色将会直接拥有他们的 Ability System Component，以及对应的属性集。

而对于我们主角角色来说，我们会将这些能力组件和属性集在玩家状态上进行设置。



**要使用该功能，我们需要再插件里开启该插件**

![image-20240405232412478](.\image-20240405232412478.png)



每次添加新的插件后，记得去项目构建文件里(xx.build.cs)添加依赖。 对于GAS，我们需要添加 `"GameplayAbilities", "GameplayTags", "GameplayTasks"`，这里我把他添加到private依赖中。

```c#
using UnrealBuildTool;

public class Arcane : ModuleRules
{
	public Arcane(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });

		PrivateDependencyModuleNames.AddRange(new string[] { "GameplayAbilities", "GameplayTags", "GameplayTasks" });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}

```





创建 游戏能力组件 和 属性集类

```c++

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class ARCANE_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
};
```

```c++
#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AuraAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class ARCANE_API UAuraAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
};

```

