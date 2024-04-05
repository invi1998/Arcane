// Copyright INVI1998


#include "Character/AuraEnemy.h"

#include "Arcane/Arcane.h"

AAuraEnemy::AAuraEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);	// ������ײ��Ӧ
}

void AAuraEnemy::HighlightActor()
{
	// ͨ�����ú�����������������������Ҫͨ����Ⱦ�Զ��������ʵ��

	GetMesh()->SetRenderCustomDepth(true);	// �����Զ��������Ⱦ
	GetMesh()->CustomDepthStencilValue = CUSTOM_DEPTH_STENCIL_RED;	// �����Զ������ֵ

	if (Weapon)
	{
		Weapon->SetRenderCustomDepth(true);	// �����Զ��������Ⱦ
		Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_STENCIL_RED);	// �����Զ������ֵ
	}

}

void AAuraEnemy::UnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);	// ȡ���Զ��������Ⱦ
	if (Weapon)
	{
		Weapon->SetRenderCustomDepth(false);	// ȡ���Զ��������Ⱦ
	}
}
