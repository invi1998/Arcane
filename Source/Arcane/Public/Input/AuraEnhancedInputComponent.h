// Copyright INVI1998

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
