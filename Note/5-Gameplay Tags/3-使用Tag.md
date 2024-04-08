之前我们已经给资产添加好了Tag，现在，我们希望能使用这些Tag。

如何使用？最好的方式自然是通过代理绑定的方式来进行。

所以，我们在AbilitysystemComponent中，新增两个函数，一个用来处理绑定，一个是绑定的回调函数

```c++
void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
    // 一但主角或者Enemy的AbilityActorInfo被设置，就代表他们的AbilitySystemComponent已经被初始化了，这时候我们就可以绑定EffectApplied委托了

    // 绑定EffectApplied委托，该委托在效果应用到目标时调用
    OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::EffectApplied);
}

void UAuraAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent,
                                                const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer AssetTagContainer;    // 创建一个GameplayTagContainer
    // 通过绑定委托，获取到GameplayTag，然后将其添加到AbilitySystemComponent的AssetTags中
    EffectSpec.GetAllAssetTags(AssetTagContainer);

    for (const FGameplayTag& Tag : AssetTagContainer)
    {
	    // TODO: 广播这些Tag到UI
        UKismetSystemLibrary::PrintString(this, Tag.ToString(), true, true, FLinearColor::Gray, 5.0f);
    }
}
```



因为Gameplay Tags依赖AbilitysystemComponent，所以绑定Tag委托的最佳时机，自然是我们创建好角色的AbilitySystem的时候。

按照我们之前的框架设计，我们的主角和敌人都基于一个基类生成，然后对于Enemy对象，我们是在他的BeginPlay函数里进行的AbilitySystem初始化，所以，对于Enemy我们就在这里进行Tag委托绑定。

![image-20240408125424524](.\image-20240408125424524.png)

然后对于我们的主角，他的ASC分服务端和客户端两种环境，这两个地方初始化AbilitySystem的函数不同，但是都调用了同一个函数，所以我们在这里进行委托绑定

![image-20240408125811969](.\image-20240408125811969.png)

其实，从上面的代码我们可以看到，虽然我们执行的委托绑定，但是我们还是封装了一个初始化的函数，顾名思义，这个函数后续也可以做一些Ability初始化内容，只是目前我们只用来绑定委托。

绑定EffectApplied委托用的不是AddDynamic，而是用AddObject。这个可以从源码看到。

