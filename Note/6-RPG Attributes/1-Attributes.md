# 从数据表初始化属性

我们此前的对属性值的初始化是在构造函数里完成的，但是，这显然不是首选，也不是业界的规范。这里我介绍另一种初始化方式，从数据表初始化

```c++
/*
 * RPG 主属性（Primary Attributes）
 */
UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Strength, Category = "Primary Attributes")
FGameplayAttributeData Strength;	// 力量
ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Strength)	// 生成属性的Getter函数，属性的Setter函数，属性的初始化函数

UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Agility, Category = "Primary Attributes")
FGameplayAttributeData Agility;		// 敏捷
ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Agility)	// 生成属性的Getter函数，属性的Setter函数，属性的初始化函数

UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Intelligence, Category = "Primary Attributes")
FGameplayAttributeData Intelligence;	// 智力
ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Intelligence)	// 生成属性的Getter函数，属性的Setter函数，属性的初始化函数

UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Vigor, Category = "Primary Attributes")
FGameplayAttributeData Vigor;		// 活力
ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Vigor)	// 生成属性的Getter函数，属性的Setter函数，属性的初始化函数

UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Resilience, Category = "Primary Attributes")
FGameplayAttributeData Resilience;	// 韧性
ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Resilience)	// 生成属性的Getter函数，属性的Setter函数，属性的初始化函数
```

这是我们数据集里的属性成员，为了初始化他们，我们用表格方式，需要先创建一个对应的数据表，表格行类型选择 AttributeMetaData

![image-20240409102929220](.\image-20240409102929220.png) 

![image-20240409103819959](.\image-20240409103819959.png)

然后将DT应用到蓝图上，这样也能成功初始化属性值

![image-20240409104012277](.\image-20240409104012277.png)



# 使用GameEffect初始化属性

在游戏开始使用GameEffect来初始化属性，应该是最规范的做法了。所以，我们在角色基类里，新增一个初始化Attributes的方法，同时，添加一个默认游戏效果类成员，用来初始化属性用

```c++

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryGameplayEffectClass;		// 默认主要游戏效果类

	void InitializePrimaryAbilities() const;		// 初始化主要能力

```



```c++
void AAuraCharacterBase::InitializePrimaryAbilities() const
{
	checkf(IsValid(GetAbilitySystemComponent()), TEXT("AbilitySystemComponent is nullptr!"));	// 检查AbilitySystemComponent是否为空
	checkf(DefaultPrimaryGameplayEffectClass, TEXT("DefaultPrimaryGameplayEffectClass is nullptr!"));	// 检查DefaultPrimaryGameplayEffectClass是否为空

	// 1：获取上下文
	const FGameplayEffectContextHandle EffectContext = GetAbilitySystemComponent()->MakeEffectContext();
	// 2：创建效果
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(DefaultPrimaryGameplayEffectClass, 1.f, EffectContext);
	// 3：应用效果到目标
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}
```

然后在角色的初始化代码中调用它

```c++
void AAuraCharacter::InitAbilityActorInfo()
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuraPlayerState, this);	// 初始化技能系统组件
	Cast<UAuraAbilitySystemComponent>(AuraPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();	// 设置技能Actor信息
	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();	// 获取技能系统组件
	AttributeSet = AuraPlayerState->GetAttributeSet();	// 获取属性集

	// 很多人纠结到底在什么情况下应该断言PlayerController*，什么时候只需要判断指针是否为空？
	// 因为我们这个是一个多人游戏，而控制器在客户端只有自己角色的控制器，其他角色的控制器是空的，服务端有所有角色的控制器。
	// 所以控制器这个指针是会存在空的情况的，这是正常的，说明这个控制器不是自己的控制器。
	// 所以这种情况下我们只需要判断指针是否为空就可以了，不需要断言。
	if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(AuraPlayerController->GetHUD()))
		{
			AuraHUD->InitOverlay(AuraPlayerController, AuraPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}

	InitializePrimaryAbilities();	// 初始化主要能力

}

```

做好这一切后，我们下一步就是去编辑器中，提供一个GameplayEffect，用来初始化Attributes

![image-20240409111125266](.\image-20240409111125266.png)

唯一需要注意的一点是，我们这里对属性的操作方式，选择覆盖，因为我们是初始化属性，这一点切记。

然后回到角色蓝图，将该GE赋值给角色就行

![image-20240409111349753](.\image-20240409111349753.png)



# 属性修饰符

到目前为止，我们所有的游戏效果都可以使用功能缩放浮点数作为其修改器来改变属性值的大小，但是，实际上在GE中，还有更多更复杂的修改器使用功场景。

![image-20240409112911590](.\image-20240409112911590.png)

在虚幻引擎 5 (UE5) 中，GameplayEffect 的 Modifier Magnitude 设置项用于定义效果的强度或大小。这个设置项位于Gameplay Effect的Modifiers部分，用于控制效果对目标属性的影响程度。以下是对各个配置项的详细解释和用法：

1. **Magnitude Calculation Type**: 这个选项决定了Modifier Magnitude的计算方式。有以下几种选择：
   - **Attribute Based**: Modifier Magnitude的值基于目标属性的值。例如，你可以设置一个效果为“增加目标属性的10%”。
   - **Fixed Value**: Modifier Magnitude是一个固定的数值，不会随目标属性的变化而变化。例如，你可以设置一个效果为“增加目标属性10点”。
   - **Randomized Fixed Value**: Modifier Magnitude是一个随机的固定数值，可以在一个范围内进行随机取值。例如，你可以设置一个效果为“增加目标属性10到20点之间的一个随机值”。
   - **Randomized Attribute Based**: Modifier Magnitude基于目标属性的随机值，可以在一个范围内进行随机取值。例如，你可以设置一个效果为“增加目标属性的10%到20%之间的一个随机值”。

2. **Coefficient**: 这是一个乘数，用于放大或缩小效果的强度。例如，如果你设置Coefficient为2.0，那么效果的强度将会翻倍。

3. **Pre Multiply Additive Value**: 这是一个在乘以系数之前添加的值。例如，如果你设置Pre Multiply Additive Value为5.0，那么效果的强度将会增加5.0。

4. **Post Multiply Additive Value**: 这是一个在乘以系数之后添加的值。例如，如果你设置Post Multiply Additive Value为5.0，那么效果的强度将会增加5.0。

5. **Backing Attribute**: 这个设置用于记录效果的原始属性值。当你想要恢复目标属性到原来的状态时，可以使用这个设置。例如，如果你设置Backing Attribute为“Health”，那么在效果结束后，目标的健康值将会恢复到效果开始前的值。

6. **Attribute to Capture**: 这个设置用于捕获目标属性的值。例如，如果你设置Attribute to Capture为“Health”，那么在效果期间，每次目标的健康值发生变化时，都会被记录下来。

7. **Snapshot**: 这个设置用于创建一个目标属性的快照。例如，如果你设置Snapshot为“Health”，那么在效果开始时，目标的健康值会被记录下来，作为恢复的目标值。

8. **Attribute Curve**: 这个设置用于定义效果随时间变化的曲线。例如，你可以设置一个曲线，使得效果在开始时较强，然后逐渐减弱。或者，你可以设置一个曲线，使得效果在开始时较弱，然后逐渐增强。

9. **Source Tags Filter**和**Target Tags Filter**: 这两个设置用于限制效果作用的对象。你可以设置一些标签，只有满足这些标签的对象才会受到效果的影响。

10. **Source Tags**和**Target Tags**: 这两个设置用于指定效果作用的对象。你可以设置一些标签，只有带有这些标签的对象才会受到效果的影响。

通过灵活地组合这些设置项，你可以创建出各种复杂的效果，如增加或减少目标属性、根据目标属性的值来调整效果的强度、以及让效果随着时间变化等等。



## Modifier Order of Operations（修饰符运算顺序）

在虚幻引擎 5 (UE5) 的Gameplay Effects系统中，当一个属性有多个修饰符时，它们的运算顺序取决于Modifier Operation设置。Modifier Operation设置决定了如何处理多个修饰符对同一属性的影响。以下是一些常见的Modifier Operation设置及其含义：

1. **Add**: 将每个修饰符的效果相加。例如，如果有两个修饰符分别增加目标属性10点和20点，那么最终效果是增加30点。

2. **Subtract**: 从每个修饰符的效果中减去。例如，如果有两个修饰符分别减少目标属性10点和20点，那么最终效果是减少30点。

3. **Multiply**: 将每个修饰符的效果相乘。例如，如果有两个修饰符分别增加目标属性10%和20%，那么最终效果是增加20%。

4. **Divide**: 用每个修饰符的效果除以另一个。例如，如果有两个修饰符分别增加目标属性10%和20%，那么最终效果是增加10%。

5. **Min**: 取每个修饰符效果中的最小值。例如，如果有两个修饰符分别增加目标属性10%和20%，那么最终效果是增加10%。

6. **Max**: 取每个修饰符效果中的最大值。例如，如果有两个修饰符分别增加目标属性10%和20%，那么最终效果是增加20%。

7. **Average**: 取每个修饰符效果的平均值。例如，如果有两个修饰符分别增加目标属性10%和20%，那么最终效果是增加15%。

8. **Weighted Sum**: 根据每个修饰符的权重进行加权求和。例如，如果有两个修饰符分别增加目标属性10%和20%，并且第一个修饰符的权重为0.5，第二个修饰符的权重为0.5，那么最终效果是增加15%。

9. **Weighted Average**: 根据每个修饰符的权重进行加权求平均值。例如，如果有两个修饰符分别增加目标属性10%和20%，并且第一个修饰符的权重为0.5，第二个修饰符的权重为0.5，那么最终效果是增加15%。

在实际应用中，你可以根据具体的需求和效果来选择合适的Modifier Operation。例如，如果你想实现一个效果，使得多个修饰符的效果相加，那么可以选择Add操作；如果你想实现一个效果，使得多个修饰符的效果相乘，那么可以选择Multiply操作。

![image-20240409114637075](.\image-20240409114637075.png)

在有多个修饰符的情况下，Attribute-Based Modifier Magnitude的工作流程如下：

1. 创建Gameplay Effect：首先，需要创建一个Gameplay Effect，其中包含一个或多个Attribute-Based Modifier Magnitudes。每个Modifier Magnitude都与一个目标属性相关联，并且有一个操作符（例如Add、Subtract、Multiply等）来确定如何将属性值转换为效果的强度或大小。

2. 应用Gameplay Effect：当Gameplay Effect被应用到一个目标对象时，系统会检查每个Attribute-Based Modifier Magnitude，并根据其操作符和目标属性的当前值来计算效果的强度或大小。

3. 更新效果：如果目标属性的值发生改变，例如，由于某个技能或装备的效果，那么Gameplay Effect的强度或大小也会相应地改变，以反映目标属性的新值。

4. 结束效果：当Gameplay Effect的持续时间结束时，系统会移除所有应用到目标对象上的效果，包括基于属性的Modifier Magnitudes。

在图片中，我们可以看到一个例子来说明这个过程：

1. 第一个Modifier Magnitude与目标属性"Health"相关联，操作符为"Add"，对应的Backed Attribute为"Vigor"。当目标对象的"Vigor"属性值为9时，这个Modifier Magnitude的强度为9。

2. 第二个Modifier Magnitude与目标属性"Health"相关联，操作符为"Add"，对应的Backed Attribute为"Strength"。当目标对象的"Strength"属性值为10时，这个Modifier Magnitude的强度为10。

3. 第三个Modifier Magnitude与目标属性"Health"相关联，操作符为"Add"，对应的Backed Attribute为"Resilience"。当目标对象的"Resilience"属性值为12时，这个Modifier Magnitude的强度为12。

将这三个Modifier Magnitudes的强度相加，得到的结果就是目标对象的"Health"属性值。在这个例子中，结果为41。

![image-20240409115715383](.\image-20240409115715383.png)

这里再举一个例子：

在图片中，我们可以看到这样一个过程：

1. 第一个Modifier Magnitude与目标属性"Health"相关联，操作符为"Add"，对应的Backed Attribute为"Vigor"。当目标对象的"Vigor"属性值为9时，这个Modifier Magnitude的强度为9。
2. 第二个Modifier Magnitude与目标属性"Health"相关联，操作符为"Multiply"，对应的Backed Attribute为"Strength"。当目标对象的"Strength"属性值为10时，这个Modifier Magnitude的强度为10。
3. 第三个Modifier Magnitude与目标属性"Health"相关联，操作符为"Divide"，对应的Backed Attribute为"Resilience"。当目标对象的"Resilience"属性值为12时，这个Modifier Magnitude的强度为12。

将这三个Modifier Magnitudes的强度相乘，得到的结果就是目标对象的"Health"属性值。在这个例子中，结果为15.83。



## Modifier Coefficients

![image-20240409120337637](.\image-20240409120337637.png)

在Gameplay Effect中，红框中的"系数"、"前置乘法累加值"和"后置乘法累加值"是用于计算效果强度的参数。

1. 系数（Coefficient）：这是一个乘数，用于放大或缩小效果的强度。例如，如果系数设置为2.0，那么效果的强度将会被乘以2.0。

2. 前置乘法累加值（Pre Multiply Additive Value）：这是一个加法项，用于在乘法之前增加效果的强度。例如，如果前置乘法累加值设置为5.0，那么效果的强度将会在乘法之前增加5.0。

3. 后置乘法累加值（Post Multiply Additive Value）：这是一个加法项，用于在乘法之后增加效果的强度。例如，如果后置乘法累加值设置为5.0，那么效果的强度将会在乘法之后增加5.0。

通过组合这些参数，可以精确地控制效果的强度。例如，如果系数设置为2.0，前置乘法累加值设置为5.0，后置乘法累加值设置为10.0，那么效果的强度将会被乘以2.0，然后在乘法之前增加5.0，在乘法之后增加10.0。

![image-20240409120810657](.\image-20240409120810657.png)

在这张图片中，展示了三个不同的修饰符，它们都是基于属性的修改器。每个修饰符都有一个系数（Coefficient），它是用来调整属性值的倍数。下面是对每个修饰符中系数的使用过程进行详细的分析：

1. 第一个修饰符：

   - 属性：Health
   - 操作符：Add
   - 系数：0.1
   - 前置乘法累加值：3
   - 后置乘法累加值：1

   对于这个修饰符，系数0.1表示将属性值乘以0.1，**注意这个系数是要在前置计算结果出来以后，再进行系数计算的。**

   （9+3）*0.1 + 1 = 2.2

2. 第二个修饰符：

   - 属性：Health
   - 操作符：Multiply
   - 系数：0.5
   - 前置乘法累加值：6
   - 后置乘法累加值：11

   （10+6）*0.5 + 11 = 19

3. 第三个修饰符：

   - 属性：Health
   - 操作符：Divide
   - 系数：2.1
   - 前置乘法累加值：0.5
   - 后置乘法累加值：5.7

   （12+0.5）*2.1 + 5.7 = 31.95

需要注意的是，这些系数、前置乘法累加值和后置乘法累加值都是用来调整属性值的。它们可以根据游戏设计的需求进行设置，以实现特定的效果。例如，如果希望增加某个属性的值，可以增大系数；如果希望减少某个属性的值，可以减小系数。

