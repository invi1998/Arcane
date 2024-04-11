# Gameplay Ability 

现在这些是游戏能力系统的核心。
游戏能力是角色在游戏中可以执行的动作或技能。
这是一个从你的游戏能力派生出来的类，它定义了一个能力的功能以及可以使用该能力的条件。
因此，与使用简单函数实现动作不同，一个GAS游戏能力是一个异步运行的实例对象。
这意味着它可以在某个时间点被激活，并运行跨越时间段的多阶段任务，可能会或可能不会跨越时间段。
这些可能会分支成不同的任务，应用它们自己的游戏效果，一旦我们了解了游戏队列，我们将看到它们也可以启动那些任务。
而且这些可以是有条件的。
它们可能基于玩家或环境交互，游戏能力具有内置的复制和预测支持，就像游戏效果一样。
游戏能力还具有成本和冷却的内置概念，其中属性资源必须以足够的数量存在以支付能力的激活。
在这门课程中，我们将使用法力作为资源来实现这种成本机制，但实际上可以是任何东西，并且冷却时间会阻止能力在冷却期结束之前再次被激活。
游戏能力使用能力任务，尽管它们不一定需要能力任务，但能力任务是从你的能力任务类派生出来的，它是更专门形式的更一般的游戏任务类。
在游戏能力执行期间执行异步工作。
它们可以通过广播委托来影响执行流程。
这些任务可以在 C++中使用，但它们也可以以蓝图的形式出现，形式是一个特殊的蓝图节点，可以具有多个输出执行引脚。
这些是根据开发人员确定的事件而进行的委托广播的结果。
基本上，游戏中会发生一些事情。
您的能力任务将广播一个委托，并在蓝图中执行。
这意味着任务节点上的特定输出执行引脚将被执行。
这使我们能够在蓝图中轻松地映射出能力的控制流程，并且我们仍然可以享受到 C++ 的最佳性能的好处，因为任务所做的工作通常是在 C++ 方面完成的，尽管不一定是这样。
但是因为我们可以将这些能力任务制作成 C++ 并让它们在 C++ 中完成大部分工作，我们获得了灵活性，能够在能力蓝图中设计游戏能力的机制，并允许那些无法访问 C++ 领域的设计师在不牺牲性能的情况下对能力拥有更多控制权。
要使用游戏能力，必须向实体授予该能力。
当这种情况发生时，会创建一个游戏能力规范，并且该规范定义了与该特定能力相关的详细信息，包括游戏能力类本身、能力的级别以及可以在运行时更改的任何动态信息。
通常情况下，能力是在服务器上授予的，但是当发生这种情况时，能力规范会复制到拥有者客户端，以便他们可以从自己的游戏能力中激活它。
一旦激活，能力将保持激活状态，直到它们被结束或取消。
能力可以自行结束，也可以被外部取消。
因此，总结起来，游戏能力是定义由角色执行的特定技能、法术或任何类型的能力的类。
能力必须授予给能力系统组件以供使用，我们在服务器上进行此操作，此时授予一个能力规范并复制到拥有的客户端。
使用能力时，必须激活它。
直到结束或取消，它被视为处于活动状态。
能力具有成本和冷却时间的内置概念。
能力可以异步运行，并且多个能力可以同时处于活动状态。
一个能力可以运行能力任务，这些任务是封装行为到单独类中的异步操作，每个类都可以执行自己特定的工作。

![image-20240411102013069](.\image-20240411102013069.png)



1. **Gameplay Abilities**: GAS中的Gameplay Abilities是定义技能或能力的类。这些类定义了特定的能力，如攻击、治疗、防御等。

2. **Must be granted**: 所有的Gameplay Abilities都必须被授予才能使用。这些能力通常由服务器授予，并复制到拥有该能力的客户端。

3. **Must be activated to be used**: 能力必须被激活才能使用。激活能力通常需要消耗一定的资源（如法力、能量、冷却时间等）。

4. **Cost and Cooldown**: 能力的使用通常需要消耗资源，如法力、能量等，并且有冷却时间限制。

5. **Abilities run asynchronously**: 能力运行是异步的，这意味着多个能力可以同时处于激活状态。

6. **Ability Tasks**: GAS还提供了Ability Tasks，用于异步执行操作。这些任务可以在能力激活期间或之后异步执行，例如计算伤害、施放特效等。

总结：
- GAS中的Gameplay Abilities是定义游戏技能或能力的类。
- 能力必须被授予并激活才能使用。
- 能力的使用通常需要消耗资源，并有冷却时间限制。
- GAS支持异步运行能力，允许多个能力同时处于激活状态。
- Ability Tasks是异步执行的操作，可以在能力激活期间或之后异步执行。

# Granting Abilities (授予能力)

首先，我希望角色的能力（技能）信息存放在角色上，所以我在角色基类这里添加一个能力指针数组。UAuraGameplayAbility 表明，这里我是基于GameplayAbility创建的一个自己的Ability类，后续我们会对这个类做很多自定义的事情。

```c++
UPROPERTY(EditAnywhere, Category="Abilities")
TArray<TSubclassOf<UAuraGameplayAbility>> StartupAbilities;	// 默认能力
```

同时，新增一个为角色添加能力的函数。给角色新增技能，这种事情只能在服务端进行。

```c++
void AAuraCharacterBase::AddCharacterAbilities()
{
	UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(GetAbilitySystemComponent());

	// 给角色添加技能，这一行为应该只能在服务端进行
	if (!HasAuthority() || !AuraASC) return;

	AuraASC->AddCharacterAbilities(StartupAbilities);

}
```

可以看到，我们的能力添加，其实最终是添加进ASC中，（每个角色，包括怪物都有一个ASC），所以我在ASC中创建一个函数用来添加技能

```c++
void UAuraAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UAuraGameplayAbility>>& StartupAbilities)
{
	for (TSubclassOf<UGameplayAbility> Ability : StartupAbilities)
	{
		if (Ability)
		{
            // 创建能力
            FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability.GetDefaultObject(), 1, 0);
            // 将能力添加到AbilitySystemComponent中
			// GiveAbility(AbilitySpec);   // 添加能力
            GiveAbilityAndActivateOnce(AbilitySpec);    // 添加并激活能力
		}
	}
}
```

## GiveAbility 和 GiveAbilityAndActivateOnce

在Unreal Engine 5 (UE5) 的Gameplay Ability System (GAS) 中，GiveAbility 和 GiveAbilityAndActivateOnce 都是用来赋予玩家角色能力的方法，但它们之间存在一些关键区别。

**GiveAbility** 方法用于将一个Gameplay Ability赋予玩家角色。这个方法不会立即激活赋予的能力，而是等待玩家角色手动激活。当玩家角色激活能力时，能力才会生效。这个方法通常用于在游戏开始时赋予玩家角色基础能力，或者在游戏过程中根据需要赋予玩家角色新的能力。

**GiveAbilityAndActivateOnce** 方法则是在赋予玩家角色能力的同时，立即激活一次该能力。这个方法通常用于一次性事件，例如给予玩家角色一个临时增益效果，或者一次性奖励。

下面是这两个方法的参数和用法的简要说明：

**GiveAbility** 方法：
```cpp
void GiveAbility(TSubclassOf<UGameplayAbility> AbilityClass, AActor* AbilityOwner = nullptr, UGameplayAbility* ExistingAbilityToReplace = nullptr);
```
- `AbilityClass`: 要赋予的Gameplay Ability的类。
- `AbilityOwner`: 要赋予能力的玩家角色。如果未指定，则默认为调用此方法的玩家角色。
- `ExistingAbilityToReplace`: 如果指定，将替换现有具有相同类的Gameplay Ability。

**GiveAbilityAndActivateOnce** 方法：
```cpp
void GiveAbilityAndActivateOnce(TSubclassOf<UGameplayAbility> AbilityClass, AActor* AbilityOwner = nullptr, UGameplayAbility* ExistingAbilityToReplace = nullptr);
```
- `AbilityClass`: 要赋予的Gameplay Ability的类。
- `AbilityOwner`: 要赋予能力的玩家角色。如果未指定，则默认为调用此方法的玩家角色。
- `ExistingAbilityToReplace`: 如果指定，将替换现有具有相同类的Gameplay Ability。

在使用这两个方法时，需要注意的是，`GiveAbility` 方法只是赋予玩家角色能力，而不会立即激活能力；而`GiveAbilityAndActivateOnce` 方法则会在赋予能力的同时立即激活一次能力。因此，在使用这两个方法时，需要根据具体的游戏设计需求来选择合适的方法。

然后，在什么时候赋予角色技能呢？

对于玩家角色的初始技能，自然是在 PossessedBy 这里最合适。

```c++
void AAuraCharacter::PossessedBy(AController* NewController)
{
	// 该函数只会在服务器端被调用
	// PossessedBy()函数通常在服务器端被调用。当一个Controller（如PlayerController或AIController）开始控制一个Pawn（如PlayerCharacter或AICharacter）时，服务器会调用PossessedBy()函数来设置Pawn的Controller属性。
	Super::PossessedBy(NewController);

	// 为服务器初始化AbilitySystemComponent
	InitAbilityActorInfo();

	// 初始化角色能力
	AddCharacterAbilities();
	
}
```



# 游戏能力设置 (Settings on Gameplay Abilities)

## Tag

![image-20240411110752843](.\image-20240411110752843.png)

![image-20240411110651058](.\image-20240411110651058.png)

在Unreal Engine 5 (UE5) 中，Ability System插件引入了一种名为“Tags”的机制，用于标识和管理游戏中的各种对象。Tags在Ability System中扮演着重要角色，它们可以用来控制能力的激活、取消和其他行为。下面是对Ability System中与Tags相关的各项设置的详细解释：

1. **Ability Tags**：这是能力自身拥有的标签。这些标签可以用来标识能力的类型或特性。例如，一个能力可能有一个“远程攻击”标签，表示它是远程攻击能力。

2. **Cancel Abilities with Tag**：当这个能力被激活时，会取消所有具有这些标签的能力。这可以用来实现某些能力之间的相互排斥，例如，当一个角色使用近战攻击时，取消所有的远程攻击能力。

3. **Block Abilities with Tag**：当这个能力激活时，会阻止所有具有这些标签的能力。这可以用来实现能力的优先级控制，例如，当一个角色使用终极技能时，阻止其他普通技能的使用。

4. **Activation Owned Tags**：这些标签将在激活能力时应用到激活者身上。这些标签可以用来标识激活者的状态或属性变化。例如，一个能力可能在激活时给激活者添加一个“无敌”标签，表示激活者暂时免疫伤害。

5. **Activation Required Tags**：只有当激活者具有所有这些标签时，才能激活这个能力。这可以用来限制能力的使用条件，例如，只有当角色达到一定等级时，才能激活某个能力。

6. **Activation Blocked Tags**：如果激活者具有任何这些标签中的任何一个，那么这个能力将无法激活。这可以用来防止某些情况下的能力滥用，例如，当角色处于战斗状态时，禁止使用某些能力。

7. **Source Required Tags**：只有当来源对象（例如，技能施放者）具有所有这些标签时，才能激活这个能力。这可以用来限制能力的使用条件，例如，只有当角色装备某种武器时，才能使用相应的技能。

8. **Source Blocked Tags**：如果来源对象具有任何这些标签中的任何一个，那么这个能力将无法激活。这可以用来防止某些情况下的能力滥用，例如，当角色处于冷却状态时，禁止使用某些技能。

9. **Target Required Tags**：只有当目标对象（例如，技能作用对象）具有所有这些标签时，才能激活这个能力。这可以用来限制能力的作用范围，例如，只有对具有“可攻击”标签的目标使用，才能造成伤害。

10. **Target Blocked Tags**：如果目标对象具有任何这些标签中的任何一个，那么这个能力将无法激活。这可以用来防止某些情况下的能力滥用，例如，当目标对象处于无敌状态时，禁止对其使用技能。

通过这些设置，开发者可以精细地控制能力的激活、取消和阻止，从而实现复杂的游戏逻辑。



## Input

![image-20240411111855846](.\image-20240411111855846.png)

"Replicate Input Directly"这个设置项的作用是决定是否将输入事件直接复制到网络上的所有玩家。如果勾选了这个选项，那么当一个玩家按下某个按键或触发某个输入事件时，这个输入事件将会被同步到网络上的其他玩家，这样其他玩家也可以看到这个玩家的动作。

这个设置项通常在多人在线游戏中使用，以确保所有玩家都能看到其他玩家的动作。例如，在一个多人在线射击游戏中，当一个玩家按下空格键跳跃时，其他玩家也能看到这个玩家的跳跃动作。

然而，直接复制输入事件可能会导致网络带宽的增加，因此在使用这个设置项时需要谨慎考虑。如果网络带宽有限，或者游戏中的输入事件非常频繁，那么直接复制输入事件可能会导致网络延迟增加，影响游戏体验。

因此，使用"Replicate Input Directly"这个设置项时，建议先在本地测试中验证其效果，确保网络带宽和输入事件的频率都在可接受范围内。如果在网络测试中发现网络延迟增加，可以尝试减少输入事件的频率，或者使用其他方法来减少网络带宽的使用。



## Advanced  (高级)

![image-20240411112347963](.\image-20240411112347963.png)

在Ability设置项的Advanced（高级）属性设置中，包含了一些高级选项，用于控制Ability的网络同步和安全策略。以下是各个配置项的作用及其子项的功能和使用建议：

1. **Replication Policy**：这个设置决定了Ability是否在网络上传输。可以选择的选项包括：

   - **Do Not Replicate**：不复制Ability到网络上。
   - **Replicated**：将Ability复制到网络上的所有玩家。
   - **Server Authoritative**：将Ability的控制权交给服务器，服务器负责将Ability的状态同步到客户端。

   根据游戏的网络架构和需求，选择合适的复制策略。

2. **Instancing Policy**：这个设置决定了Ability的实例化策略。可以选择的选项包括：

   - **Instanced Per Execution**：每次执行Ability时都会创建一个新的实例。
   - **Singleton**：只创建一个Ability实例，无论何时执行Ability都会使用同一个实例。

   根据Ability的使用方式和性能要求，选择合适的实例化策略。

3. **Server Respects Remote Ability Calls**：这个选项决定了服务器是否尊重来自远程客户端的Ability调用。如果勾选，服务器将允许远程客户端启动Ability。否则，只有服务器自己可以启动Ability。

   这个选项通常用于多人在线游戏，以允许玩家在远程启动Ability。

4. **Retrigger Instanced Ability**：这个选项决定了是否允许重新触发已经实例化的Ability。如果勾选，即使Ability已经实例化，也可以再次触发。否则，一旦Ability实例化，就不能再触发。

   根据Ability的使用场景和设计，选择是否允许重新触发。

5. **Net Execution Policy**：这个设置决定了Ability在网络上的执行策略。可以选择的选项包括：

   - **Local Predicted**：在本地预测执行Ability，然后将结果同步到服务器。
   - **Server Authoritative**：服务器控制Ability的执行和状态更新。
   - **Server Authoritative With Prediction**：服务器控制Ability的执行，但客户端可以预测Ability的状态。

   根据游戏的网络架构和性能要求，选择合适的执行策略。

6. **Net Security Policy**：这个设置决定了Ability在网络上的安全策略。可以选择的选项包括：

   - **Client Or Server**：客户端和服务器都可以启动Ability。
   - **Server Only**：只有服务器可以启动Ability。

   根据游戏的安全需求，选择合适的安全策略。

在使用这些高级设置时，需要根据游戏的具体需求和网络架构进行调整。例如，如果游戏需要保证安全性，可以选择更严格的网络安全策略。如果游戏需要实时同步Ability的状态，可以选择合适的网络执行策略。



Cast （）