# Enemy Hit React

现在，我们希望角色在受到伤害的时候，能播放受击动画。我们有不同类型的敌人，所以自然他们的受击动画就不一样，所以，首先，我们的角色需要一个动画蒙太奇，我把它挂载在BaseCharacter基类中

```c++
UPROPERTY(EditAnywhere, Category = "Combat")
TObjectPtr<UAnimMontage> HitReactMontage;	// 受击反应动画
```

然后，我们希望蓝图能通过CombatInterface接口，获取到这个受击动画。但是我们知道，Combat作为一个接口类，我想让这个接口函数在蓝图实现，同时，我又希望他能在子类中又差异化的内容，蓝图实现，我们不能定位虚函数，那就只能用 BlueprintNativeEvent 这个函数声明了

```c++
class UAnimMontage;

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)	// 将接口类型设置为蓝图类型
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ARCANE_API ICombatInterface
{
	GENERATED_BODY()
	
public:
	virtual int32 GetPlayerLevel() const;

	virtual FVector GetCombatSocketLocation() const;	// 获取战斗插槽位置

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)		// 蓝图实现，同时蓝图可调用
	void UpdateFacingTarget(const FVector& Target);	//更新面向目标

	UFUNCTION(BlueprintNativeEvent)	// 蓝图原生事件，蓝图可以重写
	UAnimMontage* GetHitReactMontage();	// 获取受击反应动画
	
};
```

然后在子类中，我们实现这个函数的子类版本

```c++
UAnimMontage* AAuraCharacterBase::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}
```

![image-20240415125620139](.\image-20240415125620139.png)



# Activating the Enemy Hit React Ability

ok，至此，我们就将这个受击播放一个受到击打的动画蒙太奇这个效果添加进了一个GameplayEffect中，我们希望这个效果应该是每个角色都应该有的能力，所以，这里将它作为一个通用的能力

> ```c++
> UPROPERTY(EditDefaultsOnly, Category = "Common Class Defaults")
> TArray<TSubclassOf<UGameplayAbility>> CommonAbilities;	// 能力
> ```

```c++
/**
 * 角色职业信息数据资产
 */
UCLASS()
class ARCANE_API UCharacterClassInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Character Class Defaults")
	TMap<ECharacterClass, FCharacterClassDefaultInfo> CharacterClassInformation;	// 角色职业属性信息

	UPROPERTY(EditDefaultsOnly, Category="Common Class Defaults")
	TSubclassOf<UGameplayEffect> SecondaryAttributes;	// 次要属性效果, 用于所有职业

	UPROPERTY(EditDefaultsOnly, Category= "Common Class Defaults")
	TSubclassOf<UGameplayEffect> VitalAttributes;		// 重要属性效果, 用于所有职业

	FCharacterClassDefaultInfo GetCharacterClassDefaultInfo(ECharacterClass CharacterClass) const;	// 获取角色职业默认信息

	UPROPERTY(EditDefaultsOnly, Category = "Common Class Defaults")
	TArray<TSubclassOf<UGameplayAbility>> CommonAbilities;	// 能力

};
```



然后，我们回到蓝图库函数文件中，添加一个静态函数，以便调用（设置角色的通用能力组）

```c++

	// 初始化角色能力组
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|CharacterClassDefault")
	static void GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC);

```



```c++
void UAuraAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC)
{
	if (const AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject)))
	{
		for (const TSubclassOf<UGameplayAbility>& AbilityClass : AuraGameMode->CharacterClassInfo->CommonAbilities)
		{
			if (AbilityClass)
			{
				FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass.GetDefaultObject(), 1);
				ASC->GiveAbility(AbilitySpec);
			}
		}
	}
}
```



然后，回到角色类中，调用这个函数 （在BegainPlay()）

```c++
	UAuraAbilitySystemLibrary::GiveStartupAbilities(this, AbilitySystemComponent);	// 初始化能力
```

然后，如何应用呢？首先，我们做这些，目前是为了给角色添加受击动画，所以，自然要在角色受击处应用，换句话说，就是在GameplayEffect执行处 （UAuraAttributeSet::PostGameplayEffectExecute）调用

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

			// 如果不是致命的，那么就激活这个Ability
			if (!bFatal)
			{
				// 创建一个TagContainer，将Effect_HitReact标签添加进去
				FGameplayTagContainer HitReactTagContainer;
				HitReactTagContainer.AddTag(FAuraGameplayTags::Get().Effect_HitReact);	// 添加受击标签
				EffectProperties.TargetASC->TryActivateAbilitiesByTag(HitReactTagContainer);	// 尝试激活标签的能力
			}
		}
	}

}
```

这里判断是否是致命伤，如果不是，才添加受伤的受击tag，然后通过TryActivateAbilitiesByTag，依据tag激活能力实现受击动画



# Showing Damage Text  显示伤害文本

准备好一个UI组件，然后在控制器中添加该组件

```c++
	/*
	 * Damage Text
	 */
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextComponent> DamageTextComponentClass;
```

然后创建组件，设置组件连接在受伤害的角色的根组件上，然后再将组件从角色身上分离出来，然后保持世界位置

```c++


void AAuraPlayerController::ShowDamageText_Implementation(float Damage, ACharacter* Target)
{
	// 为什么这里要判断IsValid(Target)而不对DamageTextComponentClass使用IsValid?
	// IsValid除了判断指针是否为空外，还会判断指针是否有效，判断这个指针是否等待销毁（Pending Kill）
	// IsValid(Target)是为了确保我们的目标是有效的，因为我们的目标是一个角色，而角色是可以销毁的，如果我们的目标销毁了，那么我们就不应该再显示伤害文本了
	if (IsValid(Target) && DamageTextComponentClass)
	{
		UDamageTextComponent* DamageTextComponent = NewObject<UDamageTextComponent>(Target, DamageTextComponentClass);	// 创建伤害文本组件，这里是创建一个新的伤害文本组件，Target是伤害文本组件的Outer，DamageTextComponentClass是伤害文本组件的类
		DamageTextComponent->RegisterComponent();	// 注册组件（通常来说，我们创建的组件都需要注册，而之前我们使用CreateDefaultSubobject创建的组件是不需要注册的，因为在CreateDefaultSubobject的时候已经注册了）
		DamageTextComponent->AttachToComponent(Target->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);	// 将组件附加到目标的根组件上
		DamageTextComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);	// 解除附加，这里是解除附加到目标的根组件上，然后保持世界位置，这样我们的伤害文本就不会跟随目标移动了
		DamageTextComponent->SetDamageText(Damage);	// 设置伤害文本
	}
}
```

