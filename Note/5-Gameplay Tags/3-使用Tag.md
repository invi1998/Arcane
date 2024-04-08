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



# UI Widget Data Table

我们希望将Tag图表配置化，包括可以表格化他的图标，控件等，所以我们添加一个表格行结构体

```c++
// 这结构体用于存储UI小部件行的数据，用来在屏幕上显示消息
USTRUCT(BlueprintType)		// 设置为蓝图类型
struct FUIWidgetRow : public FTableRowBase 	// UI小部件行，继承自FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)		// 设置为可编辑的任何地方，蓝图可读
	FGameplayTag MessageTag = FGameplayTag::EmptyTag;		// 消息标签

	FText MessageText = FText::GetEmpty();		// 消息文本

	UPROPERTY(EditAnywhere, BlueprintReadOnly)		// 设置为可编辑的任何地方，蓝图可读
	TSubclassOf<UAuraUserWidget> MessageWidgetClass;		// 小部件类，这个部件我们可以在蓝图中任意定制，比如显示文本，图片等等

	UPROPERTY(EditAnywhere, BlueprintReadOnly)		// 设置为可编辑的任何地方，蓝图可读
	UTexture2D* MessageIcon = nullptr;		// 消息图标
};
```

然后我们在编辑器中，创建一个数据表格，行选择我们新写的这个行数据结构体

![image-20240408163214404](.\image-20240408163214404.png)

![image-20240408164501645](.\image-20240408164501645.png)

同时，为了让我们的Widget控件能表格数据化，我们再给Widget类添加一个指向数据表格的指针成员。如下：

```c++
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Widget Data")		// 设置为可编辑的任何地方，蓝图可读
	TObjectPtr<UDataTable> MessageWidgetDataTable;		// 消息小部件数据表
```

![image-20240408164608255](.\image-20240408164608255.png)



## Broadcast Data Row

在我们的 Widget 类的回调绑定这里，我们想将我们的消息Widget广播出去，所以这里我们需要检索所有Tag，然后拿到MessageTag，这样就涉及到了GameplayEffect里的匹配方式了。

```c++
void UOverlayWidgetController::BindCallbacksToDependencies()
{
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(this->AttributeSet);	// 将AttributeSet转换为UAuraAttributeSet

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetHealthAttribute()).AddUObject(this, &UOverlayWidgetController::HealthChanged);	// 添加生命值改变的委托
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxHealthAttribute()).AddUObject(this, &UOverlayWidgetController::MaxHealthChanged);	// 添加最大生命值改变的委托
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetManaAttribute()).AddUObject(this, &UOverlayWidgetController::ManaChanged);	// 添加法力值改变的委托
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxManaAttribute()).AddUObject(this, &UOverlayWidgetController::MaxManaChanged);	// 添加最大法力值改变的委托

	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTags.AddLambda(
		[this](const FGameplayTagContainer& AssertTags)
		{
			for (const FGameplayTag& Tag : AssertTags)
			{
				// 这里我们只要MessageTag
				FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));	// 获取Message
				if (Tag.MatchesTag(MessageTag))
				{
					// UKismetSystemLibrary::PrintString(GEngine->GetWorld(), Tag.ToString(), true, true, FLinearColor::Green, 5.0f);	// 打印Tag

					const FUIWidgetRow* WidgetRow = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);	// 通过Tag获取数据表行
					// 我们希望通过Tag来获取数据表行，然后使用这些数据来更新UI，比如使用它里面的一些资产来显示内容

					MessageWidgetRowDelegate.Broadcast(*WidgetRow);	// 广播消息小部件行
				}
			}
		}
	);
}
```

### Match

在虚幻引擎（Unreal Engine）的Gameplay Tag系统中，GameplayTags被设计用来标记游戏中的各种对象和行为，以便于分类、查询和响应。GameplayTag系统提供了多种匹配（Match）方式，以便根据不同的需求来查找或判断GameplayTag集合中的标签。以下是几种主要的匹配方式及其区别和用法：

1. **Exact Match（精确匹配）**
   - 顾名思义，精确匹配要求GameplayTag必须与查询的标签完全一致。例如，如果你有一个GameplayTagContainer包含了`Status.Buff.Healing`，而你正在寻找`Status.Buff.Healing`，那么精确匹配就会成功。

2. **Partial Match（部分匹配）**
   - 部分匹配允许你查找包含特定前缀的GameplayTags。比如，如果你查找`Status.Buff.*`，那么任何以`Status.Buff`开头的GameplayTag（如`Status.Buff.Healing`或`Status.Buff.Strength`）都会匹配成功。

3. **Wildcard Match（通配符匹配）**
   - 通配符匹配允许使用`*`作为任意字符的占位符。例如，`Status.*.Healing`会匹配所有中间有一段任意文本但结尾是`.Healing`的GameplayTag。

4. **Complex Match（复杂匹配）**
   - 虚幻引擎提供了`FGameplayTagQuery`类来实现更复杂的匹配条件，例如：
     - 包含关系：可以检查GameplayTagContainer是否至少包含一组特定的GameplayTags。
     - 排除关系：可以排除掉指定的一组GameplayTags。
     - 任意数量的关系：可以设置匹配GameplayTags的数量限制，如至少匹配N个标签。

5. **Named Requirements Match**
   - 在`FGameplayTagQuery`中可以定义命名要求，每一个命名要求可以是一个单独的匹配条件，这些条件可以是AND（所有要求必须满足）或OR（只要满足任何一个要求即可）的关系。

6. **Parent/Child Relationship Matching**
   - GameplayTags具有父子层级结构，可以匹配某个GameplayTag及其所有子标签。例如，匹配`Status.Buff`时，所有以`Status.Buff`开头的子标签都会被认为匹配上了。

每种匹配方式都有其特定的使用场合，例如在处理游戏内效果、判定角色状态、触发特定事件或筛选游戏对象时，根据具体情况选择合适的匹配方式能够有效提高代码的灵活性和可维护性。



至此，我们就将MessageTag内容广播出去了，现在我们只需要绑定这个委托，就能实现在屏幕上显示Message了。

这里先做一个简单的蓝图，将Tag里的message打印到屏幕上，来分配委托事务；

![image-20240408202621900](.\image-20240408202621900.png)



