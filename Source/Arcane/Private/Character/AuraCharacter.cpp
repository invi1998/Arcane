// Copyright INVI1998


#include "Character/AuraCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"

AAuraCharacter::AAuraCharacter()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;	// 开启移动时旋转
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 400.0f, 0.0f);	// 设置旋转速率
	GetCharacterMovement()->bConstrainToPlane = true;	// 开启限制移动到平面，这样我们就可以在水平面上移动，而不是在空中移动
	GetCharacterMovement()->bSnapToPlaneAtStart = true;	// 开启在开始时就限制移动到平面

	bUseControllerRotationPitch = false;	// 关闭控制器旋转俯仰, 这样我们就不能抬起或者低下头部，只能左右旋转，这对于俯视角游戏来说是很好的
	bUseControllerRotationYaw = false;	// 关闭控制器旋转偏航, 这样我们就不能左右旋转
	bUseControllerRotationRoll = false;	// 关闭控制器旋转翻滚, 这样我们就不能翻滚
}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	// 该函数只会在服务器端被调用
	// PossessedBy()函数通常在服务器端被调用。当一个Controller（如PlayerController或AIController）开始控制一个Pawn（如PlayerCharacter或AICharacter）时，服务器会调用PossessedBy()函数来设置Pawn的Controller属性。
	Super::PossessedBy(NewController);

	// 为服务器初始化AbilitySystemComponent
	InitAbilityActorInfo();
	
}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// 为客户端初始化AbilitySystemComponent
	InitAbilityActorInfo();
}

void AAuraCharacter::InitAbilityActorInfo()
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuraPlayerState, this);	// 初始化技能系统组件
	Cast<UAuraAbilitySystemComponent>(AuraPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();	// 设置技能Actor信息
	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();	// 获取技能系统组件
	AttributeSet = AuraPlayerState->GetAttributeSet();	// 获取属性集

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

}
