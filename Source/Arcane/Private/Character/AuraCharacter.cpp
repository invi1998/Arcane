// Copyright INVI1998


#include "Character/AuraCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"

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
