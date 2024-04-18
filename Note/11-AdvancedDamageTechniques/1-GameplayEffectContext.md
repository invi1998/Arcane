# The Gameplay Effect Context

## MakeEffectContext

`MakeEffectContext` 是虚幻引擎（Unreal Engine）中用于创建游戏效果上下文（Gameplay Effect Context）的函数。游戏效果上下文是在游戏效果应用到游戏对象时使用的上下文，它包含了与应用游戏效果相关的各种信息，如应用者、目标、捕捉的属性值等。

`MakeEffectContext` 函数通常在游戏效果的执行计算（Execution Calculations）中使用，用于创建一个游戏效果上下文对象。这个上下文对象可以传递给游戏效果的执行计算函数，以便在执行计算过程中使用。

例如，以下是一个使用 `MakeEffectContext` 创建游戏效果上下文的例子：

```cpp
// 创建游戏效果上下文
FGameplayEffectContext* EffectContext = MakeEffectContext();

// 使用游戏效果上下文进行计算
float NewHealth = EffectContext->GetCaptureAttributeValue(FAbilityAttributeCapture, EAttributeCaptureMethod::CurrentValue, UHealthAttributeComponent::StaticClass());

// 应用游戏效果
UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponent();
AbilitySystemComponent->ApplyGameplayEffectToTarget(HealthGameplayEffect, EffectContext, TargetActor);
```

在这个例子中，我们首先使用 `MakeEffectContext` 创建了一个游戏效果上下文对象。然后，我们在游戏效果的执行计算过程中使用这个上下文对象来获取属性值。最后，我们将游戏效果应用到目标对象上，同时传递了游戏效果上下文对象，以便在应用游戏效果时使用。



我们之前的技能，已经可以计算出格挡，计算出暴击。现在，我们需要根据是否格挡，是否暴击，在界面上有所表现。这也就是说，我们需要拿到这个只出现在计算过程中的效果。而目前的困境就是，所有的这些效果是否生效，结果只在Execute里才能知道

所以，为了将这些计算过程从Execute中带出来，我们创建一个GameplayEffectContext子类，在子类中新增这些成员变量，通过效果上下文，传递给 `void UAuraAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)`，因为在Data数据里，有上下文信息，这里也是一次攻击结算的最后调用（Post阶段），然后我们在这里拿到是否暴击，是否格挡，就能在界面做出相对应的响应。

> `FGameplayEffectModCallbackData`下的 `FGameplayEffectSpec`成员，这里面有`FGameplayEffectContextHandle EffectContext; `上下文信息，所有效果相关的信息都能通过它来传递。

```c++
#pragma once
#include "GameplayEffectTypes.h"

class UAuraAbilitySystemComponent;

USTRUCT(BlueprintType)
struct FAuraGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:
	
	bool IsCriticalHit() const { return bIsCriticalHit; }
	bool IsBlockedHit() const { return bIsBlockedHit; }

	void SetCriticalHit(bool bInIsCriticalHit) { bIsCriticalHit = bInIsCriticalHit; }
	void SetBlockedHit(bool bInIsBlockedHit) { bIsBlockedHit = bInIsBlockedHit; }

	// 获取脚本结构
	virtual UScriptStruct* GetScriptStruct() const override;

	// 网络序列化。这里决定了这个结构体在网络中如何序列化
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;

protected:

	UPROPERTY()
	bool bIsBlockedHit = false;		// 是否格挡

	UPROPERTY()
	bool bIsCriticalHit = false;	// 是否暴击
	
};

```

