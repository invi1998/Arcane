# Input Config Data Asset

如何将输入与Ability System组件绑定以及如何使用Enhanced Input进行输入映射。

在以前的版本中，输入直接绑定到Ability，而现在通过Input Mapping Context将Input Actions与输入绑定，可以根据输入激活Ability。此外，还可以通过数据驱动的方式在运行时改变Input-to-Ability的映射关系。这样设计的目的是为了更加灵活地控制和管理输入与Ability之间的关系，提高游戏的可定制性和灵活性。

![image-20240411121448958](.\image-20240411121448958.png)

现在，我们已经创建了一个能力，并且我们已经看到我们可以在授予它们后立即激活能力，这非常棒。但是我们希望能够通过输入来激活能力。

所以让我们讨论一下可能的方法。

现在，在增强输入之前，我们可以直接将输入绑定到能力系统组件。可以直接将输入绑定到技能。因此，您按下一个按钮，能力自动接收该输入，并根据您编码的能力来激活或执行任何它想要执行的操作。

现在，这是通过创建一个具有能力输入常量的枚举来完成的，因此它是一个刚性系统，您可以将这些枚举常量硬编码到能力输入枚举中，并且这些输入将与特定的能力相关联。

在旧的输入系统被弃用之前，我们可以做到这一点，但现在我们有了增强的输入，事情做起来有些不同，而且我认为这些变化是好的。输入操作通过输入映射上下文与输入绑定。这就是增强输入的工作原理。

现在，我们可以根据这些输入决定如何激活能力，并且我们可以以任何我们喜欢的方式来做到这一点。

由 Epic Games 提供的多人射击游戏示例项目 Lyra，为我们提供了一个将增强输入与游戏能力系统能力相连接的示例。这有点过度设计。Lyra 是一个很好的示例，但它可能过于复杂和难以理解。我们将采用类似 Lyra 的方式，但相对简单一些，因为我们不需要 Lyra 提供的过度灵活性。因此，我们的方法会相对简单一些，但这并不意味着它会更加无用或缺乏灵活性。

我们将采用数据驱动的方法。我希望能够在运行时更改输入到能力映射。换句话说，按下鼠标左键来激活特定的能力应该是我们能够在运行时更改的事情。幸运的是，由于游戏能力系统的灵活性，只要我们编写得好，这实际上并不难做到。

现在我们要做的是创建一个包含输入动作的数据资产，但我们将把输入动作与游戏标签关联起来。

我希望每个输入都有一个游戏标签。例如，一个键，左鼠标按钮，右鼠标按钮等等。在运行时，我们应该能够为我们的游戏能力分配各种标签。然后，当我们激活能力时，每个能力都将与其输入标签关联，可以进行检查和更改。



因此，第一步是创建这个数据资产。这个数据资产，有一个成员，它里面存储了项目里所有输入相关的标签和该输入绑定的动作（就是按键和按键响应动作）

```c++
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AuraInputConfig.generated.h"

// 这个结构体用来存储输入动作的信息
USTRUCT(BlueprintType)
struct FAuraInputAction
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	const class UInputAction* InputAction = nullptr;	// 用来绑定输入的输入动作

	UPROPERTY(EditDefaultsOnly)
	const FGameplayTag InputTag = FGameplayTag();	// 输入动作的标签

};

/**
 * 
 */
UCLASS()
class ARCANE_API UAuraInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:

	const UInputAction* GetInputActionByTag(const FGameplayTag& Tag, bool bLogNotGet = false) const;	// 通过标签获取输入动作

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FAuraInputAction> AbilityInputActions;	// 用来存储所有的输入动作信息，包含特定输入动作和对应的标签


};

```

然后，接下来去GameplayTag中，添加我们需要的输入，将其作为Tag资产

```c++
	/*
	 * Input
	 */
	FGameplayTag InputTag_LeftMouseButton;						// 输入：左键
	FGameplayTag InputTag_RightMouseButton;						// 输入：右键
	FGameplayTag InputTag_1;									// 输入：1
	FGameplayTag InputTag_2;									// 输入：2
	FGameplayTag InputTag_3;									// 输入：3
	FGameplayTag InputTag_4;									// 输入：4
	FGameplayTag InputTag_5;									// 输入：5
	FGameplayTag InputTag_6;									// 输入：6
	FGameplayTag InputTag_7;									// 输入：7
	FGameplayTag InputTag_8;									// 输入：8
	FGameplayTag InputTag_9;									// 输入：9
	FGameplayTag InputTag_0;									// 输入：0
```

然后，将这些Tag初始化原生GameplayTags

```c++
/*
 * Input 按键输入
 */
GameplayTags.InputTag_LeftMouseButton = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("InputTag.LeftMouseButton"),
	FString("Input Tag for the Left Mouse Button")
);

GameplayTags.InputTag_RightMouseButton = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("InputTag.RightMouseButton"),
	FString("Input Tag for the Right Mouse Button")
);

GameplayTags.InputTag_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("InputTag.1"),
	FString("Input Tag for the 1 Key")
);

```

下一步就是在编辑器中创建我们新的数据资产（Input）

1：创建输入操作，输入类型因为我们都是单值类型，所以选择Axis1D(float)。

2：创建数据资产，类选择我们的UAuraInputConfig，

3：在我们的输入映射上下文中，添加上我们新增的这些输入操作

![image-20240411160813735](.\image-20240411160813735.png)

4：接下来只需要在我们的数据资产中将我们的inputAction和Tag标签绑定即可

![image-20240411160956616](.\image-20240411160956616.png)

> 这样使用数据资产有个很不错的特性，就是我们可以在运行时动态的交换我们的数据资产，从而让输入执行不同的动作，比如，游戏运行时，我们既可以用鼠标左键去点击怪物然后执行攻击指令，又可以在切换出背包界面的时候或者点击物品的时候执行拾取物品的指令
>
> 所以输入增强是一个很方便也很强大改进。同时通过数据资产我们也可以更模块化规整的管理我们的输入



# AuraEhanceInputComponent

该组件主要用来给Input数据资产或者说InputGameTag绑定输入回调函数。这里写了一个模板函数来进行实现

```c++

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "AuraInputConfig.h"
#include "AuraEnhancedInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class ARCANE_API UAuraEnhancedInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	// 该模板函数用于绑定按键事件，将按键事件绑定到对应的函数上，模板类型为UserClass，PressedFuncType，ReleasedFuncType，HeldFuncType
	// UserClass为绑定事件的对象，PressedFuncType为按下事件的函数类型，ReleasedFuncType为释放事件的函数类型，HeldFuncType为按住事件的函数类型
	template<typename UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType >
	void BindAbilityActions(const UAuraInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc);
	
};

template <typename UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
void UAuraEnhancedInputComponent::BindAbilityActions(const UAuraInputConfig* InputConfig, UserClass* Object,
	PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc)
{
	check(InputConfig);

	for (const FAuraInputAction& Action : InputConfig->AbilityInputActions)
	{
		// 为所有的输入动作绑定按下，释放，和按住事件
		// 为什么要为所有的输入动作绑定相同的按下，释放，和按住事件呢？
		// 因为我们有Ability去传递GamplayTag，所以如果我们将所有的输入动作绑定到相同的按下，释放，和按住事件上，我们就可以在这个事件中传递GamplayTag
		// 然后根据GamplayTag来识别这些输入，传递给AbilitySystemComponent，然后由ASC决定可以激活哪些能力
		if (Action.InputAction && Action.InputTag.IsValid())
		{
			if (PressedFunc)
			{
				// 绑定按下事件，Started表示按下事件，只会在开始调用一次，Object为绑定事件的对象，PressedFunc为按下事件的函数，Action.InputTag为PressedFunc的参数
				BindAction(Action.InputAction, ETriggerEvent::Started, Object, PressedFunc, Action.InputTag);
			}

			if (ReleasedFunc)
			{
				// 绑定释放事件，Completed表示释放事件，只会在释放时调用一次，Object为绑定事件的对象，ReleasedFunc为释放事件的函数，Action.InputTag为ReleasedFunc的参数
				BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag);
			}

			if (HeldFunc)
			{
				// 绑定按住事件，Triggered表示持续触发事件，按下期间每帧都会调用HeldFunc，Object为绑定事件的对象，HeldFunc为按住事件的函数，Action.InputTag为HeldFunc的参数
				BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, HeldFunc, Action.InputTag);
			}
		}
	}

}
```

