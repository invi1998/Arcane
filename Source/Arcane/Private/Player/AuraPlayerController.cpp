// Copyright INVI1998


#include "Player/AuraPlayerController.h"

#include "EnhancedInputSubsystems.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;		// ��������
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(AuraContext);	// �������ӳ���������Ƿ���ڣ�����������򱨴�

	UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());		// ��ȡ��ǿ������ϵͳ

	check(EnhancedInputSubsystem);	// �����ǿ������ϵͳ�Ƿ���ڣ�����������򱨴�

	EnhancedInputSubsystem->AddMappingContext(AuraContext, 0);	// �������ӳ�������ģ�0��ʾ���ȼ�����Ϊֻ��һ������ӳ�������ģ��������ȼ�Ϊ0

	bShowMouseCursor = true;	// ��ʾ�����
	DefaultMouseCursor = EMouseCursor::Default;	// ���������ΪĬ��

	FInputModeGameAndUI InputMode;	// ��������ģʽ
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);	// �����������ģʽΪ���������������ǾͿ����ڴ������ƶ����
	InputMode.SetHideCursorDuringCapture(false);	// ���ò���ʱ���������Ϊfalse���������ǾͿ����ڴ������ƶ����
	SetInputMode(InputMode);	// ��������ģʽ
}
