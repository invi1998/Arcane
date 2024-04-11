// Copyright INVI1998


#include "Player/AuraPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Components/SplineComponent.h"
#include "Input/AuraEnhancedInputComponent.h"
#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;		// 开启复制

	// 在生成路径时，设置样条点的位置
	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));	// 创建样条曲线组件
	
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();	// 鼠标点的射线检测
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(AuraContext);	// 检查输入映射上下文是否存在，如果不存在则报错

	UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());		// 获取增强输入子系统

	if (EnhancedInputSubsystem)
	{
		EnhancedInputSubsystem->AddMappingContext(AuraContext, 0);	// 添加输入映射上下文，0表示优先级，因为只有一个输入映射上下文，所以优先级为0
	}

	
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

	UAuraEnhancedInputComponent* AuraEnhancedInputComponent = CastChecked<UAuraEnhancedInputComponent>(InputComponent);		// 获取增强输入组件，类型转换为增强输入组件，如果类型转换失败则报错

	// 一但有了增强输入组件，我们就可以绑定输入了
	AuraEnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);		// 绑定移动输入，这里是一个动作输入，所以使用BindAction，触发事件是Triggered，绑定的函数是Move，Triggered事件是在按下按键时触发，因为我们的移动是持续的，所以我们需要在按下按键时触发

	// 绑定技能输入 ThisClass::AbilityInputTagPressed = AAuraPlayerController::AbilityInputTagPressed
	AuraEnhancedInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);

}

UAuraAbilitySystemComponent* AAuraPlayerController::GetASC()
{
	if (!AuraAbilitySystemComponent)
	{
		// 当然，获取能力系统组件可能会失败，因为我们的角色可能没有能力系统组件，或者能力系统组件还没有初始化，不过不重要，我们只需要在这里缓存一下，如果获取失败，返回空也没关系
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return AuraAbilitySystemComponent;
}

void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHitResult;	// 创建一个碰撞结果
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHitResult);	// 获取鼠标光标下的碰撞结果，ECC_Visibility表示只检测可见性通道，false表示不检测复杂碰撞，CursorHitResult是碰撞结果

	if (!CursorHitResult.bBlockingHit) return;

	// 根据这帧和上一帧的碰撞结果，判断是否是同一个物体
	LastActor = ThisActor;
	ThisActor = Cast<IEnemyInterface>(CursorHitResult.GetActor());		// 将碰撞结果的Actor转换为敌人接口，如果转换失败则返回nullptr

	/*
	 * 鼠标点的射线检测。可能有如下情况：
	 * 1：上一帧和这一帧都没有碰撞到物体，那么不做任何操作
	 * 2：上一帧和这一帧都碰撞到了物体，但是不是同一个物体，那么取消上一个物体的高亮
	 * 3：上一帧和这一帧都碰撞到了物体，而且是同一个物体，那么不做任何操作
	 * 4：上一帧没有碰撞到物体，这一帧碰撞到了物体，那么高亮这个物体
	 * 5：上一帧碰撞到了物体，这一帧没有碰撞到物体，那么取消上一个物体的高亮
	 */
	if (LastActor && ThisActor)
	{
		if (LastActor != ThisActor)
		{
			LastActor->UnHighlightActor();	// 取消上一个物体的高亮
			ThisActor->HighlightActor();	// 高亮这个物体
		}
	}
	else if (LastActor && !ThisActor)
	{
		LastActor->UnHighlightActor();	// 取消上一个物体的高亮
	}
	else if (!LastActor && ThisActor)
	{
		ThisActor->HighlightActor();	// 高亮这个物体
	}

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

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("AbilityInputTagPressed: %s"), *InputTag.ToString()));

	if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_RightMouseButton))	// 如果输入标签匹配自动寻路标签（右键点地板）
	{
		bTargeting = ThisActor != nullptr;	// 如果当前命中的Actor不为空，则设置为瞄准状态
		bAutoRunning = false;	// 取消自动寻路
	}

	
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("AbilityInputTagReleased: %s"), *InputTag.ToString()));

	if (GetASC())
	{
		GetASC()->AbilityInputTagReleased(InputTag);	// 调用能力系统组件的技能输入标签释放函数
	}
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("AbilityInputTagHeld: %s"), *InputTag.ToString()));

	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_RightMouseButton))	// 如果输入标签不匹配自动寻路标签（右键点地板）
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagHeld(InputTag);	// 调用能力系统组件的技能输入标签按住函数
		}

		return;
	}

	// 表明右键点击角色也能触发技能
	if (bTargeting)
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagHeld(InputTag);	// 调用能力系统组件的技能输入标签按住函数
		}
	}
	else
	{
		// 如果当前没有命中Actor，那么自动寻路
		FollowTime += GetWorld()->GetDeltaSeconds();	// 跟随时间增加

		// 前往目标位置，所以这里需要获取鼠标光标下的碰撞结果
		FHitResult CursorHitResult;	// 创建一个碰撞结果
		if (GetHitResultUnderCursor(ECC_Visibility, false, CursorHitResult))	// 获取鼠标光标下的碰撞结果，ECC_Visibility表示只检测可见性通道，false表示不检测复杂碰撞，CursorHitResult是碰撞结果
		{
			CashedDestination = CursorHitResult.ImpactPoint;	// 缓存目标位置,ImpactPoint是碰撞点
		}

		// 如果按下时间超过阈值，那么自动寻路
		if (APawn* ControlledPawn = GetPawn<APawn>())
		{
			ControlledPawn->AddMovementInput(
				(CashedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal(),	// 添加移动输入，这里是前往目标位置，所以方向是目标位置减去当前位置,GetSafeNormal是获取单位向量
				1.f);	// 添加移动输入，这里是前往目标位置，所以方向是目标位置减去当前位置，速度是1
		}
	}
}


