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

void AAuraEffectActor::ApplyEffectToTarget(AActor* Target, TSubclassOf<UGameplayEffect> GameplayEffectClass)
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
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
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
			TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
		}
	}

}




