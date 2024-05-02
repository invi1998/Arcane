// Copyright INVI1998


#include "Character/AuraCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"
#include "NiagaraComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/HUD/AuraHUD.h"

AAuraCharacter::AAuraCharacter()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));	// �������ҡ��
	CameraBoom->SetupAttachment(RootComponent);	// �������ҡ�۸��ӵ������
	CameraBoom->SetUsingAbsoluteRotation(true);	// ����ʹ�þ�����ת
	CameraBoom->bDoCollisionTest = false;	// �ر���ײ����

	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCameraComponent"));	// ����������������
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);	// ���ö��������������ӵ����ҡ��
	TopDownCameraComponent->bUsePawnControlRotation = false;	// �ر�ʹ��Pawn������ת

	GetCharacterMovement()->bOrientRotationToMovement = true;	// �����ƶ�ʱ��ת
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 400.0f, 0.0f);	// ������ת����
	GetCharacterMovement()->bConstrainToPlane = true;	// ���������ƶ���ƽ�棬�������ǾͿ�����ˮƽ�����ƶ����������ڿ����ƶ�
	GetCharacterMovement()->bSnapToPlaneAtStart = true;	// �����ڿ�ʼʱ�������ƶ���ƽ��

	bUseControllerRotationPitch = false;	// �رտ�������ת����, �������ǾͲ���̧����ߵ���ͷ����ֻ��������ת������ڸ��ӽ���Ϸ��˵�Ǻܺõ�
	bUseControllerRotationYaw = false;	// �رտ�������תƫ��, �������ǾͲ���������ת
	bUseControllerRotationRoll = false;	// �رտ�������ת����, �������ǾͲ��ܷ���

	CharacterClass = ECharacterClass::Elementalist;	// ���ý�ɫְҵ

	LevelUpEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("LevelUpEffect"));	// ����������Ч
	LevelUpEffect->SetupAttachment(RootComponent);	// ������Ч���ӵ������
	LevelUpEffect->bAutoActivate = false;	// �ر��Զ�����
}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	// �ú���ֻ���ڷ������˱�����
	// PossessedBy()����ͨ���ڷ������˱����á���һ��Controller����PlayerController��AIController����ʼ����һ��Pawn����PlayerCharacter��AICharacter��ʱ�������������PossessedBy()����������Pawn��Controller���ԡ�
	Super::PossessedBy(NewController);

	// Ϊ��������ʼ��AbilitySystemComponent
	InitAbilityActorInfo();

	// ��ʼ����ɫ����
	AddCharacterAbilities();
	
}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Ϊ�ͻ��˳�ʼ��AbilitySystemComponent
	InitAbilityActorInfo();
}

int32 AAuraCharacter::GetCharacterLevel_Implementation() const
{
	// ��ȡ���״̬
	if (const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>())
	{
		return AuraPlayerState->GetPlayerLevel();
	}
	return 0;
}

void AAuraCharacter::AddToEXP_Implementation(int32 EXP)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->AddEXP(EXP);
	const int32 Level = AuraPlayerState->GetPlayerLevel();
	const int32 TotalEXP = AuraPlayerState->GetPlayerEXP();
	const int32 MatchedLevel = AuraPlayerState->LevelUpInfo->GetLevelByExp(TotalEXP);
	if (MatchedLevel > Level)
	{
		LevelUp_Implementation(MatchedLevel - Level);	// ����
	}
}

void AAuraCharacter::SetEXP_Implementation(int32 EXP)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->SetEXP(EXP);
	const int32 Level = AuraPlayerState->GetPlayerLevel();
	const int32 TotalEXP = AuraPlayerState->GetPlayerEXP();
	const int32 MatchedLevel = AuraPlayerState->LevelUpInfo->GetLevelByExp(TotalEXP);
	if (MatchedLevel != Level)
	{
		SetLevel_Implementation(MatchedLevel);	// ���õȼ�
	}
}

void AAuraCharacter::SetLevel_Implementation(int32 Lv)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->SetLevel(Lv);
	if (Lv > 0)
	{
		MulticastLevelUpEffect();	// �ಥ������Ч

		const int32 MatchedLevel = AuraPlayerState->GetPlayerLevel();
		// ��ȡ���������Ե���
		const int32 CulAttributePoints = AuraPlayerState->LevelUpInfo->GetTotalAttributePointRewardInLevel(MatchedLevel);
		if (CulAttributePoints > 0)
		{
			AddAttributePoint_Implementation(CulAttributePoints);
		}

		// ��ȡ�����ļ��ܵ���
		const int32 CulSkillPoints = AuraPlayerState->LevelUpInfo->GetTotalSkillPointRewardInLevel(MatchedLevel);
		if (CulSkillPoints > 0)
		{
			AddSkillPoint_Implementation(CulSkillPoints);
		}
	}
}

void AAuraCharacter::LevelUp_Implementation(int32 Lv)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->AddLevel(Lv);

	if (Lv > 0)
	{
		MulticastLevelUpEffect();	// �ಥ������Ч

		const int32 MatchedLevel = AuraPlayerState->GetPlayerLevel();
		// ��ȡ���������Ե���
		const int32 CulAttributePoints = AuraPlayerState->LevelUpInfo->GetAttributePointRewardByLevel(MatchedLevel);
		if (CulAttributePoints > 0)
		{
			AddAttributePoint_Implementation(CulAttributePoints);
		}

		// ��ȡ�����ļ��ܵ���
		const int32 CulSkillPoints = AuraPlayerState->LevelUpInfo->GetSkillPointRewardByLevel(MatchedLevel);
		if (CulSkillPoints > 0)
		{
			AddSkillPoint_Implementation(CulSkillPoints);
		}

	}
}

int32 AAuraCharacter::GetEXP_Implementation() const
{
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->GetPlayerEXP();
}

int32 AAuraCharacter::GetLevelByEXP_Implementation(int32 EXP) const
{
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->LevelUpInfo->GetLevelByExp(EXP);
}

int32 AAuraCharacter::GetAttributePointReward_Implementation() const
{
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->LevelUpInfo->GetAttributePointRewardByLevel(AuraPlayerState->GetPlayerLevel());
}

int32 AAuraCharacter::GetSkillPointReward_Implementation() const
{
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->LevelUpInfo->GetSkillPointRewardByLevel(AuraPlayerState->GetPlayerLevel());
}

void AAuraCharacter::AddAttributePoint_Implementation(int32 Point)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->AddAttributePoint(Point);
}

void AAuraCharacter::AddSkillPoint_Implementation(int32 Point)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->AddSkillPoint(Point);
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

	InitializeDefaultAttributes();	// ��ʼ����Ҫ����

}

void AAuraCharacter::MulticastLevelUpEffect_Implementation() const
{
	if (IsValid(LevelUpEffect))
	{
		const FVector CameraLocation = TopDownCameraComponent->GetComponentLocation();	// ��ȡ���λ��
		const FVector NiagaraLocation = LevelUpEffect->GetComponentLocation();	// ��ȡ��Чλ��
		// ����Ч���������
		const FRotator LookAtRotator = FRotationMatrix::MakeFromX(CameraLocation - NiagaraLocation).Rotator();
		LevelUpEffect->SetWorldRotation(LookAtRotator);	// ������Ч��ת
		LevelUpEffect->Activate(true);	// ������Ч
	}
}

