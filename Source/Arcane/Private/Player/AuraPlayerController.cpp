// Copyright INVI1998


#include "Player/AuraPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Components/SplineComponent.h"
#include "Input/AuraEnhancedInputComponent.h"
#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;		// ��������

	// ������·��ʱ�������������λ��
	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));	// ���������������
	
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();	// ��������߼��

	AutoRun();	// �Զ�����
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

UAuraAbilitySystemComponent* AAuraPlayerController::GetASC()
{
	if (!AuraAbilitySystemComponent)
	{
		// ��Ȼ����ȡ����ϵͳ������ܻ�ʧ�ܣ���Ϊ���ǵĽ�ɫ����û������ϵͳ�������������ϵͳ�����û�г�ʼ������������Ҫ������ֻ��Ҫ�����ﻺ��һ�£������ȡʧ�ܣ����ؿ�Ҳû��ϵ
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return AuraAbilitySystemComponent;
}

void AAuraPlayerController::AutoRun()
{
	if (bAutoRunning)
	{
		APawn* ControlledPawn = GetPawn();	// ��ȡ���Ƶ�Pawn
		if (!ControlledPawn) return;

		const FVector PawnLocation = ControlledPawn->GetActorLocation();	// ��ȡPawn��λ��
		const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(PawnLocation, ESplineCoordinateSpace::World);	// ��ȡ������������Pawn����ĵ��λ��
		const FVector DirectionOnSpline = Spline->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);	// ��ȡ������������Pawn����ĵ�ķ���

		ControlledPawn->AddMovementInput(DirectionOnSpline, 1.f);	// ����ƶ����룬�������������������ƶ������Է�����������������Pawn����ĵ�ķ����ٶ���1

		// �ж��Ƿ񵽴����ǵ�Ŀ��λ�ã������������Զ����ܵİ뾶��ֵ��
		const float DistanceToDestination = (LocationOnSpline - CashedDestination).Length();	// ����Pawnλ�ú�������������Pawn����ĵ��λ�õľ���

		if (DistanceToDestination <= AutoRunAcceptanceRadius)	// �������С��100
		{
			bAutoRunning = false;	// ȡ���Զ�Ѱ·
		}
	}
}

void AAuraPlayerController::CursorTrace()
{
	
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
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("AbilityInputTagPressed: %s"), *InputTag.ToString()));

	if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_RightMouseButton))	// ��������ǩƥ���Զ�Ѱ·��ǩ���Ҽ���ذ壩
	{
		bTargeting = ThisActor != nullptr;	// �����ǰ���е�Actor��Ϊ�գ�������Ϊ��׼״̬
		bAutoRunning = false;	// ȡ���Զ�Ѱ·
	}

	
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("AbilityInputTagReleased: %s"), *InputTag.ToString()));

	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_RightMouseButton))	// ��������ǩ��ƥ���Զ�Ѱ·��ǩ���Ҽ���ذ壩
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagReleased(InputTag);
		}

		return;
	}

	if (bTargeting)
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagReleased(InputTag);
		}
	}
	else
	{
		APawn* ControlledPawn = GetPawn<APawn>();	// ��ȡ���Ƶ�Pawn
		// �жϵ�ǰ�Ƕ̰����ǳ���
		if (FollowTime <= ShortPressThreshold && ControlledPawn)
		{
			// �㰴���̰���ʱ��ͨ��Ѱ·���ϵ�Ŀ��λ��
			// // Ѱ��·����Ŀ��λ��
			if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControlledPawn->GetActorLocation(), CashedDestination))	// ���·����Ч
			{
				// ������������
				Spline->ClearSplinePoints();	// ����������ߵ�
				for (const auto& PathPoint : NavPath->PathPoints)	// ����·����
				{
					Spline->AddSplinePoint(PathPoint, ESplineCoordinateSpace::World);	// ����������ߵ�
					// ���Ƶ�����
					// DrawDebugSphere(GetWorld(), PathPoint, 10.f, 12, FColor::Green, false, 5.1f);	// ���Ƶ�������
				}
				// ����Ŀ��λ�ã�������·�������һ���㣬��������Ϊ�˱�����������������λ�ò��ɴ���������ǵĽ�ɫ�޷�����Ŀ��λ��
				// ��Ϊ�����������ߵĵ���·���ĵ㣬��һ���ɴ��
				if (NavPath->PathPoints.Num() > 0)
				{
					CashedDestination = NavPath->PathPoints.Last();
					bAutoRunning = true;	// ����Ϊ�Զ�Ѱ·
				}
			}
		}

		FollowTime = 0.f;	// ���ø���ʱ��
		bTargeting = false;	// ȡ����׼
	}
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("AbilityInputTagHeld: %s"), *InputTag.ToString()));

	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_RightMouseButton))	// ��������ǩ��ƥ���Զ�Ѱ·��ǩ���Ҽ���ذ壩
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagHeld(InputTag);	// ��������ϵͳ����ļ��������ǩ��ס����
		}

		return;
	}

	// �����Ҽ������ɫҲ�ܴ�������
	if (bTargeting)
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagHeld(InputTag);	// ��������ϵͳ����ļ��������ǩ��ס����
		}
	}
	else
	{
		// �����ǰû������Actor����ô�Զ�Ѱ·
		FollowTime += GetWorld()->GetDeltaSeconds();	// ����ʱ������

		// ǰ��Ŀ��λ�ã�����������Ҫ��ȡ������µ���ײ���
		if (CursorHitResult.bBlockingHit)	// ��ȡ������µ���ײ�����ECC_Visibility��ʾֻ���ɼ���ͨ����false��ʾ����⸴����ײ��CursorHitResult����ײ���
		{
			CashedDestination = CursorHitResult.ImpactPoint;	// ����Ŀ��λ��,ImpactPoint����ײ��
		}

		// �������ʱ�䳬����ֵ����ô�Զ�Ѱ·
		if (APawn* ControlledPawn = GetPawn<APawn>())
		{
			ControlledPawn->AddMovementInput(
				(CashedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal(),	// ����ƶ����룬������ǰ��Ŀ��λ�ã����Է�����Ŀ��λ�ü�ȥ��ǰλ��,GetSafeNormal�ǻ�ȡ��λ����
				1.f);	// ����ƶ����룬������ǰ��Ŀ��λ�ã����Է�����Ŀ��λ�ü�ȥ��ǰλ�ã��ٶ���1
		}
	}
}


