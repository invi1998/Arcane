// Copyright INVI1998


#include "Character/AuraEnemy.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Arcane/Arcane.h"
#include "Components/WidgetComponent.h"
#include "AuraGameplayTags.h"
#include "AI/AuraAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UI/Widget/AuraUserWidget.h"

AAuraEnemy::AAuraEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);	// 设置碰撞响应

	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>(TEXT("AbilitySystemComponent"));	// 创建能力系统组件
	AbilitySystemComponent->SetIsReplicated(true);	// 设置复制
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);	// 设置复制模式，对于AI来说，只需要最小的复制

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>(TEXT("AttributeSet"));	// 创建属性集

	HealthBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));	// 创建血条
	HealthBar->SetupAttachment(GetRootComponent());	// 设置父级

	bUseControllerRotationYaw = false;	// 关闭控制器旋转偏航
	bUseControllerRotationPitch = false;	// 关闭控制器旋转俯仰
	bUseControllerRotationRoll = false;	// 关闭控制器旋转翻滚

	GetCharacterMovement()->bUseControllerDesiredRotation = true;	// 开启控制器期望旋转（移动组件会根据控制器的旋转来旋转）

}

void AAuraEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (HasAuthority())
	{
		AuraAIController = Cast<AAuraAIController>(NewController);	// 设置AI控制器

		AuraAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);	// 初始化黑板

		AuraAIController->RunBehaviorTree(BehaviorTree);	// 运行行为树

		AuraAIController->GetBlackboardComponent()->SetValueAsBool("HitReacting", false);	// 设置黑板值, 是否受击反应

		AuraAIController->GetBlackboardComponent()->SetValueAsBool("RangedAttacker", CharacterClass == ECharacterClass::Ranger);	// 设置黑板值, 是否远程攻击者

		AuraAIController->GetBlackboardComponent()->SetValueAsBool("ZombieAttacker", CharacterClass == ECharacterClass::Zombie);	// 设置黑板值, 是否丧尸攻击者

		AuraAIController->GetBlackboardComponent()->SetValueAsBool("LichAttacker", CharacterClass == ECharacterClass::Lich);	// 设置黑板值, 是否巫妖攻击者

		AuraAIController->GetBlackboardComponent()->SetValueAsBool("GoblinAttacker", CharacterClass == ECharacterClass::Goblin);	// 设置黑板值, 是否哥布林攻击者

		AuraAIController->GetBlackboardComponent()->SetValueAsBool("ElementalistAttacker", CharacterClass == ECharacterClass::Elementalist);	// 设置黑板值, 是否元素师攻击者

		AuraAIController->GetBlackboardComponent()->SetValueAsBool("WarriorAttacker", CharacterClass == ECharacterClass::Warrior);	// 设置黑板值, 是否战士攻击者

		AuraAIController->GetBlackboardComponent()->SetValueAsBool("LichKingAttacker", CharacterClass == ECharacterClass::LichKing);	// 设置黑板值, 是否巫妖王攻击者

		AuraAIController->GetBlackboardComponent()->SetValueAsBool("LichGuardAttacker", CharacterClass == ECharacterClass::LichGuard);	// 设置黑板值, 是否巫妖卫士攻击者

		AuraAIController->GetBlackboardComponent()->SetValueAsBool("LichQueenAttacker", CharacterClass == ECharacterClass::LichQueen);	// 设置黑板值, 是否巫妖女王攻击者

		AuraAIController->GetBlackboardComponent()->SetValueAsBool("GhoulAttacker", CharacterClass == ECharacterClass::Ghoul);	// 设置黑板值, 是否食尸鬼攻击者

		AuraAIController->GetBlackboardComponent()->SetValueAsBool("IsAlive", true);	// 设置黑板值, 是否存活

	}
}

void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;	// 设置最大行走速度
	
	InitAbilityActorInfo();	// 初始化能力系统组件，设置拥有者和所有者

	if (HasAuthority())
	{
		UAuraAbilitySystemLibrary::GiveStartupAbilities(this, AbilitySystemComponent, CharacterClass);	// 初始化能力
	}

	// 将血条组件的WidgetController设置为Actor本身
	if (UAuraUserWidget* AuraUserWidget = Cast<UAuraUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		// 这样，WidgetController就是拥有这个Widget的Actor本身，为什么可以这样设置呢？
		// 因为UAuraUserWidgetController继承自UObject，而UObject是一个基类，任何继承自UObject的类都可以设置为WidgetController
		// 在UE里，任何类都继承自UObject，所以我们可以设置任何类为WidgetController
		AuraUserWidget->SetWidgetController(this);
	}

	if (const UAuraAttributeSet* AuraAS = Cast<UAuraAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAS->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)->void
			{
				OnHealthChanged.Broadcast(Data.NewValue);	// 广播生命值改变
			}
		);	// 添加生命值改变委托
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAS->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)->void
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);	// 广播最大生命值改变
			}
		);	// 添加最大生命值改变委托
		
		AbilitySystemComponent->RegisterGameplayTagEvent(FAuraGameplayTags::Get().Effect_HitReact, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AAuraEnemy::HitReactTagChanged);	// 注册死亡标签改变委托

		// 直接在这里广播一次，作为血条的初始化
		OnHealthChanged.Broadcast(AuraAS->GetHealth());	// 广播生命值改变
		OnMaxHealthChanged.Broadcast(AuraAS->GetMaxHealth());	// 广播最大生命值改变
	}
}

void AAuraEnemy::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bIsHitReact = NewCount > 0;	// 是否受击反应

	// 受击，停止移动
	GetCharacterMovement()->MaxWalkSpeed = bIsHitReact ? 0.f : BaseWalkSpeed;	// 如果受击，速度为0，否则为原来的速度

	if (AuraAIController && AuraAIController->GetBlackboardComponent())
	{
		AuraAIController->GetBlackboardComponent()->SetValueAsBool("HitReacting", bIsHitReact);	// 设置黑板值，是否受击反应
	}
	
}

void AAuraEnemy::Die()
{
	if (AuraAIController && AuraAIController->GetBlackboardComponent())
	{
		AuraAIController->GetBlackboardComponent()->SetValueAsBool("IsAlive", false);	// 设置黑板值, 是否存活
	}
	
	SetLifeSpan(LifeSpan);
	Super::Die();
}

void AAuraEnemy::SetCombatTarget_Implementation(AActor* NewTarget)
{
	CombatTarget = NewTarget;
}

AActor* AAuraEnemy::GetCombatTarget_Implementation() const
{
	return CombatTarget;
}

void AAuraEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);	// 初始化能力系统组件，设置拥有者和所有者

	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();	// 设置技能Actor信息

	if (HasAuthority())
	{
		InitializeDefaultAttributes();
	}
}

void AAuraEnemy::InitializeDefaultAttributes() const
{
	// 通过角色职业和等级初始化角色属性
	UAuraAbilitySystemLibrary::InitCharacterAttributesByClassAndLevel(this, CharacterClass, Level, AbilitySystemComponent);
}

void AAuraEnemy::HighlightActor()
{
	// 通过设置后处理材质来高亮，后处理材质主要通过渲染自定义深度来实现

	GetMesh()->SetRenderCustomDepth(true);	// 设置自定义深度渲染
	GetMesh()->CustomDepthStencilValue = CUSTOM_DEPTH_STENCIL_RED;	// 设置自定义深度值

	if (Weapon)
	{
		Weapon->SetRenderCustomDepth(true);	// 设置自定义深度渲染
		Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_STENCIL_RED);	// 设置自定义深度值
	}
	if (LeftWeapon)
	{
		LeftWeapon->SetRenderCustomDepth(true);	// 设置自定义深度渲染
		LeftWeapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_STENCIL_RED);	// 设置自定义深度值
	}
	if (RightWeapon)
	{
		RightWeapon->SetRenderCustomDepth(true);	// 设置自定义深度渲染
		RightWeapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_STENCIL_RED);	// 设置自定义深度值
	}
	if (BowWeapon)
	{
		BowWeapon->SetRenderCustomDepth(true);	// 设置自定义深度渲染
		BowWeapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_STENCIL_RED);	// 设置自定义深度值
	}
	if (BowArrow)
	{
		BowArrow->SetRenderCustomDepth(true);	// 设置自定义深度渲染
		BowArrow->SetCustomDepthStencilValue(CUSTOM_DEPTH_STENCIL_RED);	// 设置自定义深度值
	}

}

void AAuraEnemy::UnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);	// 取消自定义深度渲染
	if (Weapon)
	{
		Weapon->SetRenderCustomDepth(false);	// 取消自定义深度渲染
	}
	if (LeftWeapon)
	{
		LeftWeapon->SetRenderCustomDepth(false);	// 设置自定义深度渲染
	}
	if (RightWeapon)
	{
		RightWeapon->SetRenderCustomDepth(false);	// 设置自定义深度渲染
	}
	if (BowWeapon)
	{
		BowWeapon->SetRenderCustomDepth(false);	// 设置自定义深度渲染
	}
	if (BowArrow)
	{
		BowArrow->SetRenderCustomDepth(false);	// 设置自定义深度渲染
	}
}

int32 AAuraEnemy::GetCharacterLevel_Implementation() const
{
	return Level;
}
