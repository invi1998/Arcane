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
	// ��ģ�庯�����ڰ󶨰����¼����������¼��󶨵���Ӧ�ĺ����ϣ�ģ������ΪUserClass��PressedFuncType��ReleasedFuncType��HeldFuncType
	// UserClassΪ���¼��Ķ���PressedFuncTypeΪ�����¼��ĺ������ͣ�ReleasedFuncTypeΪ�ͷ��¼��ĺ������ͣ�HeldFuncTypeΪ��ס�¼��ĺ�������
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
		// Ϊ���е����붯���󶨰��£��ͷţ��Ͱ�ס�¼�
		// ΪʲôҪΪ���е����붯������ͬ�İ��£��ͷţ��Ͱ�ס�¼��أ�
		// ��Ϊ������Abilityȥ����GamplayTag������������ǽ����е����붯���󶨵���ͬ�İ��£��ͷţ��Ͱ�ס�¼��ϣ����ǾͿ���������¼��д���GamplayTag
		// Ȼ�����GamplayTag��ʶ����Щ���룬���ݸ�AbilitySystemComponent��Ȼ����ASC�������Լ�����Щ����
		if (Action.InputAction && Action.InputTag.IsValid())
		{
			if (PressedFunc)
			{
				// �󶨰����¼���Started��ʾ�����¼���ֻ���ڿ�ʼ����һ�Σ�ObjectΪ���¼��Ķ���PressedFuncΪ�����¼��ĺ�����Action.InputTagΪPressedFunc�Ĳ���
				BindAction(Action.InputAction, ETriggerEvent::Started, Object, PressedFunc, Action.InputTag);
			}

			if (ReleasedFunc)
			{
				// ���ͷ��¼���Completed��ʾ�ͷ��¼���ֻ�����ͷ�ʱ����һ�Σ�ObjectΪ���¼��Ķ���ReleasedFuncΪ�ͷ��¼��ĺ�����Action.InputTagΪReleasedFunc�Ĳ���
				BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag);
			}

			if (HeldFunc)
			{
				// �󶨰�ס�¼���Triggered��ʾ���������¼��������ڼ�ÿ֡�������HeldFunc��ObjectΪ���¼��Ķ���HeldFuncΪ��ס�¼��ĺ�����Action.InputTagΪHeldFunc�Ĳ���
				BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, HeldFunc, Action.InputTag);
			}
		}
	}

}
