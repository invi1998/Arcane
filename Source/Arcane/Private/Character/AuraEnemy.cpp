// Copyright INVI1998


#include "Character/AuraEnemy.h"

#include "Arcane/Arcane.h"

AAuraEnemy::AAuraEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);	// 设置碰撞响应
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

}

void AAuraEnemy::UnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);	// 取消自定义深度渲染
	if (Weapon)
	{
		Weapon->SetRenderCustomDepth(false);	// 取消自定义深度渲染
	}
}
