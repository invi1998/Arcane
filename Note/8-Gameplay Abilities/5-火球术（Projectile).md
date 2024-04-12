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

