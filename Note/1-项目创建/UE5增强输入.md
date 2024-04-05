# 增强输入（Enhanced Input）

在虚幻引擎5.3（UE5.3）中，增强输入（Enhanced Input）是对原有输入系统的革新，旨在提供一种更为强大、灵活且易于使用的输入处理机制。相比于之前的输入方式，增强输入系统主要改进和区别在于以下几个方面：

1. **模块化和可配置性提升**：
   - 增强输入采用了新的架构设计，包括输入操作（Input Actions）和输入映射情境（Input Mapping Contexts）等概念，允许开发者更加细致地定义和组织不同的输入行为，比如可以针对不同情境（如游戏菜单、游戏内控制、UI交互等）单独配置输入响应。

2. **动态重映射**：
   - 开发者现在可以在运行时轻松地重新映射输入，这意味着玩家可以根据自己的喜好自定义按键布局，或者游戏可以根据游戏状态动态改变输入响应。

3. **复合输入和修饰符**：
   - 增强输入支持更复杂的输入组合，例如通过拌合输入轴值（Axis Mappings）可以合并多个输入源（如两个摇杆方向合成一个动作），并且支持输入修饰符来修改输入的行为（如反转轴向、死区设置等）。

4. **矢量化输入**：
   - 输入不再局限于简单的布尔型触发，而是可以处理连续的数值输入，如鼠标平滑移动或手柄的双轴输入，这为实现更精细的操控体验提供了可能。

5. **易用性和一致性**：
   - 新的蓝图接口和可视化编辑工具让输入设置变得直观且统一，无论是在蓝图还是C++中设置输入，都能享受到一致且强大的输入处理能力。

总结来说，UE5.3的增强输入系统不仅简化了输入系统的管理和配置，还极大地增强了游戏对各种输入设备的支持能力和灵活性，能够更好地满足现代游戏设计中对于复杂输入逻辑和玩家个性化需求的要求。



## 工程实践

一个很简单的场景，假设我们希望在场景中控制角色的移动，对于这个移动操作，我们需要一个输入动作，这里以工程为例：

我们在工程中创建一个`input`文件夹，再创建一个名为`inputActions`的文件夹，所有的输入动作都将被放入到这个文件夹中。

现在为了进行增强输入，需要一个输入动作来接收设备的输入。

![image-20240405141926836](.\image-20240405141926836.png)

按照UE以前的惯例是为输入a添加一个动作，我们命名为mov，但是现在，我们想要打开一个move，我们真正希望改变的只有值的类型，对于这个move操作来说，我们希望它能处理二维运动（前后左右），这里前后是第一维度，左后是第二维度，所以我们洗完将值类型改为2D向量。这将允许我们以二维向量的形式作为输入值。

![image-20240405142006652](.\image-20240405142006652.png)

对于这个二维向量，我们希望x为左右，y为前后，记住，这里不要混淆，很重要。在三维空间中，我们通常将Actor的前向量（Forward）作为x轴，但是对于我们这个项目（场景）来说，我们谈论的是一个二维空间，对应键盘上的WASD，W和S用于前进和后退，我们将他们视为二维空间中的Y轴，A和D作为左右移动，他们视为二维空间中的X轴。

现在我们有了一个输入动作，需要将这个动作和角色光联起来。

这里就需要创建一个`输入映射上下文`

![image-20240405142739392](.\image-20240405142739392.png)

在输入映射上下文中，我们可以将按键和移动输入关联起来。打开这个文件，我们在`映射`这里，将我们之前添加的输入操作添加进去，关联起来

![image-20240405143028854](.\image-20240405143028854.png)

然后，我们继续添加操作映射，分别对应按键WASD。

![image-20240405143144204](.\image-20240405143144204.png)

当我们按下D的时候，我们希望我们接收到一个正数，按下A的时候，接收到一个负数，为了实现这点，我们可以在对应的按键映射哪里，添加一个修饰符来实现修改输入。就那A来说，我们在 `修改器`这里点击添加一个子项，然后在子项中选择否定。

![image-20240405143527945](.\image-20240405143527945.png)

同时，如果我们打开索引，可以发现他会否定3个轴的数据（xyz)，但是我们前面讨论过了，我们的左右移动（AD）只会出现在X轴上，所以我们在这里修改其他的轴向也没啥意义，所以这我们在索引这里取消勾选YZ，只否定我们需要的轴数据。

![image-20240405143818923](.\image-20240405143818923.png)

然后，我们解决了DA左右移动，现在来考虑前后（WS)。

当我们按下W的时候，我们希望得到一个正值，前向移动。而且，我们不希望在X轴上接收到它，也就是在二维空间下，我们希望在Y轴上得到一个正值。为此，我们必须添加一个修饰符，强制W在Y轴上提供输入数据。

所以这里我们可以在W键这里在修改器（修饰符）下拉选择 重新排列输入轴的值（拌合输入轴值）。然后选择将Y提前到最前面即 YXZ。

![image-20240405144836959](.\image-20240405144836959.png)

然后，对于S后移来说，也是一样的，因为我们还需要一个负值，所以这里在w的基础上，再加一个修饰符，否定即可。

![image-20240405144949051](.\image-20240405144949051.png)



然后在公共模块包含 `EnhancedInput`，这样才能引用头文件

```c++

using UnrealBuildTool;

public class Arcane : ModuleRules
{
	public Arcane(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
```

然后，这里项目里我希望在controller文件中处理输入，所以在controller文件中，我们定义 输入映射上下文

```c++
private:
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> AuraContext;	// 输入映射上下文
```

同时，我认为输入映射应该是很重要的一部分，如果游戏开发者没有定义这部分，那么我们将终止游戏进程。所以这里在begainPlay中断言 AuraContext，同时拿到输入增强子系统

```c++

#include "EnhancedInputSubsystems.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;		// 开启复制
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(AuraContext);	// 检查输入映射上下文是否存在，如果不存在则报错

	UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());		// 获取增强输入子系统

	check(EnhancedInputSubsystem);	// 检查增强输入子系统是否存在，如果不存在则报错

	EnhancedInputSubsystem->AddMappingContext(AuraContext, 0);	// 添加输入映射上下文，0表示优先级，因为只有一个输入映射上下文，所以优先级为0

	bShowMouseCursor = true;	// 显示鼠标光标
	DefaultMouseCursor = EMouseCursor::Default;	// 设置鼠标光标为默认

	FInputModeGameAndUI InputMode;	// 创建输入模式
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);	// 设置鼠标锁定模式为不锁定，这样我们就可以在窗口外移动鼠标
	InputMode.SetHideCursorDuringCapture(false);	// 设置捕获时隐藏鼠标光标为false，这样我们就可以在窗口外移动鼠标
	SetInputMode(InputMode);	// 设置输入模式
}

```

