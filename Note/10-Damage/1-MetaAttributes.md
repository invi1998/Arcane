# Meta Attributes

Meta Attributes并不直接代表游戏角色的具体数值属性，如生命值、攻击力等，而是对这些属性进行管理和操作的额外元数据信息。

**Meta Attributes**：
Meta Attributes主要用于描述 Gameplay Attributes的行为特性或者管理规则，它们不存储具体的游戏数据，而是定义了属性如何运作、如何影响其他系统，以及它们与其他游戏机制之间的交互方式。例如，Meta Attributes可以包括以下类型的设置：

1. **Attribute Aggregation Type**：决定多个来源赋予同一属性值时如何聚合，比如是否累加还是取最大值、最小值等。
2. **Attribute Lifetime Policy**：指定属性值如何持久化或何时重置，如每回合结束时清零，或者一直累积直到手动重置。
3. **Display Name & Tooltip**：为属性提供用户界面显示的名称和提示信息。
4. **Clamp Min/Max Values**：限制属性实际数值的上下界。
5. **Scaling Factors**：属性值可能受到某种比例因子的影响，如基于等级或其他属性的变化而变化。

**普通属性（Gameplay Attributes）**：
相比之下，普通属性（Gameplay Attributes）是直接反映游戏角色状态的数据指标，例如：
- 生命值（Health）
- 魔法值（Mana）
- 攻击力（Damage）
- 移动速度（Movement Speed）
- 护甲（Armor）
- 冰冻时间（Freeze Duration）

这些属性通常会被实时更新，并且可以直接影响到游戏玩法和角色性能。它们的值可以在游戏过程中被增减、查询并应用到各种游戏逻辑中。

总的来说，Meta Attributes是关于普通属性如何管理和使用的附加规则集合，而普通属性则是角色状态的具体数值表现。两者结合使得GAS能够灵活且高效地处理各种复杂的角色状态和能力效果。

![image-20240415101515657](.\image-20240415101515657.png)

普通的游戏属性在多人游戏中会需要网络同步，进行网络复制，而元数据不需要进行网络同步，它仅仅作为一种占位符，被允许参与到计算中，用于计算其他GameplayEffect或者属性。



# Damage Meta Attributes

![image-20240415102811727](.\image-20240415102811727.png)

可以看到，我们目前的伤害是直接硬编码10点伤害，完全没有任何的属性计算的参与。

所以，这里新增一个DamageMetaAttributes，回到属性集中（AttributeSet）

```c++

	/*
	 * Meta Attributes 元属性, 用于描述角色的特性
	 */
	UPROPERTY(BlueprintReadOnly, Category="Meta Attributes")	// 蓝图只读，分类为Meta Attributes（元属性）, 元属性不需要复制
	FGameplayAttributeData IncomingDamage;	// 元属性：受到的伤害
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, IncomingDamage)	// 生成属性的Getter函数，属性的Setter函数，属性的初始化函数
```

然后在PostEffect（效果执行后的处理）中，执行Meta

```c++
void UAuraAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties EffectProperties;
	SetEffectsProperties(Data, EffectProperties);	// 设置效果属性

	// Clamp
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
	}
	else if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.0f, GetMaxMana()));
	}
	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		const float LocalIncomingDamage = GetIncomingDamage();
		SetIncomingDamage(0.0f);
		if (LocalIncomingDamage > 0.0f)
		{
			// Apply the damage to the target
			// 造成伤害
			const float NewHealth = GetHealth() - LocalIncomingDamage;
			SetHealth(FMath::Clamp(NewHealth, 0.0f, GetMaxHealth()));

			const bool bFatal = GetHealth() <= 0.0f;	// 是否致命
		}
	}

}
```

然后回到，蓝图，我们这次GE_Damage不再是直接修改Health，按照代码里的说法，应该传入伤害，通过伤害去间接修改Health，如下

![image-20240415104704615](.\image-20240415104704615.png)



# Set By Caler Magitude

1：新增Tag，Damage

```c++
	/*
	 * Meta Tags
	 */
	FGameplayTag Damage;						// 元标签：受到伤害

-----
    	/*
	* Meta Tags
	*/
	GameplayTags.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage"),
		FString("Damage")
	);
```

然后，拿火球术来举例，我们将伤害标签，添加

```c++
void UAuraProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation)
{
	
	// 投射物生成，我们希望他是在服务端生成，然后在客户端同步
	const bool bIsServer = GetOwningActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;

	// 生成位置，我不希望简单使用角色的位置，而是使用施法者武器上的插槽位置
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());

	// TODO: 设置投射物旋转，比如朝向目标

	if (CombatInterface)
	{
		const FVector SocketLocation = CombatInterface->GetCombatSocketLocation();
		FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();	// 获取朝向目标的旋转
		// 此时，如果怪物身高高于或者低于角色，那么投射物的朝向可能会有问题，我们需要调整一下
		// 因为我们希望投射物能尽可能平行，所以这里把Pith调整为0
		Rotation.Pitch = 0.f;
		
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);		// 使用武器插槽位置
		SpawnTransform.SetRotation(Rotation.Quaternion());	// 设置旋转(这里需要传入四元数）

		// SpawnActorDeferred 异步生成Actor 是因为我们希望在生成之前设置一些属性，比如伤害，速度等
		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
			ProjectileClass,	// 投射物类
			SpawnTransform,		// 生成位置
			GetOwningActorFromActorInfo(),	// 拥有者
			Cast<APawn>(GetOwningActorFromActorInfo()),	// 控制者
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn	// 碰撞处理方式, 总是生成
		);

		// TODO: 设置投射物属性，比如伤害，速度等
		const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwningActorFromActorInfo());	// 获取施法者的ASC
		const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), SourceASC->MakeEffectContext());	// 生成效果

		FAuraGameplayTags GameTags = FAuraGameplayTags::Get();	// 获取游戏标签
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameTags.Damage, 50.f);	// 设置伤害

		Projectile->DamageEffectSpecHandle = SpecHandle;	// 设置效果句柄

		Projectile->FinishSpawning(SpawnTransform);	// 完成生成
	}

}
```

```c++
		FAuraGameplayTags GameTags = FAuraGameplayTags::Get();	// 获取游戏标签
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameTags.Damage, 50.f);	// 设置伤害
```

"AssignTagSetByCallerMagnitude"是一种在虚幻引擎（Unreal Engine）中使用Gameplay Attribute System (GAS) 的一种配置。这个配置通常用于在游戏效果（GameEffect）中添加一个标签集（Tag Set），并允许调用者（通常是游戏中的某个对象或者系统）来设定标签集的幅度（Magnitude）。

在Gameplay Attribute System中，标签集是一种特殊的属性，它允许游戏开发者为游戏中的对象添加一组标签。这些标签可以用于标识对象的属性、状态、行为等，从而实现更复杂的逻辑和交互。

"AssignTagSetByCallerMagnitude"的具体工作原理如下：

1. 在"Attribute"下拉框中选择要影响的标签集。在这个例子中，选择的是"Tag Set"属性。

2. 在"Modifier Op"下拉框中选择操作类型。常见的操作类型包括Add（添加）、Subtract（移除）、Replace（替换）等。

3. 在"Magnitude Calculation Type"下拉框中选择幅度计算类型。在这个例子中，选择了"Set by Caller"，表示幅度由调用者设定。

4. 如果选择了"Set by Caller"，则需要在"Set by Caller Magnitude"部分配置相关的信息。在这个界面中，可以看到"Data Name"和"Data Tag"两个选项，这两个选项通常用于指定数据的来源。例如，你可以从某个变量或者标签中获取数据，然后将其作为标签集的幅度。

5. 根据需要，可以添加多个标签集，以实现更复杂的效果。例如，你可以添加一个减伤的标签集，当受到伤害时，根据减伤幅度减少实际受到的伤害。

6. 最后，将这个GameEffect应用到游戏中的某个对象或者系统中，使其产生预期的效果。

需要注意的是，这个界面中的设置需要结合具体的编程代码才能生效。在虚幻引擎中，这些设置通常会被序列化成JSON格式的数据，然后在游戏中通过运行时的逻辑来解析和应用。因此，理解这些设置的含义和作用，对于编写高效且灵活的游戏逻辑至关重要。

所以，这里我们第二个参数选择我们的Damage标签，这里硬编码传入50伤害。

> AssignTagSetByCallerMagnitude 这里需要设置一个键值对，键是伤害标签，值是伤害数值，然后GE在修饰符中，只需要根据键（Tag）去找数值就行

![image-20240415111741399](.\image-20240415111741399.png)