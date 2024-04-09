# Derived Attributes (当前游戏项目里的派生属性)

![image-20240409123124782](.\image-20240409123124782.png)

这张图片展示了游戏项目中关于属性的设计图。图中分为两个部分，左边是“Primary Attributes”（主要属性），右边是“Secondary Attributes”（次要属性）。主要属性包括力量（Strength）、智力（Intelligence）、耐力（Resilience）和活力（Vigor），而次要属性则与这些主要属性相关联，提供了更多的游戏机制和角色特性。

具体来说，主要属性影响了次要属性的效果。例如，力量（Strength）增加了物理伤害，智力（Intelligence）增加了魔法伤害。耐力（Resilience）则影响了护甲（Armor）和护甲穿透（Armor Penetration），活力（Vigor）影响了最大生命值（Max Health）和生命恢复（Health Regeneration）。

次要属性中，装甲（Armor）可以减少所受的伤害，并提高格挡几率（Block Chance）。格挡几率（Block Chance）则是抵挡一半伤害的几率。护甲穿透（Armor Penetration）可以无视敌人的一部分护甲，从而提高暴击几率（Critical Hit Chance）。暴击几率（Critical Hit Chance）决定了玩家攻击时触发双倍伤害和暴击加成的几率。暴击伤害（Critical Hit Damage）是在暴击时额外增加的伤害。此外，活力（Vigor）还影响了暴击抵抗（Critical Hit Resistance）和最大生命值（Max Health）。

从整体来看，这张图片展示了游戏中的属性系统如何运作，以及不同属性如何相互影响，从而提供多样化的角色发展路径和游戏体验。

以下是图中列出的主要属性及其对应的次要属性：

1. 力量（Strength）
	* 抵抗力（Resilience）：增加物理伤害
	* 耐力（Resilience）：增加韧性，减少受到的伤害
	* 活力（Vigor）：增加最大生命值（Max Health）

2. 智力（Intelligence）
	* 抵抗力（Resilience）：增加魔法伤害
	* 活力（Vigor）：增加法力值恢复（Mana Regeneration）
	* 活力（Vigor）：增加最大法力值（Max Mana）

3. 耐力（Resilience）
	* 抵抗力（Resilience）：增加韧性，减少受到的伤害
	* 耐力（Resilience）：增加护甲（Armor）
	* 耐力（Resilience）：增加护甲穿透（Armor Penetration）

4. 活力（Vigor）
	* 活力（Vigor）：增加最大生命值（Max Health）
	* 活力（Vigor）：增加生命恢复（Health Regeneration）
	* 活力（Vigor）：增加最大法力值（Max Mana）

5. 护甲（Armor）
	* 抵抗力（Resilience）：减少受到的伤害
	* 抵抗力（Resilience）：增加格挡几率（Block Chance）

6. 格挡几率（Block Chance）
	* 护甲（Armor）：增加格挡几率（Block Chance）

7. 护甲穿透（Armor Penetration）
	* 抵抗力（Resilience）：无视敌人的一部分护甲，提高暴击几率（Critical Hit Chance）

8. 暴击几率（Critical Hit Chance）
	* 护甲穿透（Armor Penetration）：提高暴击几率（Critical Hit Chance）

9. 暴击伤害（Critical Hit Damage）
	* 护甲穿透（Armor Penetration）：在暴击时增加额外的伤害

10. 暴击抵抗（Critical Hit Resistance）
	* 活力（Vigor）：减少受到的暴击伤害

11. 最大生命值（Max Health）
	* 活力（Vigor）：增加最大生命值（Max Health）

12. 生命恢复（Health Regeneration）
	* 活力（Vigor）：增加生命恢复（Health Regeneration）

13. 最大法力值（Max Mana）
	* 活力（Vigor）：增加最大法力值（Max Mana）

14. 法力恢复（Mana Regeneration）
	* 活力（Vigor）：增加法力恢复（Mana Regeneration）

这些属性关系说明了游戏中的角色可以通过提升主要属性来解锁和增强次要属性，从而实现角色的成长和发展。



# Modifier Magnitude Calculations

我们此前的属性初始化都是使用系统自带的修饰符，现在，对于最大血量和最大蓝量，我们希望做一些自定义的事情，比如我们最大蓝量和最大血量取决于智力和体质，还有角色等级，而这个角色等级，我们放在了其他类中，PlayerState，需要从其他类（如玩家状态）获取值，这时候，编辑器里提供的那些修饰符已经无法满足我们的使用场景。所以这里我们需要添加一个自定义属性修饰符

回到代码，我们创建一个基类 为 GameplayModMagnitudeCalculation 的自定义修饰符

## UGameplayModMagnitudeCalculation

在Unreal Engine 5 (UE5) 中，UGameplayModMagnitudeCalculation 类是一个用于计算游戏性属性修改器（Gameplay Attribute Modifier）的幅度（Magnitude）的基类。游戏性属性修改器是一种机制，允许在运行时动态地改变游戏对象的属性值，例如生命值、攻击力等。

UGameplayModMagnitudeCalculation 类定义了以下几种类型的游戏性属性修改器幅度计算方法：

1. **固定值（FixedValue）**：幅度始终等于一个固定的数值。
2. **线性函数（LinearFunction）**：幅度基于一个线性函数计算，即幅度等于输入值乘以一个常数。
3. **二次函数（QuadraticFunction）**：幅度基于一个二次函数计算，即幅度等于输入值的平方乘以一个常数。
4. **曲线（Curve）**：幅度基于一个预定义的曲线计算，曲线可以在编辑器中进行自定义。

这些计算方法被用于根据游戏中的各种条件动态地调整属性修改器的幅度。例如，当一个角色受到伤害时，可以使用线性函数来计算减少的生命值幅度；当一个技能的等级提高时，可以使用二次函数来计算增加的技能伤害幅度。

通过继承UGameplayModMagnitudeCalculation类并实现自己的计算逻辑，开发者可以创建自定义的幅度计算方法，以满足游戏中的特殊需求。



```c++
/**
 * 
 */
UCLASS()
class ARCANE_API UMMC_MaxHealth : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	UMMC_MaxHealth();

	// 这个函数是用来计算最终的数值的，修饰符
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
	// 为我们想要捕获的属性创建一个属性定义
	FGameplayEffectAttributeCaptureDefinition VigorDef;		// 体质
};
```



```c++
UMMC_MaxHealth::UMMC_MaxHealth()
{
	VigorDef.AttributeToCapture = UAuraAttributeSet::GetVigorAttribute();		// 体质
	VigorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;	// 目标
	VigorDef.bSnapshot = false;	// 不是快照，快照是指在效果生效时捕获的属性值，而不是在效果创建时捕获的属性值，所以效果被创建时，属性值是多少，效果就是多少，这里获取的是真实值

	// 对体质（活力）定义设置了这些基础属性后，我们的修饰符计算还需要一个变量，他是要捕获的属性数值之一
	// 我们需要将这个 VigorDef 添加到 CapturedAttributeModifiers 数组中，这样我们的修饰符计算就可以获取到这个属性的数值了
	RelevantAttributesToCapture.Add(VigorDef);	// 添加到捕获属性数组中
	// 这样，在最终执行修饰符计算时，我们就可以获取到体质的数值了
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Cather tags from source and target （从来源和目标中捕获标签）
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();		// 来源标签
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();		// 目标标签

	// 现在，为了捕获一个属性并获取他的数值，我们需要使用 FAggregatorEvaluateParameters 结构体, 这个结构体包含了我们需要的所有信息
	FAggregatorEvaluateParameters EvaluationParameters;	// 评估参数
	EvaluationParameters.SourceTags = SourceTags;	// 来源标签
	EvaluationParameters.TargetTags = TargetTags;	// 目标标签

	float Vigor = 0.f;	// 体质
	GetCapturedAttributeMagnitude(VigorDef, Spec, EvaluationParameters, Vigor);	// 获取捕获的属性数值，传入捕获定义，效果规格，评估参数，体质（属性）

	// Clamp the value to be positive （将值限制为正数）
	Vigor = FMath::Max<float>(Vigor, 0.f);	// 体质

	// 同时，我们希望玩家的最大生命值不仅取决于体质（vigor），还取决于其他因素，比如等级，所以我们需要一个公式来计算最大生命值
	// 这也是我们创建这个修饰符的原因，因为我们在这里可以始终获取到这个效果的来源和目标，所以我们可以根据来源和目标的属性来计算最大生命值

	const ICombatInterface* SourceActor = Cast<ICombatInterface>(Spec.GetEffectContext().GetSourceObject());	// 来源角色
	const int32 Level = SourceActor ? SourceActor->GetPlayerLevel() : 1;	// 等级

	return 80.f + Level * 10.5f + Vigor * 2.5f;	// 返回最大生命值

}
```

然后，编译运行，在编辑器中，找到我的次要属性的初始化GE，选择我们的MaxHealth，采用我们新写的计算修饰符。这你甚至可以看到，我们还以在Editor中，继续在修饰符计算的基础上再使用前置，后置这些运算进一步细化我们的数值。这都是可以的。

![image-20240409182339130](.\image-20240409182339130.png)

总结一下：自定义修饰符的编写逻辑

在Unreal Engine 5 (UE5) 中，添加自定义属性修饰符的过程通常涉及以下几个步骤：

1. 创建自定义属性修饰符类：
   - 继承自`UGameplayModMagnitudeCalculation`类。
   - 在类定义中包含必要的成员变量和函数。
   - 实现`CalculateBaseMagnitude_Implementation()`函数，该函数用于计算属性修饰符的幅度。
2. 注册自定义属性修饰符类：
   - 在项目中注册属性修饰符类，以便在运行时使用。
   - 可以在插件的`Config`文件夹下创建`.ini`文件，将属性修饰符类注册到`GameplayTag`系统中。
3. 使用自定义属性修饰符：
   - 在游戏逻辑中，创建属性修饰符实例，并将其应用到目标对象上。
   - 使用`FGameplayEffectAttributeCaptureDefinition`结构体来定义要捕获的目标属性。
   - 将属性修饰符添加到`RelevantAttributesToCapture`数组中。
4. 计算属性修饰符的幅度：
   - 在`CalculateBaseMagnitude_Implementation()`函数中，根据捕获的属性值和游戏逻辑来计算属性修饰符的幅度。
   - 可以使用`GetCapturedAttributeMagnitude()`函数来获取捕获的属性值。
5. 应用属性修饰符：
   - 将计算出的幅度应用到目标对象的属性上，以影响其行为或状态。

![image-20240409184858032](.\image-20240409184858032.png)