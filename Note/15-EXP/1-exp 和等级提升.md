

# Exp

![image-20240429104532997](.\image-20240429104532997.png)



## Awarding EXP

![image-20240429131049899](.\image-20240429131049899.png)

图中展示了击杀怪物获取经验的过程。整个流程由多个步骤组成，包括监听游戏事件、奖励经验、处理攻击和死亡事件等。

具体来说，流程如下：

1. 游戏事件监听：通过GA_监听事件（ListenForEvents）节点监听游戏事件，当游戏事件触发时，执行下一步。
2. 经验奖励：当游戏事件触发时，通过Gameplay Event Event节点执行奖励经验的操作。这可以通过设置Gameplay Event Event节点的参数来实现。
3. 攻击处理：通过Attack节点处理攻击事件，当玩家对怪物造成伤害时，执行下一步。
4. 死亡处理：通过Death节点处理怪物死亡事件，当怪物死亡时，执行下一步。
5. 经验扣除：通过Gameplay Event Event节点执行经验扣除的操作。这可以通过设置Gameplay Event Event节点的参数来实现。

通过这种方式，GAS可以实现游戏中的经验奖励

![image-20240429131007461](.\image-20240429131007461.png)



1. **XP Reward on Enemies (and a way to get it)**：在敌人身上设置经验值奖励，并提供获取这些经验值的方法。
2. **Incoming XP Meta Attribute**：创建一个用于接收经验值的元属性。
3. **Passive Gameplay Ability, GA_ListenForEvents (and grant it)**：添加一个被动游戏能力，称为GA_ListenForEvents，并赋予玩家此能力。
4. **Gameplay Effect to apply in response to Event**：为响应事件而应用的游戏效果。
5. **Award XP from Attribute Set when damage is fatal**：当伤害是致命的时，从属性集中奖励经验值。
6. **Handle Incoming XP in Attribute Set and increase XP on Player State**：处理来自属性集的传入经验值，并增加玩家状态中的经验值。



# 被动监听事件(Passively Listening For Events)

对于经验的获取，我们希望这是一个被动技能，它一旦被赋予，就会持久存在，换句话说，它是一个持续类型的能力，同时，我们不希望这个被动技能被RPC传递给客户端，它只需要存在服务端即可。所以，我们可以将能力设置为ServetOnly，只在服务端存在，自然不存在需要预测策略。同时，实例化策略选择为每个Actor创建一个实例，这样就不用再每次激活或者获得能力的时候创建一个实例，优化实例策略。

![image-20240429171705044](.\image-20240429171705044.png)

然后在Acitvate节点后，开始添加游戏监听节点。这里我取消勾选完全匹配，因为我希望这个监听能力不止监听一种单一属性，它可以监听一些带相同tag的模糊属性，让他变得更通用。同时也取消勾选`Only triggger once（仅触发一次）`，因为作为一个监听事件，它应该在游戏运行过程中，响应每次触发。

![image-20240429173652194](.\image-20240429173652194.png)

如图，我们希望该监听事件监听Attributes下的所有属性Tag，这自然也包括我们现在做的Exp，在得到监听结果后，payload节点会输入EventData，我们可以通过Tag进行匹配。不过，首先，我们要先提供一个GameplayEffect，用来处理收到监听结果后的属性处理。

![image-20240429174045251](.\image-20240429174045251.png)

然后回到监听蓝图，回顾以前我们赋予或者说应用一个GameplayEffect的过程

> 1；首先需要准备好GameplayEffectClass，用来指定该游戏效果，这里就是我们上面新创建的这个GE，用来处理经验获取。所以，我们需要在蓝图中添加这样一个变量，他是GameplayEffect的类引用（记住是类引用，不是GE实例，因为这里只是用来存储GE类型，后面具体应用效果的时候才会根据该类型进行实例创建），然后指定默认值，将我们的GE指定进去
>
> ![image-20240429174656329](.\image-20240429174656329.png)
>
> 2：然后就是具体的GE创建过程，以及GE apply
>
> > 通过ASC生成EffectContext（效果上下文）
> >
> > 添加源对象
> >
> > 创建EffectSpecHandle
> >
> > MakeOutGoing
> >
> > ApplyGameplayEffectSpecToSelf
>
> 对应到蓝图，就是如下
>
> ![image-20240429180447124](.\image-20240429180447124.png)

这样我们就可以将基于事件的GE通过调用者的游戏事件数据传递的内容里的大小(比如等级)设置其效果规格，然后应用到角色自身

在我们实际发送事件之前，我们还应该考虑角色不具备这种GA的情况，角色没有任何被动监听的游戏能力，那么这些能力应该尽快激活如果能力被授予的话。

所以，我们还应该做的就是，在角色上应该还有一系列的被动游戏能力(passive gameplay abilities)，这些能力应该由ASC提供，因为是被动技能，所以只在提供的时候激活一次即可，后续持续存在

```c++
private:
	UPROPERTY(EditAnywhere, Category="Abilities")
	TArray<TSubclassOf<UAuraGameplayAbility>> StartupAbilities;	// 默认能力


	UPROPERTY(EditAnywhere, Category="Abilities")
	TArray<TSubclassOf<UAuraGameplayAbility>> StartupPassiveAbilities;	// 默认被动能力
};
```

然后是ASC的Ability添加函数，这里我们被动技能和和普通技能不太一样，我们希望角色被动（exp获取）在被授予的时候就进行激活，所以我们用GiveAbilityAndActivateOnce

```c++
void UAuraAbilitySystemComponent::AddCharacterPassiveAbilities(
	const TArray<TSubclassOf<UAuraGameplayAbility>>& StartupAbilities)
{
	for (TSubclassOf<UGameplayAbility> Ability : StartupAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability.GetDefaultObject(), 1);
		GiveAbilityAndActivateOnce(AbilitySpec);   // 添加能力并激活一次
	}
}
```

然后就是在Character中调用

```c++
void AAuraCharacterBase::AddCharacterAbilities()
{
	UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(GetAbilitySystemComponent());

	// 给角色添加技能，这一行为应该只能在服务端进行
	if (!HasAuthority() || !AuraASC) return;

	AuraASC->AddCharacterAbilities(StartupAbilities);
	AuraASC->AddCharacterPassiveAbilities(StartupPassiveAbilities);

}
```

![image-20240429182730390](.\image-20240429182730390.png)



# Send EXP Events

现在，我们的蓝图已经准备好了Ability，也可以进行事件监听，下一步，就是在击杀怪物的时候，发送经验奖励事件，事件里，我们只需要组装好经验事件需要的参数（Payload：tag和经验值）

```c++

void UAuraAttributeSet::SendEXPEvent(const FEffectProperties& Props) const
{
	if (Props.TargetCharacter)
	{
		// 获取Combat
		if (const ICombatInterface* CombatInterface = Cast<ICombatInterface>(Props.TargetCharacter))
		{
			// 获取NPC经验奖励
			int32 EXP = UAuraAbilitySystemLibrary::GetMonsterEXPRewardByClassAndLv(this, CombatInterface->GetCharacterClass(), CombatInterface->GetPlayerLevel());

			const FAuraGameplayTags& AuraGameplayTags = FAuraGameplayTags::Get();	// 获取标签

			// 创建Payload
			FGameplayEventData Payload;
			Payload.EventTag = AuraGameplayTags.Attributes_Meta_RewardExperience;
			Payload.EventMagnitude = EXP;

			// 发送游戏事件
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Props.SourceCharacter, AuraGameplayTags.Attributes_Meta_RewardExperience, Payload);
		}

	
	}
}
```

> 注意：上面的代码有一处错误。
>
> ```c++
> // 获取角色职业
> UFUNCTION(BlueprintCallable, BlueprintNativeEvent)	// 蓝图可调用，蓝图原生事件
> ECharacterClass GetCharacterClass() const;
> 
> virtual int32 GetPlayerLevel() const;
> ```
>
> `CombatInterface->GetCharacterClass(), CombatInterface->GetPlayerLevel()`这个地方，通过Combat接口获取角色职业，我们不能直接调用它的原始版本的函数，因为这个接口被定义为蓝图原生事件（BlueprintNative）,所以我们要调用的他的执行版本（Execute_GetCharacterClass），而GetPlayerLevel()是一个普通的虚函数，所以正常调用就行
>
> ```c++
> // 获取角色职业
> const ECharacterClass CharacterClass = CombatInterface->Execute_GetCharacterClass(Props.SourceAvatarActor);
> // 获取NPC经验奖励
> int32 EXP = UAuraAbilitySystemLibrary::GetMonsterEXPRewardByClassAndLv(this, CharacterClass, CombatInterface->GetPlayerLevel());
> 
> ```
>
> 