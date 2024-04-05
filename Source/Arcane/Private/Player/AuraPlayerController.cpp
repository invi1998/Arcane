// Copyright INVI1998


#include "Player/AuraPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;		// 开启复制
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(AuraContext);	// 检查输入映射上下文是否存在，如果不存在则报错

	UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());		// 获取增强输入子系统

	check(EnhancedInputSubsystem);	// 检查增强输入子系统是否存在，如果不存在则报错

	EnhancedInputSubsystem->AddMappingContext(AuraContext, 0);	// 添加输入映射上下文，0表示优先级，因为只有一个输入映射上下文，所以优先级为0

	bShowMouseCursor = true;	// 显示鼠标光标
	DefaultMouseCursor = EMouseCursor::Default;	// 设置鼠标光标为默认

	FInputModeGameAndUI InputMode;	// 创建输入模式
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);	// 设置鼠标锁定模式为不锁定，这样我们就可以在窗口外移动鼠标
	InputMode.SetHideCursorDuringCapture(false);	// 设置捕获时隐藏鼠标光标为false，这样我们就可以在窗口外移动鼠标
	SetInputMode(InputMode);	// 设置输入模式
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);	// 获取增强输入组件，类型转换为增强输入组件，如果类型转换失败则报错

	// 一但有了增强输入组件，我们就可以绑定输入了
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Started, this, &AAuraPlayerController::Move);		// 绑定移动动作，当动作开始时调用Move函数
}

void AAuraPlayerController::Move(const FInputActionValue& Value)
{
	// 这里是移动函数的实现

	const FVector2D& InputAxisVector = Value.Get<FVector2D>();	// 获取输入轴向量，这里是二维向量，因为我们的移动是二维的，而且我们在输入映射上下文中设置了二维向量
	const FRotator Rotation = GetControlRotation();	// 获取控制旋转
	const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);	// 创建一个只有Yaw旋转的旋转，这样我们就可以在水平面上移动

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);	// 获取前向方向
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);		// 获取右向方向

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);		// 添加前向移动输入
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);		// 添加右向移动输入
	}
	
}
