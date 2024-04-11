// Copyright INVI1998


#include "Player/AuraPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "Input/AuraEnhancedInputComponent.h"
#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;		// ��������
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();	// ��������߼��
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(AuraContext);	// �������ӳ���������Ƿ���ڣ�����������򱨴�

	UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());		// ��ȡ��ǿ������ϵͳ

	if (EnhancedInputSubsystem)
	{
		EnhancedInputSubsystem->AddMappingContext(AuraContext, 0);	// �������ӳ�������ģ�0��ʾ���ȼ�����Ϊֻ��һ������ӳ�������ģ��������ȼ�Ϊ0
	}

	
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

	UAuraEnhancedInputComponent* AuraEnhancedInputComponent = CastChecked<UAuraEnhancedInputComponent>(InputComponent);		// ��ȡ��ǿ�������������ת��Ϊ��ǿ����������������ת��ʧ���򱨴�

	// һ��������ǿ������������ǾͿ��԰�������
	AuraEnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);		// ���ƶ����룬������һ���������룬����ʹ��BindAction�������¼���Triggered���󶨵ĺ�����Move��Triggered�¼����ڰ��°���ʱ��������Ϊ���ǵ��ƶ��ǳ����ģ�����������Ҫ�ڰ��°���ʱ����

	// �󶨼������� ThisClass::AbilityInputTagPressed = AAuraPlayerController::AbilityInputTagPressed
	AuraEnhancedInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);

}

void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHitResult;	// ����һ����ײ���
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHitResult);	// ��ȡ������µ���ײ�����ECC_Visibility��ʾֻ���ɼ���ͨ����false��ʾ����⸴����ײ��CursorHitResult����ײ���

	if (!CursorHitResult.bBlockingHit) return;

	// ������֡����һ֡����ײ������ж��Ƿ���ͬһ������
	LastActor = ThisActor;
	ThisActor = Cast<IEnemyInterface>(CursorHitResult.GetActor());		// ����ײ�����Actorת��Ϊ���˽ӿڣ����ת��ʧ���򷵻�nullptr

	/*
	 * ��������߼�⡣���������������
	 * 1����һ֡����һ֡��û����ײ�����壬��ô�����κβ���
	 * 2����һ֡����һ֡����ײ�������壬���ǲ���ͬһ�����壬��ôȡ����һ������ĸ���
	 * 3����һ֡����һ֡����ײ�������壬������ͬһ�����壬��ô�����κβ���
	 * 4����һ֡û����ײ�����壬��һ֡��ײ�������壬��ô�����������
	 * 5����һ֡��ײ�������壬��һ֡û����ײ�����壬��ôȡ����һ������ĸ���
	 */
	if (LastActor && ThisActor)
	{
		if (LastActor != ThisActor)
		{
			LastActor->UnHighlightActor();	// ȡ����һ������ĸ���
			ThisActor->HighlightActor();	// �����������
		}
	}
	else if (LastActor && !ThisActor)
	{
		LastActor->UnHighlightActor();	// ȡ����һ������ĸ���
	}
	else if (!LastActor && ThisActor)
	{
		ThisActor->HighlightActor();	// �����������
	}

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

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("AbilityInputTagPressed: %s"), *InputTag.ToString()));
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("AbilityInputTagReleased: %s"), *InputTag.ToString()));
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("AbilityInputTagHeld: %s"), *InputTag.ToString()));
}
