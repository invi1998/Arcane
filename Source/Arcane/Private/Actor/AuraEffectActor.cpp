// Copyright INVI1998


#include "Actor/AuraEffectActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/AuraAttributeSet.h"

// Sets default values
AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent")));	// ���������, ���������

}

void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
}

void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	//IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(Target);	// ��Ŀ��ת��ΪIAbilitySystemInterface�ӿ�

	//// ���Ŀ��ʵ����IAbilitySystemInterface�ӿڣ���ʾĿ����һ��ӵ������ϵͳ�Ľ�ɫ

	//if (AbilitySystemInterface)
	//{
	//	UAbilitySystemComponent* TargetAbilitySystemComponent = AbilitySystemInterface->GetAbilitySystemComponent();	// ��ȡĿ�������ϵͳ���

	//	if (TargetAbilitySystemComponent)
	//	{
	//		FGameplayEffectContextHandle EffectContext = TargetAbilitySystemComponent->MakeEffectContext();	// ����Ч��������
	//		EffectContext.AddSourceObject(this);	// ���Դ����

	//		FGameplayEffectSpecHandle NewHandle = TargetAbilitySystemComponent->MakeOutgoingSpec(GameplayEffectClass, 1.f, EffectContext);	// �����µ�Ч�����
	//		if (NewHandle.IsValid())
	//		{
	//			TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*NewHandle.Data.Get());	// ��Ч�����Ӧ�õ��Լ�
	//		}
	//	}
	//}

	// ��Ȼ���������е�Ŀ�궼��ʵ��IAbilitySystemInterface�ӿڣ��������ǿ���ֱ�ӻ�ȡĿ�������ϵͳ���
	// ���ǿ��Ե���AbilitySystemBlueprintLibrary::GetAbilitySystemComponent��������ȡĿ�������ϵͳ���
	// ������������Զ��ж�Ŀ���Ƿ�ʵ����IAbilitySystemInterface�ӿڣ����ʵ���ˣ��ͻ᷵��Ŀ�������ϵͳ�����
	// ���û��ʵ�֣��������Ŀ����������Ƿ�ʵ����IAbilitySystemInterface�ӿڣ����ʵ���ˣ��ͻ᷵�������ߵ�����ϵͳ���

	// ��ȡĿ�������ϵͳ���
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (TargetASC)
	{
		checkf(GameplayEffectClass, TEXT("GameplayEffectClass is nullptr!"));	// ���GameplayEffectClass�Ƿ�Ϊ��

		// ����Ч�������ģ����ڴ���Ч�����
		// ʲô��Ч���������أ�Ч����������һ���ṹ�壬���ڴ洢Ч������Դ��Ŀ�꣬ʩ���ߵ���Ϣ���ڴ���Ч�����ʱ��������Ҫ����Ч�������ġ�
		FGameplayEffectContextHandle EffectContext = TargetASC->MakeEffectContext();
		EffectContext.AddSourceObject(this);	// ���Դ���󣬱�ʾ���Ч������˭�����ġ�

		// ����Ч���������Ӧ��Ч���������ֱ���Ч���࣬�ȼ���Ч�������ġ�
		FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, 1.f, EffectContext);
		// ���Ч�������Ч����Ӧ��Ч������Լ���
		if (EffectSpecHandle.IsValid())
		{
			// Ӧ��Ч������Լ���
			FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

			// ��ȡЧ���ĳ���ʱ�����ͣ��ж�Ч���Ƿ������޳����ġ�
			const bool bIsInfinite = EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;

			// ֻ��Ч�������޳����ģ���������ϣ���ڽ����ص�ʱ�Ƴ�Ч��ʱ�����ǲŽ�Ч�����������ϵͳ����洢��ӳ����С�
			if (bIsInfinite && InfiniteEffectRemovePolicy == EEffectRemovePolicy::RemoveOnEndOverlap)
			{
				// ���Ч�������޳����ģ��ͽ�Ч�����������ϵͳ����洢��ӳ����С�
				ActiveGameplayEffectsMap.Add(ActiveEffectHandle, TargetASC);
			}
		}
	}

}

void AAuraEffectActor::OnOverlap(AActor* TargetActor)
{
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}
}

void AAuraEffectActor::OnEndOverlap(AActor* TargetActor)
{
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}
	if (InfiniteEffectRemovePolicy == EEffectRemovePolicy::RemoveOnEndOverlap)
	{
		// �Ƴ�Ч��
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);	// ��ȡĿ�������ϵͳ���
		if (IsValid(TargetASC))
		{
			TArray<FActiveGameplayEffectHandle> ActiveEffectsToRemove;	// ���ڴ洢Ҫ�Ƴ���Ч�����
			for (const TTuple<FActiveGameplayEffectHandle, UAbilitySystemComponent*>& Elem : ActiveGameplayEffectsMap)
			{
				if (Elem.Value == TargetASC)	// ���ӳ����е�����ϵͳ�����Ŀ�������ϵͳ�����ͬ����ʾ���Ч����Ŀ���
				{
					TargetASC->RemoveActiveGameplayEffect(Elem.Key, 1);	// �Ƴ�Ч��
					ActiveEffectsToRemove.Add(Elem.Key);	// ��Ҫ�Ƴ���Ч������洢��������
				}
			}
			for (const FActiveGameplayEffectHandle& ActiveEffectHandle : ActiveEffectsToRemove)
			{
				ActiveGameplayEffectsMap.Remove(ActiveEffectHandle);	// ��ӳ������Ƴ�Ч�����
			}
		}
	}
}




