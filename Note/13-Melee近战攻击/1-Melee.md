# Melee Attack

我们当前的游戏设计是想为我们的角色添加初始能力，我们之前已经有了一个默认GE

```c++

// 默认角色职业信息
USTRUCT(BlueprintType)
struct FCharacterClassDefaultInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category="Class Defaults")
	TSubclassOf<UGameplayEffect> PrimaryAttributes;	// 主属性效果
};

```

但是GE是游戏效果，类似于，受击反馈，死亡反馈，现在我们希望角色还能拥有一些默认的（可装配的）技能（能力），类似火球术这样，对于近战，我们希望每个近战都能拥有一个普通技能。

所以，我们给默认角色的初始信息添加一个能力数组，用来存储角色的默认能力

```c++

// 默认角色职业信息
USTRUCT(BlueprintType)
struct FCharacterClassDefaultInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category="Class Defaults")
	TSubclassOf<UGameplayEffect> PrimaryAttributes;	// 主属性效果

	UPROPERTY(EditAnywhere, Category="Class Defaults")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;	// 初始能力
};

```

然后就是近战攻击，我们先在Tag标签中添加一个新的标签，将其分类到Abilities这个新的标签类别下

```c++
	FGameplayTag Abilities_Attack;				// 能力：攻击

	/*
	 * Ability Tags
	 */
	GameplayTags.Abilities_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Attack"),
		FString("Attack Ability")
	);

```

然后，回到我们的蓝图函数库中，我们此前添加了一个函数，用来给角色初始化默认能力，我们将这个函数改动一下，因为此前那些默认能力都是通用的，是每一个生成在场景中的Character都会有的，所以我们没做职业划分，现在，对于技能（Ability）我们希望它能按照职业进行初始化，所以我给函数新增一个ClassInfo参数

```c++
	// 初始化角色能力组
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|CharacterClassDefault")
	static void GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass);

```

```c++
void UAuraAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass)
{
	if (const UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject))
	{
		for (const TSubclassOf<UGameplayAbility>& AbilityClass : CharacterClassInfo->CommonAbilities)
		{
			if (AbilityClass)
			{
				FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass.GetDefaultObject(), 1);
				ASC->GiveAbility(AbilitySpec);
			}
		}

		const FCharacterClassDefaultInfo ClassDefaultInfo = CharacterClassInfo->GetCharacterClassDefaultInfo(CharacterClass);
		const ICombatInterface* CombatInterface = Cast<ICombatInterface>(ASC->GetAvatarActor());
		for (TSubclassOf<UGameplayAbility> AbilityClass : ClassDefaultInfo.StartupAbilities)
		{
			if (AbilityClass && CombatInterface)
			{
				FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass.GetDefaultObject(), CombatInterface->GetPlayerLevel());
				ASC->GiveAbility(AbilitySpec);
			}
		}
	}
}
```

然后编译，回到蓝图，我们新建一个GA，（GA_MelleAttack近战攻击），里面就先做一件事，就是将Tag指定为Attack。

![image-20240420220641818](.\image-20240420220641818.png)

然后回到DA_CharacterClassInfo（角色职业信息表），将我们的这项新能力添加到战士职业中，赋予战士近战攻击的能力

![image-20240420220850178](.\image-20240420220850178.png)

至此，我们的战士角色就有了近战攻击能力，但是这个能力还没有实际实现，下一步就是实现近战攻击。