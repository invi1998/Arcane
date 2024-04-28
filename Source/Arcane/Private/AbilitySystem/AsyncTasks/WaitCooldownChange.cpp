// Copyright INVI1998


#include "AbilitySystem/AsyncTasks/WaitCooldownChange.h"

#include "AbilitySystemComponent.h"

UWaitCooldownChange* UWaitCooldownChange::WaitCooldownChange(UAbilitySystemComponent* AbilitySystemComponent,
                                                             const FGameplayTag& InCooldownTag)
{
	UWaitCooldownChange* WaitForCooldownChange = NewObject<UWaitCooldownChange>();
	WaitForCooldownChange->ASC = AbilitySystemComponent;
	WaitForCooldownChange->CooldownTag = InCooldownTag;

	if (!IsValid(AbilitySystemComponent) || !InCooldownTag.IsValid())
	{
		WaitForCooldownChange->EndTask();
		return nullptr;
	}

	// ��ȴ��ʱ������CooldownTag��ʱ���Ƴ���
	// ���ǿ���ͨ������RegiterGameplayTagEvent������CooldownTag�ı仯
	// ע�⣺��Ϊ����һ����̬������û��thisָ�룬����������Ҫʹ�����洴����WaitForCooldownChange������AddUObject
	AbilitySystemComponent->RegisterGameplayTagEvent(InCooldownTag, EGameplayTagEventType::NewOrRemoved).AddUObject(WaitForCooldownChange, &UWaitCooldownChange::CooldownChanged);

	// ��ȴ��ʱ��ʼ���������һ���µĻ��ڳ���ʱ���GameplayEffectʱ��
	// ÿ�����һ���µĻ��ڳ���ʱ���GameplayEffectʱ���ͻ��˺ͷ�����������ô�ί��
	AbilitySystemComponent->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(WaitForCooldownChange, &UWaitCooldownChange::OnActiveEffectAdded);

	return WaitForCooldownChange;
}

void UWaitCooldownChange::EndTask()
{
	// �Ƴ�����
	if (IsValid(ASC))
	{
		ASC->RegisterGameplayTagEvent(CooldownTag, EGameplayTagEventType::NewOrRemoved).RemoveAll(this);
	}

	SetReadyToDestroy();	// ����Ϊ׼�����٣�������ȫ��������ã�������������ɾ���ò���������ȡ������Ϸʵ���е�ע��
	MarkAsGarbage();		// ���Ϊ�������ȴ�GC����
}

void UWaitCooldownChange::CooldownChanged(const FGameplayTag InCooldownTag, int32 NewCount)
{
	if (NewCount == 0)	// �����ȴʱ��Ϊ0����������
	{
		CooldownEnd.Broadcast(0);
	}
}

void UWaitCooldownChange::OnActiveEffectAdded(UAbilitySystemComponent* AbilitySystemComponent,
	const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveHandle)
{
	FGameplayTagContainer AssetTags;
	EffectSpec.GetAllAssetTags(AssetTags);

	FGameplayTagContainer GrantedTags;
	EffectSpec.GetAllGrantedTags(GrantedTags);

	if (AssetTags.HasTagExact(CooldownTag) || GrantedTags.HasTagExact(CooldownTag))
	{
		FGameplayEffectQuery Query = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CooldownTag.GetSingleTagContainer());	// ��ѯ�Ƿ���CooldownTag

		// ΪʲôGetActiveEffectsTimeRemaining���ص���һ��float���飿
		// ��Ϊ�ú�����������ƥ���GameplayEffect��ʣ��ʱ�䣬������ֻ����һ��
		TArray<float> TimeRemainingArray = AbilitySystemComponent->GetActiveEffectsTimeRemaining(Query);	// ��ȡʣ��ʱ��
		// ��ȴ��ʼ
		if (TimeRemainingArray.Num() > 0)
		{
			// ������ȴ����������ֵ��Ϊ���ǵ���ȴʱ��
			float CooldownTime = TimeRemainingArray[0];
			for (int i = 1; i < TimeRemainingArray.Num(); i++)
			{
				CooldownTime = FMath::Max(CooldownTime, TimeRemainingArray[i]);
			}
			CooldownEnd.Broadcast(CooldownTime);
		}
	}
}
