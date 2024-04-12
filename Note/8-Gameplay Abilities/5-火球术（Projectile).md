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

	// 投射物生成，我们希望他是在服务端生成，然后在客户端同步
	const bool bIsServer = HasAuthority(&ActivationInfo);
	if (!bIsServer) return;

	// 生成位置，我不希望简单使用角色的位置，而是使用施法者武器上的插槽位置
	// 等价于 Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(ActorInfo->AvatarActor);

	if (CombatInterface)
	{
		const FVector SocketLocation = CombatInterface->GetCombatSocketLocation();

		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);		// 使用武器插槽位置

		GetWorld()->SpawnActorDeferred<AAuraProjectile>(
			ProjectileClass,	// 投射物类
			SpawnTransform,		// 生成位置
			GetOwningActorFromActorInfo(),	// 拥有者
			Cast<APawn>(GetOwningActorFromActorInfo()),	// 控制者
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn	// 碰撞处理方式, 总是生成
		);
	}

}

```

