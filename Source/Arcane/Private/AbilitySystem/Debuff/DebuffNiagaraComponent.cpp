// Copyright INVI1998


#include "AbilitySystem/Debuff/DebuffNiagaraComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/KismetSystemLibrary.h"

UDebuffNiagaraComponent::UDebuffNiagaraComponent()
{
	bAutoActivate = false;	// 不自动激活
}

void UDebuffNiagaraComponent::BeginPlay()
{
	Super::BeginPlay();

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetOwner());
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner());

	if (ASC)
	{
		// 注册减益Buff的Tag，当Tag的数量发生变化时，调用DebuffTagChanged函数
		ASC->RegisterGameplayTagEvent(DebuffTag, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UDebuffNiagaraComponent::DebuffTagChanged);
	}
	else if (CombatInterface)
	{
		// 如果没有找到AbilitySystemComponent，那就创建一个委托，当ASC被创建时，调用DebuffTagChanged函数
		// 但是我们不希望DebuffNiagaraComponent依赖于ASC，所以我们需要在CombatInterface中添加一个委托，当ASC被创建时，调用这个委托
		// 然后我们就可以在这里注册DebuffTagChanged函数，去响应减益Buff的Tag以及消费ASC委托
		// 首先我们得确定当前Owner是否实现了CombatInterface，如果实现了，那么我们就可以调用GetOnASCRegisteredDelegate函数，获取ASC注册委托

		// WeakLambda是一个弱引用，当ASC被销毁时，这个委托也会被销毁
		// WeakLambda具有这样的特性，即它可以在不增加引用计数的情况下，保持对某个对象的引用，因此它仍然可以进行垃圾回收，就好像WeakLambda没有引用该对象一样
		// AddWeakLambda：此函数允许你以弱引用的方式添加一个lambda表达式作为委托或事件的监听器。
		// 当使用AddWeakLambda注册事件处理器时，即使触发该事件的对象被销毁，也不会因为lambda内部持有该对象的强引用而导致对象无法被正确清理。
		// 这样的特性正是我们在这里所需要的，因为我们不希望DebuffNiagaraComponent依赖于ASC，所以我们需要一个弱引用的委托
		CombatInterface->GetOnASCRegisteredDelegate().AddWeakLambda(this, [this](UAbilitySystemComponent* InASC)
		{
			InASC->RegisterGameplayTagEvent(DebuffTag, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UDebuffNiagaraComponent::DebuffTagChanged);
		});
	}

	if (CombatInterface)
	{
		// 如果Owner实现了CombatInterface，那么我们就可以调用GetOwner函数，获取Owner
		// 然后我们就可以调用GetOnCharacterDeathDelegate函数，获取角色死亡委托
		// 当角色死亡时，我们就可以停止Niagara组件
		CombatInterface->GetOnCharacterDeathDelegate().AddDynamic(this, &UDebuffNiagaraComponent::OnOwnerDeath);
	}
}

void UDebuffNiagaraComponent::DebuffTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (CallbackTag != DebuffTag)
	{
		return;
	}
	if (IsValid(GetOwner()) && GetOwner()->Implements<UCombatInterface>() && ICombatInterface::Execute_IsDead(GetOwner()))
	{
		Deactivate();
		return;
	}
	if (NewCount > 0 && IsValid(GetOwner()))
	{
		Activate();
	}
	else
	{
		Deactivate();
	}
}

void UDebuffNiagaraComponent::OnOwnerDeath(AActor* DeadActor)
{
	Deactivate();
}
