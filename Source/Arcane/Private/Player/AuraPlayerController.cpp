// Copyright INVI1998


#include "Player/AuraPlayerController.h"

#include "EnhancedInputComponent.h"
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

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);	// ��ȡ��ǿ�������������ת��Ϊ��ǿ����������������ת��ʧ���򱨴�

	// һ��������ǿ������������ǾͿ��԰�������
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);		// ���ƶ����룬������һ���������룬����ʹ��BindAction�������¼���Triggered���󶨵ĺ�����Move��Triggered�¼����ڰ��°���ʱ��������Ϊ���ǵ��ƶ��ǳ����ģ�����������Ҫ�ڰ��°���ʱ����
}

void AAuraPlayerController::Move(const FInputActionValue& Value)
{
	// �������ƶ�������ʵ��

	const FVector2D& InputAxisVector = Value.Get<FVector2D>();	// ��ȡ�����������������Ƕ�ά��������Ϊ���ǵ��ƶ��Ƕ�ά�ģ���������������ӳ���������������˶�ά����
	const FRotator Rotation = GetControlRotation();	// ��ȡ������ת
	const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);	// ����һ��ֻ��Yaw��ת����ת���������ǾͿ�����ˮƽ�����ƶ�

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);	// ��ȡǰ����
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);		// ��ȡ������

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);		// ���ǰ���ƶ�����
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);		// ��������ƶ�����
	}
	
}
