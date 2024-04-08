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
	GetCharacterMovement()->bOrientRotationToMovement = true;	// �����ƶ�ʱ��ת
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 400.0f, 0.0f);	// ������ת����
	GetCharacterMovement()->bConstrainToPlane = true;	// ���������ƶ���ƽ�棬�������ǾͿ�����ˮƽ�����ƶ����������ڿ����ƶ�
	GetCharacterMovement()->bSnapToPlaneAtStart = true;	// �����ڿ�ʼʱ�������ƶ���ƽ��

	bUseControllerRotationPitch = false;	// �رտ�������ת����, �������ǾͲ���̧����ߵ���ͷ����ֻ��������ת������ڸ��ӽ���Ϸ��˵�Ǻܺõ�
	bUseControllerRotationYaw = false;	// �رտ�������תƫ��, �������ǾͲ���������ת
	bUseControllerRotationRoll = false;	// �رտ�������ת����, �������ǾͲ��ܷ���
}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	// �ú���ֻ���ڷ������˱�����
	// PossessedBy()����ͨ���ڷ������˱����á���һ��Controller����PlayerController��AIController����ʼ����һ��Pawn����PlayerCharacter��AICharacter��ʱ�������������PossessedBy()����������Pawn��Controller���ԡ�
	Super::PossessedBy(NewController);

	// Ϊ��������ʼ��AbilitySystemComponent
	InitAbilityActorInfo();
	
}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Ϊ�ͻ��˳�ʼ��AbilitySystemComponent
	InitAbilityActorInfo();
}

void AAuraCharacter::InitAbilityActorInfo()
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuraPlayerState, this);	// ��ʼ������ϵͳ���
	Cast<UAuraAbilitySystemComponent>(AuraPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();	// ���ü���Actor��Ϣ
	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();	// ��ȡ����ϵͳ���
	AttributeSet = AuraPlayerState->GetAttributeSet();	// ��ȡ���Լ�

	// �ܶ��˾��ᵽ����ʲô�����Ӧ�ö���PlayerController*��ʲôʱ��ֻ��Ҫ�ж�ָ���Ƿ�Ϊ�գ�
	// ��Ϊ���������һ��������Ϸ�����������ڿͻ���ֻ���Լ���ɫ�Ŀ�������������ɫ�Ŀ������ǿյģ�����������н�ɫ�Ŀ�������
	// ���Կ��������ָ���ǻ���ڿյ�����ģ����������ģ�˵����������������Լ��Ŀ�������
	// �����������������ֻ��Ҫ�ж�ָ���Ƿ�Ϊ�վͿ����ˣ�����Ҫ���ԡ�
	if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(AuraPlayerController->GetHUD()))
		{
			AuraHUD->InitOverlay(AuraPlayerController, AuraPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}

}
