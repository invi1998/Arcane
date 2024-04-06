# Apply Gameplay Effect （游戏效果应用于目标negligible系统组件）

这里为了理解Gameplay Effect到底是干嘛，我们对比前后两版代码来看

```c++
void AAuraEffectActor::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// TODO: 在GamePlayEffect中改变属性，而不是直接在这里改变
	if (const IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(OtherActor))
	{
		const UAuraAttributeSet* AuraAttributeSet = Cast<UAuraAttributeSet>(AbilitySystemInterface->GetAbilitySystemComponent()->GetAttributeSet(UAuraAttributeSet::StaticClass()));

		// 去const
		UAuraAttributeSet* AuraAS = const_cast<UAuraAttributeSet*>(AuraAttributeSet);

		AuraAS->SetHealth(AuraAttributeSet->GetHealth() + 10.f);
	}
}
```

上面这段代码是之前EffectActor里的代码，这个类原先提供了一个静态网格体，一个碰撞球体，我们希望实现的效果就是，角色碰撞到球体后，有一个加血效果。所以，上面这段代码，是在没有使用Gameplay Effect的情况下，硬编码改变GAS的属性集里的值。

现在，我们希望通过Game Effect 来修改属性集。那我们的做法就是下面这种。我们在类中新写一个ApplyEffectToTarget函数，顾名思义，该函数的作用就是将效果应用到目标角色上。

```c++
	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* Target, TSubclassOf<UGameplayEffect> GameplayEffectClass);	// 将效果应用到目标，传入目标和GameplayEffect类

```

CPP

```c++
void AAuraEffectActor::ApplyEffectToTarget(AActor* Target, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	//IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(Target);	// 将目标转换为IAbilitySystemInterface接口

	//// 如果目标实现了IAbilitySystemInterface接口，表示目标是一个拥有能力系统的角色

	//if (AbilitySystemInterface)
	//{
	//	UAbilitySystemComponent* TargetAbilitySystemComponent = AbilitySystemInterface->GetAbilitySystemComponent();	// 获取目标的能力系统组件

	//	if (TargetAbilitySystemComponent)
	//	{
	//		FGameplayEffectContextHandle EffectContext = TargetAbilitySystemComponent->MakeEffectContext();	// 创建效果上下文
	//		EffectContext.AddSourceObject(this);	// 添加源对象

	//		FGameplayEffectSpecHandle NewHandle = TargetAbilitySystemComponent->MakeOutgoingSpec(GameplayEffectClass, 1.f, EffectContext);	// 创建新的效果规格
	//		if (NewHandle.IsValid())
	//		{
	//			TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*NewHandle.Data.Get());	// 将效果规格应用到自己
	//		}
	//	}
	//}

	// 当然，不是所有的目标都会实现IAbilitySystemInterface接口，所以我们可以直接获取目标的能力系统组件
	// 我们可以调用AbilitySystemBlueprintLibrary::GetAbilitySystemComponent函数来获取目标的能力系统组件
	// ，这个函数会自动判断目标是否实现了IAbilitySystemInterface接口，如果实现了，就会返回目标的能力系统组件，
	// 如果没有实现，他会查找目标的所有者是否实现了IAbilitySystemInterface接口，如果实现了，就会返回所有者的能力系统组件

	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
	if (TargetASC)
	{
		// 创建效果上下文，用于创建效果规格。
		// 什么是效果上下文呢？效果上下文是一个结构体，用于存储效果的来源，目标，施法者等信息。在创建效果规格时，我们需要传入效果上下文。
		FGameplayEffectContextHandle EffectContext = TargetASC->MakeEffectContext();
		EffectContext.AddSourceObject(this);	// 添加源对象

		FGameplayEffectSpecHandle NewHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, 1.f, EffectContext);
		if (NewHandle.IsValid())
		{
			TargetASC->ApplyGameplayEffectSpecToSelf(*NewHandle.Data.Get());
		}
	}

}

```

这段代码逻辑的主要目的是在 Unreal Engine 游戏中应用一个 Gameplay Effect 规格（Spec）到一个目标 Actor 上。以下是代码逻辑的简要概述：

1. 首先，代码检查目标 Actor 是否实现了 `IAbilitySystemInterface` 接口。如果实现了，代码将尝试获取目标 Actor 的 Ability System Component 并创建一个效果上下文（Effect Context）。

2. 如果目标 Actor 实现了 `IAbilitySystemInterface` 接口，代码将调用 `MakeEffectContext()` 函数创建一个效果上下文，并将源对象（通常是当前 Actor）添加到上下文中。

3. 然后，代码调用 `MakeOutgoingSpec()` 函数创建一个 Gameplay Effect 规格（Spec）。这个规格包含了要应用到目标 Actor 的效果类（Gameplay Effect Class）以及效果等级（Level）。

4. 如果规格有效，代码将调用 `ApplyGameplayEffectSpecToSelf()` 函数将规格应用到目标 Actor 自身。

5. 如果目标 Actor 没有实现 `IAbilitySystemInterface` 接口，代码会尝试直接获取目标 Actor 的 Ability System Component。如果找到了，代码会重复步骤 2 到 4。

6. 最后，代码会检查 Gameplay Effect 类是否为空，以及规格是否有效。如果规格无效，代码会抛出错误信息。

总之，这段代码的主要目的是在 Unreal Engine 游戏中应用一个 Gameplay Effect 规格到目标 Actor 上，以实现特定的游戏效果。



# Instant Gameplay Effects

我们在UE中创建一个GE_PotionHeal（血点）基于最基础的GameplayEffect，用来实现碰撞加血逻辑。

![image-20240407023108265](.\image-20240407023108265.png)

然后我们在设置里指定好属性集，因为是加血逻辑，所以这里关联的是属性集里的Health，然后指定数据操作运算（这里是做加法操作，选择Add），然后指定操作数值，比如一次加多少血，这里定为25.5。然后保存，这样，一个最简单的GameEffect蓝图就设置好了。

然后我们基于我们上面的 AAuraEffectActor 类创建蓝图，该蓝图用于实例化生成一个血点，比如实例化一些血球（药瓶）的静态网格体啊之类的。

![image-20240407023545828](.\image-20240407023545828.png)

然后编辑该蓝图的事件。我们希望角色碰撞到（和碰撞球体重合）触发回血效果，然后销毁血包。所以在蓝图里我们在overlap节点下，执行我们的将我们的GameplayEffect应用到角色上，targetActor就是应用目标，对于碰撞球来说，就是配碰到了我（角色碰到了血瓶）。然后我们将我们准备好的GE_PointHeal作为游戏效果传给ApplyEffectToTarget函数，这样就实现了回血逻辑。ApplyEffectToTarget函数就是上面我们暴露给蓝图的。

![image-20240407023657270](.\image-20240407023657270.png)

由此我们就打通了GameplayEffect的一个最基础的工作流程。
