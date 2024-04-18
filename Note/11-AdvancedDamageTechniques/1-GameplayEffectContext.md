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