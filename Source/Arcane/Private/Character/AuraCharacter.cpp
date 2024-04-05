// Copyright INVI1998


#include "Character/AuraCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"

AAuraCharacter::AAuraCharacter()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;	// �����ƶ�ʱ��ת
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 400.0f, 0.0f);	// ������ת����
	GetCharacterMovement()->bConstrainToPlane = true;	// ���������ƶ���ƽ�棬�������ǾͿ�����ˮƽ�����ƶ����������ڿ����ƶ�
	GetCharacterMovement()->bSnapToPlaneAtStart = true;	// �����ڿ�ʼʱ�������ƶ���ƽ��

	bUseControllerRotationPitch = false;	// �رտ�������ת����, �������ǾͲ���̧����ߵ���ͷ����ֻ��������ת������ڸ��ӽ���Ϸ��˵�Ǻܺõ�
	bUseControllerRotationYaw = false;	// �رտ�������תƫ��, �������ǾͲ���������ת
	bUseControllerRotationRoll = false;	// �رտ�������ת����, �������ǾͲ��ܷ���
}
