# 火球术（Projectile-投射类型）

```c++
// Sets default values
AAuraProjectile::AAuraProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// 创建球形碰撞体，并设置为根组件，用于检测碰撞，但不产生物理效果，只用于检测，对动态物体和静态物体都产生重叠，对角色产生重叠
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SetRootComponent(Sphere);
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	// 创建投射物移动组件，并设置为球形碰撞体的移动组件
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->SetUpdatedComponent(Sphere);	// 设置更新组件
	ProjectileMovement->InitialSpeed = 550.f;	// 初始速度
	ProjectileMovement->MaxSpeed = 550.f;	// 最大速度
	ProjectileMovement->ProjectileGravityScale = 0.f;	// 投射物重力缩放

}
```



我们希望火球是通过法杖顶端生成，所以在战斗组件中，提供一个虚函数，用来获取插槽位置

```c++
virtual FVector GetCombatSocketLocation() const;	// 获取战斗插槽位置
```

然后因为武器是挂载在角色上的，所以在角色这里也添加一个获取武器插槽的函数

```c++
FVector AAuraCharacterBase::GetCombatSocketLocation() const
{
	// 获取武器尖端插槽的位置
	return Weapon->GetSocketLocation(WeaponTipSocketName);
}
```

然后，就是将我们武器插槽位置作为火球生成位置，进行法术生成

```c++
void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Activate Projectile Spell"), true, false, FLinearColor::Red, 1.0f);

	// 投射物生成，我们希望他是在服务端生成，然后在客户端同步
	const bool bIsServer = HasAuthority(&ActivationInfo);
	if (!bIsServer) return;

	// 生成位置，我不希望简单使用角色的位置，而是使用施法者武器上的插槽位置
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());

	// TODO: 设置投射物旋转，比如朝向目标

	if (CombatInterface)
	{
		const FVector SocketLocation = CombatInterface->GetCombatSocketLocation();

		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);		// 使用武器插槽位置

		// SpawnActorDeferred 是因为我们希望在生成之前设置一些属性，比如伤害，速度等
		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
			ProjectileClass,	// 投射物类
			SpawnTransform,		// 生成位置
			GetOwningActorFromActorInfo(),	// 拥有者
			Cast<APawn>(GetOwningActorFromActorInfo()),	// 控制者
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn	// 碰撞处理方式, 总是生成
		);

		// TODO: 设置投射物属性，比如伤害，速度等

		Projectile->FinishSpawning(SpawnTransform);	// 完成生成
	}

}

```



# SpawnActorDeferred 

在 Unreal Engine 5 (UE5) 中，SpawnActorDeferred 是一个函数，用于异步地创建一个 Actor 实例。这个函数通常用于在游戏逻辑中创建 Actor 实例，但不会立即执行创建操作，而是将创建操作推迟到稍后的时间执行。

以下是 SpawnActorDeferred 函数的详细说明：

- **类名**：UWorld
- **函数名**：SpawnActorDeferred
- **参数**：
  - `ActorClass`：要创建的 Actor 类的类对象。
  - `SpawnTransform`：新创建的 Actor 的初始位置和旋转。
  - `Owner`：新创建的 Actor 的拥有者。
  - `Template`：用于复制 Actor 的模板对象。
  - `Flags`：用于控制 Actor 创建行为的标志位。
  - `OutActor`：用于存储新创建的 Actor 的指针。

这个函数会异步地创建一个 Actor 实例，并将新创建的 Actor 的指针存储在 OutActor 参数中。这个函数通常用于在游戏逻辑中创建 Actor 实例，但不会立即执行创建操作，而是将创建操作推迟到稍后的时间执行。

以下是一个使用 SpawnActorDeferred 函数的示例代码：

```cpp
// 假设 ActorClass 已经被设置好
UWorld* World = GetWorld();
UActorComponent* Owner = GetOwner();
UActorComponent* Template = GetTemplate();

FActorSpawnParameters SpawnParams;
SpawnParams.Owner = Owner;
SpawnParams.Template = Template;
SpawnParams.SpawnFlags |= ESpawnActorFlags::DontInitializeComponents;

UWorld::SpawnActorDeferred<ActorClass>(World, SpawnTransform, SpawnParams, OutActor);
```

在这个示例中，我们首先获取当前世界的指针 World，然后获取当前 Actor 的拥有者 Owner 和模板对象 Template。接着，我们创建一个 FActorSpawnParameters 结构体实例 SpawnParams，并设置一些参数，比如 Owner、Template 和一些创建标志位。最后，我们调用 UWorld::SpawnActorDeferred 函数，将 Actor 类的类对象 ActorClass、新创建的 Actor 的初始位置和旋转 SpawnTransform、创建参数 SpawnParams 和新创建的 Actor 的指针 OutActor 作为参数传入。这个函数会异步地创建一个 Actor 实例，并将新创建的 Actor 的指针存储在 OutActor 参数中。