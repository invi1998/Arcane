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

