# 执行本地GameplayCue

游戏提示（GameplayCue）默认复制，因为我们知道有 RPC，尽管我们已经提高了每个网络更新的项目 RPC 限制到十个，但是对于一个一次性生成12个火球的技能来说，12这个数字还是太大了。所以，我们应该尽量不使用GameplayCue，或者应该节制使用。但是如果我们非要使用GameplayCue，并且是在本地使用而不发送RPC怎么办？

如何执行本地GameplayCue？如果我们搜索源文件，可以在GameplayCueManager.h中看到这几个静态函数

```c++
	/** 
	 *  Convenience methods for invoking non-replicated gameplay cue events. 
	 * 
	 *	We want to avoid exposing designers the choice of "is this gameplay cue replicated or non-replicated?".
	 *	We want to make the decision for them in most cases:
	 *	- Abilities will always use replicated GameplayCue events because they are not executed on simulated proxies.
	 *	- Animations always use non-replicated GameplayCue events because they are always executed on simulated proxies.
	 *	
	 *  Sometimes it will be useful to give designers both options: in actor classes where there are many possible use cases.
	 *  Still, we should keep the choice confined to the actor class, and not globally.  E.g., Don't add both choices to the function library
	 *  since they would appear everywhere. Add the choices to the actor class so they only appear there.
	 */
	static void AddGameplayCue_NonReplicated(AActor* Target, const FGameplayTag GameplayCueTag, const FGameplayCueParameters& Parameters);
	static void RemoveGameplayCue_NonReplicated(AActor* Target, const FGameplayTag GameplayCueTag, const FGameplayCueParameters& Parameters);
	static void ExecuteGameplayCue_NonReplicated(AActor* Target, const FGameplayTag GameplayCueTag, const FGameplayCueParameters& Parameters);

	// -------------------------------------------------------------
```

> ```
> /**
> 2 * 提供了几个方便的方法来调用非复制的游戏提示事件。
> 3 *
> 4 * 我们希望避免让设计师选择“此游戏提示是复制还是非复制？”。
> 5 * 在大多数情况下，我们要为他们做出决定：
> 6 * - 技能总是使用复制的游戏提示事件，因为它们不在模拟代理上执行。
> 7 * - 动画始终使用非复制的游戏提示事件，因为它们总是在模拟代理上执行。
> 8 * 
> 9 * 有时，对于有许多可能用例的演员类，给设计师两个选项是有用的。
> 10 * 然而，我们应该将选择限制在演员类中，而不是全局范围。例如，不要将两者都添加到函数库，
> 11 * 因为它们会出现在所有地方。将这些选项添加到演员类，使它们只在那里出现。
> 12 */
> ```

具体来说，这是关于非复制的游戏玩法提示（Gameplay Cue）事件的一些便利方法。

在Unreal Engine中，Gameplay Cues是一种机制，允许开发者定义和管理与游戏角色相关的特定行为或效果。这些可能包括播放动画、发出声音或者改变角色外观等。Gameplay Cues可以根据是否需要在网络游戏中同步到所有玩家分为两类：复制的（replicated）和非复制的（non-replicated）。复制的Gameplay Cues会通过网络发送给所有玩家，而非复制的则只在本地机器上执行。

在这段代码中，我们看到三个静态函数，它们都负责处理非复制的游戏玩法提示：

1. `static void AddGameplayCue_NonReplicated(AActor* Target, const FGameplayTag GameplayCueTag, const FGameplayCueParameters& Parameter);`: 这个函数用于向目标演员（Target Actor）添加一个非复制的游戏玩法提示。它接受一个指向目标演员的指针，一个表示游戏玩法提示的Gameplay Tag，以及一个包含额外参数的结构体引用。

2. `static void RemoveGameplayCue_NonReplicated(AActor* Target, const FGameplayTag GameplayCueTag, const FGameplayCueParameters& Parameter);`: 这个函数用于从目标演员移除一个非复制的游戏玩法提示。它的作用类似于`AddGameplayCue_NonReplicated()`, 但是它删除而不是添加游戏玩法提示。

3. `static void ExecuteGameplayCue_NonReplicated(AActor* Target, const FGameplayTag GameplayCueTag, const FGameplayCueParameters& Parameter);`: 这个函数用于立即执行一个非复制的游戏玩法提示。它接受相同类型的参数，并且立即触发所指定的游戏玩法提示。

这些函数都是静态的，这意味着它们可以直接被其他类调用，而不需要创建该类的对象。此外，它们还提供了灵活性，因为它们接受一个`FGameplayCueParameters`结构体作为参数，这个结构体可以包含任何与游戏玩法提示相关的数据。



```c++
void AAuraFireBall::OnHit()
{
	// 如何执行本地GameplayCue？
	if (GetOwner())
	{
		// 播放击中特效, 播放击中音效
		FGameplayCueParameters CueParameters;

		CueParameters.Location = GetActorLocation();

		UGameplayCueManager::ExecuteGameplayCue_NonReplicated(GetOwner(), FAuraGameplayTags::Get().GameplayCue_FireBlast, CueParameters);

	}

	// 停止飞行音效
	if (LoopingSoundComponent)
	{
		LoopingSoundComponent->Stop();
		LoopingSoundComponent->DestroyComponent();
	}

	bHit = true;
}

```

我们在需要进行本地执行Cue的地方执行该函数，（需要事先设置好Owner以及Cue参数）

然后在蓝图中添加GameplayCue。![image-20240622161611581](.\image-20240622161611581.png)

![image-20240622162135526](.\image-20240622162135526.png)

然后在GameplayCue中指定你想要的特效和指定好Tag（因为GameplayCue的执行就是依赖的Tag），比如我这里希望在本地有一个爆炸特效和一个爆炸音效，如上。

这种事很好的，因为我们知道GameplayCue默认是网络多播的，它是会发送RPC的，我们采用了本地执行GameplayCue后，不会产生任何RPC，这是很高效和网络友好的。
