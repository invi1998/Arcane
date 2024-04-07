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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	TSubclassOf<UGameplayEffect> InstanceGameplayEffectClass;	// 瞬时类型的GameplayEffect类


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

由此我们就打通了GameplayEffect的一个最基础的工作流程。（这个紫色节点就是我们在C++代码里暴露给蓝图的GamplayEffect类型），他需要我再蓝图中设置他的游戏效果类型（GameEffect），它是一个瞬时类型的游戏效果

```c++
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	TSubclassOf<UGameplayEffect> InstanceGameplayEffectClass;	// 瞬时类型的GameplayEffect类
```

当然，我们也可以直接在蓝图里直接拖拽一个 GameEffec 蓝图来使用。



## 纯蓝图

当然，有了上面的基础，我们想使用纯蓝图实现gameEffect也是很容易的

![image-20240407112037333](.\image-20240407112037333.png)

同样是碰撞事件触发，然后获取AbilitySystemCompenent，然后AddSourceObject添加源对象。然后创建上下文，然后创建一个 Gameplay Effect 规格（Spec）。这个规格包含了要应用到目标 Actor 的效果类（Gameplay Effect Class）以及效果等级（Level）。最后调用`ApplyGameplayEffectSpecToSelf()` 函数将规格应用到目标 Actor 自身。



一个有意思的事情是，我们在蓝图中是没办法使用 AddSourceObject 节点去设置源对象的，因为我们回到代码，可以看到该函数不是一个蓝图可调函数（它其实是一个静态函数，并不是类的成员函数），但是我们可以调用  GetSourceObject 节点，这个函数在代码里其实也是一个非蓝图可调函数，但是我们却可以在这里调用。为啥？因为这个节点不是代码里的那个函数（不是Ability结构体上的函数）他是Ability System蓝图库上的函数。这些静态库是向蓝图公开某些只能通过C++代码完成某些事情的好方法。

![image-20240407110859495](.\image-20240407110859495.png)

后续我将制作自己的蓝图库，可以用来暴露特定的功能给蓝图端使用。



# Duration Gamepaly Effect （持续游戏效果）

这里我们介绍第二种游戏效果类型，上面的瞬时游戏效果（加血，是一瞬间就加25.5），现在，我们做一个回血水晶，它可以有一个持续回血的效果。

![image-20240407112924173](.\image-20240407112924173.png)

![image-20240407113349951](.\image-20240407113349951.png)

如图，我们做点疯狂的事情，我们让这个游戏效果改变我们的最大生命值。首先选择效果持续类型，选择持续，然后设置持续时间，然后添加modifiers，这里我们选择改变MaxHealth属性，数值计算为加，加100。至此，GE完成。

这里我们需要回到代码，为我们的游戏效果新增一个类型（持续型）

![image-20240407113848644](.\image-20240407113848644.png)

同样蓝图设置

![image-20240407120118805](.\image-20240407120118805.png)

**记住，这里的持续时间是这个效果的持续时间，而不是施加这个效果，需要的时间。换句话说，我们上面这个加最大血量的效果，按照我们的设置，它只能持续2s。2s后又恢复到原来的100了。**
