// Copyright INVI1998


#include "Character/AuraCharacter.h"

#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"
#include "AbilitySystem/Debuff/DebuffNiagaraComponent.h"
#include "NiagaraComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/HUD/AuraHUD.h"

AAuraCharacter::AAuraCharacter()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));	// 创建相机摇臂
	CameraBoom->SetupAttachment(RootComponent);	// 设置相机摇臂附加到根组件
	CameraBoom->SetUsingAbsoluteRotation(true);	// 设置使用绝对旋转
	CameraBoom->bDoCollisionTest = false;	// 关闭碰撞测试

	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCameraComponent"));	// 创建顶视摄像机组件
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);	// 设置顶视摄像机组件附加到相机摇臂
	TopDownCameraComponent->bUsePawnControlRotation = false;	// 关闭使用Pawn控制旋转

	GetCharacterMovement()->bOrientRotationToMovement = true;	// 开启移动时旋转
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 400.0f, 0.0f);	// 设置旋转速率
	GetCharacterMovement()->bConstrainToPlane = true;	// 开启限制移动到平面，这样我们就可以在水平面上移动，而不是在空中移动
	GetCharacterMovement()->bSnapToPlaneAtStart = true;	// 开启在开始时就限制移动到平面

	bUseControllerRotationPitch = false;	// 关闭控制器旋转俯仰, 这样我们就不能抬起或者低下头部，只能左右旋转，这对于俯视角游戏来说是很好的
	bUseControllerRotationYaw = false;	// 关闭控制器旋转偏航, 这样我们就不能左右旋转
	bUseControllerRotationRoll = false;	// 关闭控制器旋转翻滚, 这样我们就不能翻滚

	CharacterClass = ECharacterClass::Elementalist;	// 设置角色职业

	LevelUpEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("LevelUpEffect"));	// 创建升级特效
	LevelUpEffect->SetupAttachment(RootComponent);	// 设置特效附加到根组件
	LevelUpEffect->bAutoActivate = false;	// 关闭自动激活
}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	// 该函数只会在服务器端被调用
	// PossessedBy()函数通常在服务器端被调用。当一个Controller（如PlayerController或AIController）开始控制一个Pawn（如PlayerCharacter或AICharacter）时，服务器会调用PossessedBy()函数来设置Pawn的Controller属性。
	Super::PossessedBy(NewController);

	// 为服务器初始化AbilitySystemComponent
	InitAbilityActorInfo();

	// 初始化角色能力
	AddCharacterAbilities();
	
}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// 为客户端初始化AbilitySystemComponent
	InitAbilityActorInfo();
}

int32 AAuraCharacter::GetCharacterLevel_Implementation() const
{
	// 获取玩家状态
	if (const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>())
	{
		return AuraPlayerState->GetPlayerLevel();
	}
	return 0;
}

void AAuraCharacter::Die(const FVector& DeathImpulse)
{
	Super::Die(DeathImpulse);


}

void AAuraCharacter::AddToEXP_Implementation(int32 EXP)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->AddEXP(EXP);
	const int32 Level = AuraPlayerState->GetPlayerLevel();
	const int32 TotalEXP = AuraPlayerState->GetPlayerEXP();
	const int32 MatchedLevel = AuraPlayerState->LevelUpInfo->GetLevelByExp(TotalEXP);
	if (MatchedLevel > Level)
	{
		if (IsValid(AttributeSet))
		{
			UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
			AuraAttributeSet->SetTopOfHealth(true);	// 设置满血
			AuraAttributeSet->SetTopOfMana(true);	// 设置满蓝
		}
		Execute_LevelUp(this, MatchedLevel - Level);	// 升级
	}
}

void AAuraCharacter::SetEXP_Implementation(int32 EXP)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->SetEXP(EXP);
	const int32 Level = AuraPlayerState->GetPlayerLevel();
	const int32 TotalEXP = AuraPlayerState->GetPlayerEXP();
	const int32 MatchedLevel = AuraPlayerState->LevelUpInfo->GetLevelByExp(TotalEXP);
	if (MatchedLevel != Level)
	{
		Execute_SetLevel(this, MatchedLevel);	// 设置等级
	}
}

void AAuraCharacter::SetLevel_Implementation(int32 Lv)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->SetLevel(Lv);
	if (Lv > 0)
	{
		MulticastLevelUpEffect();	// 多播升级特效

		const int32 MatchedLevel = AuraPlayerState->GetPlayerLevel();
		// 获取奖励的属性点数
		const int32 CulAttributePoints = AuraPlayerState->LevelUpInfo->GetTotalAttributePointRewardInLevel(MatchedLevel);
		if (CulAttributePoints > 0)
		{
			// 调用AddAttributePoint的Execute版本
			Execute_AddAttributePoint(this, CulAttributePoints);
		}

		// 获取奖励的技能点数
		const int32 CulSkillPoints = AuraPlayerState->LevelUpInfo->GetTotalSkillPointRewardInLevel(MatchedLevel);
		if (CulSkillPoints > 0)
		{
			Execute_AddSkillPoint(this, CulSkillPoints);
		}
	}
	// 更新角色能力状态
	if (UAuraAbilitySystemComponent* AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
	{
		AuraAbilitySystemComponent->UpdateAbilityStateTags(AuraPlayerState->GetPlayerLevel());
	}
}

void AAuraCharacter::LevelUp_Implementation(int32 Lv)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	for (int32 i = 0; i < Lv; i++)
	{
		AuraPlayerState->AddLevel(1);
		const int32 MatchedLevel = AuraPlayerState->GetPlayerLevel();
		// 获取奖励的属性点数
		const int32 CulAttributePoints = AuraPlayerState->LevelUpInfo->GetAttributePointRewardByLevel(MatchedLevel);
		if (CulAttributePoints > 0)
		{
			Execute_AddAttributePoint(this, CulAttributePoints);
		}

		// 获取奖励的技能点数
		const int32 CulSkillPoints = AuraPlayerState->LevelUpInfo->GetSkillPointRewardByLevel(MatchedLevel);
		if (CulSkillPoints > 0)
		{
			Execute_AddSkillPoint(this, CulSkillPoints);
		}
	}

	if (Lv > 0)
	{
		MulticastLevelUpEffect();	// 多播升级特效
		// 更新角色能力状态
		if (UAuraAbilitySystemComponent* AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
		{
			AuraAbilitySystemComponent->UpdateAbilityStateTags(AuraPlayerState->GetPlayerLevel());
		}
	}
}

int32 AAuraCharacter::GetEXP_Implementation() const
{
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->GetPlayerEXP();
}

int32 AAuraCharacter::GetLevelByEXP_Implementation(int32 EXP) const
{
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->LevelUpInfo->GetLevelByExp(EXP);
}

int32 AAuraCharacter::GetAttributePointReward_Implementation() const
{
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->LevelUpInfo->GetAttributePointRewardByLevel(AuraPlayerState->GetPlayerLevel());
}

int32 AAuraCharacter::GetSkillPointReward_Implementation() const
{
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->LevelUpInfo->GetSkillPointRewardByLevel(AuraPlayerState->GetPlayerLevel());
}

void AAuraCharacter::AddAttributePoint_Implementation(int32 Point)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->AddAttributePoint(Point);
}

void AAuraCharacter::AddSkillPoint_Implementation(int32 Point)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->AddSkillPoint(Point);
}

int32 AAuraCharacter::GetAttributePoint_Implementation() const
{
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->GetAttributePoints();
}

int32 AAuraCharacter::GetSkillPoint_Implementation() const
{
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->GetSkillPoints();
}

void AAuraCharacter::OnRep_Stunned()
{
	if (UAuraAbilitySystemComponent* AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
	{
		const FAuraGameplayTags& AuraTags = FAuraGameplayTags::Get();

		//FInheritedTagContainer InheritedTags;	// 创建继承标签容器
		//InheritedTags.AddTag(AuraTags.Player_Block_CursorTrace);	// 禁止鼠标追踪
		//InheritedTags.AddTag(AuraTags.Player_Block_InputHeld);	// 禁止输入保持
		//InheritedTags.AddTag(AuraTags.Player_Block_InputPressed);	// 禁止输入按下
		//InheritedTags.AddTag(AuraTags.Player_Block_InputReleased);	// 禁止输入释放

		//const FString DebuffName = FString::Printf(TEXT("DynamicDebuff_%s"), *AuraTags.Debuff_LightningStun.ToString());	 // 获取Debuff名称
		//UGameplayEffect* DebuffEffect = NewObject<UGameplayEffect>(GetTransientPackage(), FName(*DebuffName));	// 创建Debuff效果
		//UTargetTagsGameplayEffectComponent& Component = DebuffEffect->FindOrAddComponent<UTargetTagsGameplayEffectComponent>();

		//Component.SetAndApplyTargetTagChanges(InheritedTags);	// 添加继承标签容器

		//if (bIsStunned)
		//{
		//	AuraAbilitySystemComponent->AddLooseGameplayTag(AuraTags.Debuff_LightningStun);	// 添加眩晕标签
		//}
		//else
		//{
		//	AuraAbilitySystemComponent->RemoveLooseGameplayTag(AuraTags.Debuff_LightningStun);	// 移除眩晕标签
		//}

		FGameplayTagContainer BlockTags;	// 创建阻挡标签容器
		BlockTags.AddTag(AuraTags.Player_Block_CursorTrace);	// 添加阻挡标签
		BlockTags.AddTag(AuraTags.Player_Block_InputHeld);	// 添加阻挡标签
		BlockTags.AddTag(AuraTags.Player_Block_InputPressed);	// 添加阻挡标签
		BlockTags.AddTag(AuraTags.Player_Block_InputReleased);	// 添加阻挡标签

		if (bIsStunned)
		{
			AuraAbilitySystemComponent->AddLooseGameplayTags(BlockTags);	// 添加阻挡标签
			StunDebuffEffect->Activate();	// 激活眩晕Debuff特效)
		}
		else
		{
			AuraAbilitySystemComponent->RemoveLooseGameplayTags(BlockTags);	// 移除阻挡标签
			StunDebuffEffect->Deactivate();	// 停止眩晕Debuff特效
		}
	}
}
;
void AAuraCharacter::InitAbilityActorInfo()
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuraPlayerState, this);	// 初始化技能系统组件
	Cast<UAuraAbilitySystemComponent>(AuraPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();	// 设置技能Actor信息
	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();	// 获取技能系统组件
	AttributeSet = AuraPlayerState->GetAttributeSet();	// 获取属性集

	// 绑定角色眩晕标签改变委托
	AbilitySystemComponent->RegisterGameplayTagEvent(FAuraGameplayTags::Get().Debuff_LightningStun, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AAuraCharacter::StunTagChanged);

	OnASCRegistered.Broadcast(AbilitySystemComponent);	// 广播ASC注册委托

	// 很多人纠结到底在什么情况下应该断言PlayerController*，什么时候只需要判断指针是否为空？
	// 因为我们这个是一个多人游戏，而控制器在客户端只有自己角色的控制器，其他角色的控制器是空的，服务端有所有角色的控制器。
	// 所以控制器这个指针是会存在空的情况的，这是正常的，说明这个控制器不是自己的控制器。
	// 所以这种情况下我们只需要判断指针是否为空就可以了，不需要断言。
	if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(AuraPlayerController->GetHUD()))
		{
			AuraHUD->InitOverlay(AuraPlayerController, AuraPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}

	InitializeDefaultAttributes();	// 初始化主要能力

}

void AAuraCharacter::MulticastLevelUpEffect_Implementation() const
{
	if (IsValid(LevelUpEffect))
	{
		const FVector CameraLocation = TopDownCameraComponent->GetComponentLocation();	// 获取相机位置
		const FVector NiagaraLocation = LevelUpEffect->GetComponentLocation();	// 获取特效位置
		// 让特效朝向摄像机
		const FRotator LookAtRotator = FRotationMatrix::MakeFromX(CameraLocation - NiagaraLocation).Rotator();
		LevelUpEffect->SetWorldRotation(LookAtRotator);	// 设置特效旋转
		LevelUpEffect->Activate(true);	// 激活特效
	}
}

