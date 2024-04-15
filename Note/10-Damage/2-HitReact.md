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