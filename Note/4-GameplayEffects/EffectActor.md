# Apply Gameplay Effect （游戏效果应用于目标能力系统组件）

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



# Periodic Gameplay Effects （周期性游戏效果）



![image-20240407121156320](.\image-20240407121156320.png)

在虚幻引擎 5 (UE5) 中，Gameplay Effects 系统允许开发者创建和应用各种游戏效果，包括但不限于属性修改、状态变化等。这些效果可以通过 Gameplay Effects 规格（Spec）来定义和应用。在 UE5 中，Gameplay Effects 支持不同类型的持续时间，包括瞬时、持续时间和周期性效果。下面是对这些持续时间类型的解释：

1. **瞬时效果**：
   - 瞬时效果是指立即生效并立即消失的效果。它们不会对目标的属性值产生永久性的改变，而是立即改变当前值。一旦效果被移除，当前值将恢复到原始状态。
   - 瞬时效果通常用于一次性事件，如造成伤害、治疗、施加增益或减益等。

2. **持续时间效果**：
   - 持续时间效果是指具有明确持续时间的效果。它们可以改变目标的当前值，并在持续时间内保持这种改变。
   - 当效果到期时，当前值将恢复到原始状态。
   - 持续时间效果可以用于持续性的增益或减益，如持续伤害、持续治疗、持续的属性增强等。

3. **无限持续效果**：
   - 无限持续效果是指没有明确持续时间的效果。它们可以改变目标的当前值，并一直保持这种改变，直到被移除为止。
   - 只有当效果被移除时，当前值才会恢复到原始状态。
   - 无限持续效果通常用于永久性的增益或减益，如永久属性增强、永久的负面状态等。

4. **周期性效果**：
   - 周期性效果是指按照一定周期重复生效的效果。它们可以改变目标的当前值，并在每次周期性触发时保持这种改变。
   - 周期性效果通常用于持续性的属性增强或削弱，如每秒回复生命值、每秒受到持续伤害等。

在 UE5 中，开发者可以使用这些不同的持续时间类型来创建和应用各种复杂的 Gameplay Effects。通过组合不同的持续时间类型和属性修改，开发者可以实现各种有趣的游戏效果，从而丰富游戏体验。



现在，我们结合这个周期性效果来实现一个真正意义上的持续回血

![image-20240407122308332](.\image-20240407122308332.png)

还是那个回血水晶GE，

- 1：依旧选择效果类型为持续，（因为我们需要控制血量的回血效果的时间）
- 2：打开周期属性（Period），然后设置他的周期时间间隔（这里我设置为1s），
- 3：然后注意看3这里的设置，如果打开这个，表示立即开始第一次周期触发，如果不开启，表示，假如我触发了这个回血，但是周期是从1s后开始算，所以效果会在1s后才开始。这里我们选择开始，表示立即开始周期
- 4：我们将属性改为血量，然后每次加5.

所以，经过上面这通计算，我们可以得到，这个效果他会在3秒内回复（3*5）15点血量。但是我们从下面的实际效果可以看到，它回复了20点血，这不奇怪，**记得我们开启了这个。他的效果就是：在申请时执行定期效果，如果为真，该效果在应用程序上执行，然后在每个周期内执行，如果为假，在第一个周期之前不会执行**

![image-20240407123310470](.\image-20240407123310470.png)

打开AbilityDEBUG。

![image-20240407122917097](.\image-20240407122917097.png)

![image-20240407123014197](.\image-20240407123014197.png)

到这里，我们其实就能做很多事情了，比如设置这个周期间隔，让它小一点，0.1s一次执行，这样我们就能得到更顺滑的回血效果效果。当然可以这样做，但是我们在做这些的时候，要时刻记住，在GAS中，gameplayEffect是被预测的，并且他们会在每次改变的时候都被GAS记录，所以这不仅仅是一个简单的变量设置，他会涉及很多事情。一个很简单的，就是预测它涉及到网络通信，以及帧数据保留的内存开销，这些会在你缩小这个值的时候让游戏消耗增加，影响游戏性能。

因此，你可以这样快速的实现这种丝滑效果，但是如果想要实现真正意义上的流畅，更推荐的做法是通过Widget的值变化进行差值计算



# Effect Stacking （效果叠加）

## Aggregate by Source （按源聚合）

![image-20240407133704175](.\image-20240407133704175.png)



"Aggregate by Source （按源聚合）" 是一种叠加类型，它决定了多个相同效果如何在目标上累积。这种叠加类型通常用于处理来自不同来源的效果，每个来源可能对目标施加相同类型但不同数值的效果。

在 "Aggregate by Source" 的情况下，来自不同来源的效果会被累加在一起，形成一个总的效果。例如，如果两个不同的角色分别对目标施加了一个增加攻击力的效果，那么这两个效果将会被累加起来，形成一个更大的攻击力提升效果。

在图中，我们可以看到一个例子，其中三个不同的角色分别对目标施加了相同类型但不同数值的效果。这些效果被累加在一起，形成一个总的效果。在这个例子中，每个角色都对目标施加了一个增加攻击力的效果，这些效果被累加在一起，形成了一个更大的攻击力提升效果。

需要注意的是，这种叠加类型通常会设置一个叠加限制，以防止效果的数值过大或者调用次数过多。例如，在图中可以看到一个 "Stack Limit Count" 的设置，它限制了同一来源的效果可以叠加的最大数量。在这个例子中，同一来源的效果最多可以叠加两次。这意味着，即使有多个角色对目标施加相同类型的效果，同一来源的效果最多只能叠加两次。而我们的第四个角色它是一个新的源，所以他和之前的伤害不是同一个源，所以他可以继续作用到目标上。



## Aggregate by Target （目标聚合）

![image-20240407134320493](D:\study\Arcane\Note\4-GameplayEffects\image-20240407134320493.png)

按目标聚合（Aggregate by Target）是虚幻引擎 5 中 Gameplay Effects 系统中的叠加类型之一。在这种叠加类型下，来自不同来源的效果会被累加在一起，形成一个总的效果，但是这个总的效果是针对目标的，而不是针对源的。

具体来说，当多个效果作用于同一个目标时，它们会被累加在一起，形成一个总的效果。例如，如果两个不同的角色分别对目标施加了一个增加攻击力的效果，那么这两个效果将会被累加起来，形成一个更大的攻击力提升效果。

与按源聚合（Aggregate by Source）相比，按目标聚合的主要区别在于，它关注的是效果对目标的影响，而不是效果的来源。在按源聚合的情况下，来自同一来源的效果只会被累加一次，而在按目标聚合的情况下，来自不同来源的效果都会被累加在一起。

比如图中，该效果我们按目标聚合，设置了limitCount为2，也就是该目标只能接受2次伤害，所以不管事同一个源的超过2次的伤害，还是其他源的伤害，只要目标超过了2次伤害，其他次数的伤害（效果）都不会应用

总的来说，按目标聚合和按源聚合都是虚幻引擎 5 中 Gameplay Effects 系统中的叠加类型，它们分别适用于不同的情况。选择哪种叠加类型取决于游戏设计的需求，以及效果的来源和目标之间的关系。



如图，我们在场景中放置5个回蓝水晶（一个回复10点）,4个叠一起，一个分开，如果我们按源聚合，然后limit我们设置为2，那么依照上面的说法，我们总的回蓝量应该是30点。记住，因为我们设置了源聚合的使用限制为2，也就是说，我们叠加的这4个水晶，你可以看成它虽然有40点蓝的回复量，但是它只有2次使用机会，一次是10点，所以他经过叠加处理后就只能回复20点蓝。然后旁边的那个因为没有和那4个叠在一起，所以不算一个源，所以他的叠加次数也是单独计算是2，所以它能完整回蓝10点，所以这种模式下，应该回蓝30点。

![image-20240407141410120](.\image-20240407141410120.png)

![image-20240407140600224](.\image-20240407140600224.png)

那如果我们将叠加效果改为按目标聚合呢？

同样的场景下，那他的回蓝就应该只有20点了。



# Infinite Gameplay Effects（永久游戏效果类型）

我们先在代码中添加这个游戏效果类型，现在我们已经有3种游戏类型了。

```c++
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	TSubclassOf<UGameplayEffect> InstanceGameplayEffectClass;	// 瞬时类型的GameplayEffect类

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	TSubclassOf<UGameplayEffect> DurationGameplayEffectClass;	// 持续类型的GameplayEffect类

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	TSubclassOf<UGameplayEffect> InfiniteGameplayEffectClass;	// 无限类型的GameplayEffect类
```

为了更灵活的使用，我们定义一个效果应用策略枚举，它包含3中使用策略（当然，可以随意扩展），然后我们给我们的每个效果类型加一个策略属性，这样我们就能更灵活的将各个游戏效果使用不同的游戏策略来进行扩展了

同样有应用效果的策略，也有移除效果的策略

```c++

// 定义效果应用策略
UENUM(BlueprintType)
enum class EEffectApplicationPolicy
{
	ApplyOnOverlap,		// 在重叠时应用效果
	ApplyOnEndOverlap,	// 在结束重叠时应用效果
	DoNotApply			// 不应用效果
};

// 定义效果移除策略，实际上，效果移除一般只针对永久效果，因为瞬时效果和持续效果会在自身结束时自动移除
UENUM(BlueprintType)
enum class EEffectRemovePolicy
{
	RemoveOnEndOverlap,	// 在结束重叠时移除效果
	DoNotRemove			// 不移除效果
};

UCLASS()
class ARCANE_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AAuraEffectActor();

protected:
	virtual void BeginPlay() override;

	/*
	 * 游戏中的效果分为三种：瞬时效果、持续效果和永久效果
	 */

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	TSubclassOf<UGameplayEffect> InstanceGameplayEffectClass;	// 瞬时类型的GameplayEffect类

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	TSubclassOf<UGameplayEffect> DurationGameplayEffectClass;	// 持续类型的GameplayEffect类

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	TSubclassOf<UGameplayEffect> InfiniteGameplayEffectClass;	// 无限类型的GameplayEffect类

	/*
	 * 3种游戏效果的应用策略
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	EEffectApplicationPolicy InstanceEffectApplicationPolicy;	// 瞬时效果应用策略

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	EEffectApplicationPolicy DurationEffectApplicationPolicy;	// 持续效果应用策略

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	EEffectApplicationPolicy InfiniteEffectApplicationPolicy;	// (永久类型)无限效果应用策略

	/*
	 * 游戏效果的移除策略 （只针对永久效果，其他效果如果想通过策略控制移除，也可自行添加）
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	EEffectRemovePolicy InfiniteEffectRemovePolicy;		// (永久类型)无限效果移除策略

	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass);	// 将效果应用到目标，传入目标和GameplayEffect类

	

private:
};

```



## Infinite Effect Application and Removed

我们如果想要移除一个游戏效果，首先，我们应该拿到他的效果句柄，一但一个游戏效果成功应用，那么他会有一个被激活的句柄，我们需要将这个句柄和游戏能力组件关联起来

```c++
#include "GameplayEffectTypes.h"

	/*
	 * 游戏效果和能力系统组件的映射Map，该Map后续用于移除效果
	 */
	TMap<FActiveGameplayEffectHandle, UAbilitySystemComponent*> ActiveGameplayEffectsMap;	// 活动效果句柄 用于存储游戏效果和能力系统组件的映射

```

那么我们应该在什么时候去保存这个信息呢？ApplyEffectToTarget 显然是极好的。

```c++
void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	// 获取目标的能力系统组件
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (TargetASC)
	{
		checkf(GameplayEffectClass, TEXT("GameplayEffectClass is nullptr!"));	// 检查GameplayEffectClass是否为空

		// 创建效果上下文，用于创建效果规格。
		// 什么是效果上下文呢？效果上下文是一个结构体，用于存储效果的来源，目标，施法者等信息。在创建效果规格时，我们需要传入效果上下文。
		FGameplayEffectContextHandle EffectContext = TargetASC->MakeEffectContext();
		EffectContext.AddSourceObject(this);	// 添加源对象，表示这个效果是由谁发出的。

		// 创建效果规格，用于应用效果。参数分别是效果类，等级，效果上下文。
		FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, 1.f, EffectContext);
		// 如果效果规格有效，就应用效果规格到自己。
		if (EffectSpecHandle.IsValid())
		{
			// 应用效果规格到自己。
			FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

			// 获取效果的持续时间类型，判断效果是否是无限持续的。
			const bool bIsInfinite = EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;

			// 只有效果是无限持续的，并且我们希望在结束重叠时移除效果时，我们才将效果句柄和能力系统组件存储到映射表中。
			if (bIsInfinite && InfiniteEffectRemovePolicy == EEffectRemovePolicy::RemoveOnEndOverlap)
			{
				// 如果效果是无限持续的，就将效果句柄和能力系统组件存储到映射表中。
				ActiveGameplayEffectsMap.Add(ActiveEffectHandle, TargetASC);
			}
		}
	}

}
```

这里我们只保存永久无限类型的gamepalyEffect的效果句柄，因为其他类型的他们有自己的生命周期，能自行移除效果。我们做这个事情本来就是专门处理永久效果的移除的。

然后，我们将移除逻辑在策略函数里进行实现

```c++
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
		// 移除效果
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);	// 获取目标的能力系统组件
		if (IsValid(TargetASC))
		{
			TArray<FActiveGameplayEffectHandle> ActiveEffectsToRemove;	// 用于存储要移除的效果句柄
			for (const TTuple<FActiveGameplayEffectHandle, UAbilitySystemComponent*>& Elem : ActiveGameplayEffectsMap)
			{
				if (Elem.Value == TargetASC)	// 如果映射表中的能力系统组件和目标的能力系统组件相同，表示这个效果是目标的
				{
					TargetASC->RemoveActiveGameplayEffect(Elem.Key);	// 移除效果
					ActiveEffectsToRemove.Add(Elem.Key);	// 将要移除的效果句柄存储到数组中
				}
			}
			for (const FActiveGameplayEffectHandle& ActiveEffectHandle : ActiveEffectsToRemove)
			{
				ActiveGameplayEffectsMap.Remove(ActiveEffectHandle);	// 从映射表中移除效果句柄
			}
		}
	}
}
```

然后回到蓝图，我们制作一个进入后持续掉血的火焰区域。离开这个区域，结束掉血

![image-20240407195108474](.\image-20240407195108474.png)

这个图apply哪里选错了，应该选择重叠开始的时候应用效果，注意一下

当然，上面这种实现，移除效果它是直接将这一类效果都移除了，但是比如，我们在一个火堆堆叠环境中，它的中心区域会收到3个火场的伤害，此时如果玩家从中心区域走出来，走到只受一个火场灼烧的区域，因为我们上面的移除函数，会导致玩家的灼烧效果都失效，都被移除，这显然不是我们想看到的，所以 TargetASC->RemoveActiveGameplayEffect(Elem.Key);这个函数它还提供了一个额外参数，我们将其设置为1，表示每次移除一个（默认不填是-1，表示全部移除）

```c++
if (Elem.Value == TargetASC)	// 如果映射表中的能力系统组件和目标的能力系统组件相同，表示这个效果是目标的
{
	TargetASC->RemoveActiveGameplayEffect(Elem.Key, 1);	// 移除效果
	ActiveEffectsToRemove.Add(Elem.Key);	// 将要移除的效果句柄存储到数组中
}
```

![image-20240407201605146](.\image-20240407201605146.png)



# PreAttributeChange （临时限制属性数值范围）

我们之前所做的所有的效果，他们在改变属性集里的数值的时候，是没有边界限制的，血量可以低于0，可以高于100等等等等，现在，我们需要限制这些边界值。

只需要在UAuraAttributeSet类中 重写PreAttributeChange函数，用于属性改变前的处理

```c++
void UAuraAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	// 限制生命值和最大生命值的范围
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	else if (Attribute == GetMaxHealthAttribute())
	{
		NewValue = FMath::Max(NewValue, 0.f);
	}
	else if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
	}
	else if (Attribute == GetMaxManaAttribute())
	{
		NewValue = FMath::Max(NewValue, 0.f);
	}
}
```

![image-20240407203849780](D:\study\Arcane\Note\4-GameplayEffects\image-20240407203849780.png)

PreAttributeChange函数是一个在虚幻引擎 5 中用于处理属性变化的函数。它在属性值发生改变之前被调用，用于计算属性的新值。以下是PreAttributeChange函数的一些主要特点：

1. 改变CurrentValue：PreAttributeChange函数会在属性值发生改变之前被调用，用于计算属性的新值。这个新值被称为CurrentValue，它是属性当前值的临时副本。

2. 触发器：PreAttributeChange函数是由属性访问器、游戏效果或游戏效果执行器等引起的属性值改变所触发的。

3. 不永久改变修饰符：PreAttributeChange函数不会永久改变修饰符，它只是临时地改变了查询修饰符时返回的值。

4. 后续操作重新计算Current Value：在PreAttributeChange函数之后的操作会重新计算属性的Current Value，这可能会导致需要再次进行限制（clamp）。

5. 与PostGameplayEffectExecute的关系：PreAttributeChange函数在PostGameplayEffectExecute函数之前被调用。PostGameplayEffectExecute函数是在游戏效果执行后调用的，用于更新属性值。

总之，PreAttributeChange函数提供了一种在属性值改变之前进行临时计算和处理的方法，这对于游戏中的属性系统来说是非常重要的。通过使用PreAttributeChange函数，游戏开发者可以在属性值改变之前进行一些必要的计算和检查，确保属性值的变化符合游戏规则和逻辑。
