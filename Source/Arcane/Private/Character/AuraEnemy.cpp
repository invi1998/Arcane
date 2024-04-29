// Copyright INVI1998


#include "Character/AuraEnemy.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Arcane/Arcane.h"
#include "Components/WidgetComponent.h"
#include "AuraGameplayTags.h"
#include "AI/AuraAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UI/Widget/AuraUserWidget.h"

AAuraEnemy::AAuraEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);	// ������ײ��Ӧ

	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>(TEXT("AbilitySystemComponent"));	// ��������ϵͳ���
	AbilitySystemComponent->SetIsReplicated(true);	// ���ø���
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);	// ���ø���ģʽ������AI��˵��ֻ��Ҫ��С�ĸ���

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>(TEXT("AttributeSet"));	// �������Լ�

	HealthBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));	// ����Ѫ��
	HealthBar->SetupAttachment(GetRootComponent());	// ���ø���

	bUseControllerRotationYaw = false;	// �رտ�������תƫ��
	bUseControllerRotationPitch = false;	// �رտ�������ת����
	bUseControllerRotationRoll = false;	// �رտ�������ת����

	GetCharacterMovement()->bUseControllerDesiredRotation = true;	// ����������������ת���ƶ��������ݿ���������ת����ת��

}

void AAuraEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (HasAuthority())
	{
		AuraAIController = Cast<AAuraAIController>(NewController);	// ����AI������

		AuraAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);	// ��ʼ���ڰ�

		AuraAIController->RunBehaviorTree(BehaviorTree);	// ������Ϊ��

		AuraAIController->GetBlackboardComponent()->SetValueAsBool("HitReacting", false);	// ���úڰ�ֵ, �Ƿ��ܻ���Ӧ

		AuraAIController->GetBlackboardComponent()->SetValueAsBool("RangedAttacker", CharacterClass == ECharacterClass::Ranger);	// ���úڰ�ֵ, �Ƿ�Զ�̹�����

		AuraAIController->GetBlackboardComponent()->SetValueAsBool("ZombieAttacker", CharacterClass == ECharacterClass::Zombie);	// ���úڰ�ֵ, �Ƿ�ɥʬ������

		AuraAIController->GetBlackboardComponent()->SetValueAsBool("LichAttacker", CharacterClass == ECharacterClass::Lich);	// ���úڰ�ֵ, �Ƿ�����������

		AuraAIController->GetBlackboardComponent()->SetValueAsBool("GoblinAttacker", CharacterClass == ECharacterClass::Goblin);	// ���úڰ�ֵ, �Ƿ�粼�ֹ�����

		AuraAIController->GetBlackboardComponent()->SetValueAsBool("ElementalistAttacker", CharacterClass == ECharacterClass::Elementalist);	// ���úڰ�ֵ, �Ƿ�Ԫ��ʦ������

		AuraAIController->GetBlackboardComponent()->SetValueAsBool("WarriorAttacker", CharacterClass == ECharacterClass::Warrior);	// ���úڰ�ֵ, �Ƿ�սʿ������

		AuraAIController->GetBlackboardComponent()->SetValueAsBool("LichKingAttacker", CharacterClass == ECharacterClass::LichKing);	// ���úڰ�ֵ, �Ƿ�������������

		AuraAIController->GetBlackboardComponent()->SetValueAsBool("LichGuardAttacker", CharacterClass == ECharacterClass::LichGuard);	// ���úڰ�ֵ, �Ƿ�������ʿ������

		AuraAIController->GetBlackboardComponent()->SetValueAsBool("LichQueenAttacker", CharacterClass == ECharacterClass::LichQueen);	// ���úڰ�ֵ, �Ƿ�����Ů��������

		AuraAIController->GetBlackboardComponent()->SetValueAsBool("GhoulAttacker", CharacterClass == ECharacterClass::Ghoul);	// ���úڰ�ֵ, �Ƿ�ʳʬ������

		AuraAIController->GetBlackboardComponent()->SetValueAsBool("IsAlive", true);	// ���úڰ�ֵ, �Ƿ���

	}
}

void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;	// ������������ٶ�
	
	InitAbilityActorInfo();	// ��ʼ������ϵͳ���������ӵ���ߺ�������

	if (HasAuthority())
	{
		UAuraAbilitySystemLibrary::GiveStartupAbilities(this, AbilitySystemComponent, CharacterClass);	// ��ʼ������
	}

	// ��Ѫ�������WidgetController����ΪActor����
	if (UAuraUserWidget* AuraUserWidget = Cast<UAuraUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		// ������WidgetController����ӵ�����Widget��Actor����Ϊʲô�������������أ�
		// ��ΪUAuraUserWidgetController�̳���UObject����UObject��һ�����࣬�κμ̳���UObject���඼��������ΪWidgetController
		// ��UE��κ��඼�̳���UObject���������ǿ��������κ���ΪWidgetController
		AuraUserWidget->SetWidgetController(this);
	}

	if (const UAuraAttributeSet* AuraAS = Cast<UAuraAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAS->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)->void
			{
				OnHealthChanged.Broadcast(Data.NewValue);	// �㲥����ֵ�ı�
			}
		);	// �������ֵ�ı�ί��
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAS->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)->void
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);	// �㲥�������ֵ�ı�
			}
		);	// ����������ֵ�ı�ί��
		
		AbilitySystemComponent->RegisterGameplayTagEvent(FAuraGameplayTags::Get().Effect_HitReact, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AAuraEnemy::HitReactTagChanged);	// ע��������ǩ�ı�ί��

		// ֱ��������㲥һ�Σ���ΪѪ���ĳ�ʼ��
		OnHealthChanged.Broadcast(AuraAS->GetHealth());	// �㲥����ֵ�ı�
		OnMaxHealthChanged.Broadcast(AuraAS->GetMaxHealth());	// �㲥�������ֵ�ı�
	}
}

void AAuraEnemy::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bIsHitReact = NewCount > 0;	// �Ƿ��ܻ���Ӧ

	// �ܻ���ֹͣ�ƶ�
	GetCharacterMovement()->MaxWalkSpeed = bIsHitReact ? 0.f : BaseWalkSpeed;	// ����ܻ����ٶ�Ϊ0������Ϊԭ�����ٶ�

	if (AuraAIController && AuraAIController->GetBlackboardComponent())
	{
		AuraAIController->GetBlackboardComponent()->SetValueAsBool("HitReacting", bIsHitReact);	// ���úڰ�ֵ���Ƿ��ܻ���Ӧ
	}
	
}

void AAuraEnemy::Die()
{
	if (AuraAIController && AuraAIController->GetBlackboardComponent())
	{
		AuraAIController->GetBlackboardComponent()->SetValueAsBool("IsAlive", false);	// ���úڰ�ֵ, �Ƿ���
	}
	
	SetLifeSpan(LifeSpan);
	Super::Die();
}

void AAuraEnemy::SetCombatTarget_Implementation(AActor* NewTarget)
{
	CombatTarget = NewTarget;
}

AActor* AAuraEnemy::GetCombatTarget_Implementation() const
{
	return CombatTarget;
}

void AAuraEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);	// ��ʼ������ϵͳ���������ӵ���ߺ�������

	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();	// ���ü���Actor��Ϣ

	if (HasAuthority())
	{
		InitializeDefaultAttributes();
	}
}

void AAuraEnemy::InitializeDefaultAttributes() const
{
	// ͨ����ɫְҵ�͵ȼ���ʼ����ɫ����
	UAuraAbilitySystemLibrary::InitCharacterAttributesByClassAndLevel(this, CharacterClass, Level, AbilitySystemComponent);
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
	if (LeftWeapon)
	{
		LeftWeapon->SetRenderCustomDepth(true);	// �����Զ��������Ⱦ
		LeftWeapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_STENCIL_RED);	// �����Զ������ֵ
	}
	if (RightWeapon)
	{
		RightWeapon->SetRenderCustomDepth(true);	// �����Զ��������Ⱦ
		RightWeapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_STENCIL_RED);	// �����Զ������ֵ
	}
	if (BowWeapon)
	{
		BowWeapon->SetRenderCustomDepth(true);	// �����Զ��������Ⱦ
		BowWeapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_STENCIL_RED);	// �����Զ������ֵ
	}
	if (BowArrow)
	{
		BowArrow->SetRenderCustomDepth(true);	// �����Զ��������Ⱦ
		BowArrow->SetCustomDepthStencilValue(CUSTOM_DEPTH_STENCIL_RED);	// �����Զ������ֵ
	}

}

void AAuraEnemy::UnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);	// ȡ���Զ��������Ⱦ
	if (Weapon)
	{
		Weapon->SetRenderCustomDepth(false);	// ȡ���Զ��������Ⱦ
	}
	if (LeftWeapon)
	{
		LeftWeapon->SetRenderCustomDepth(false);	// �����Զ��������Ⱦ
	}
	if (RightWeapon)
	{
		RightWeapon->SetRenderCustomDepth(false);	// �����Զ��������Ⱦ
	}
	if (BowWeapon)
	{
		BowWeapon->SetRenderCustomDepth(false);	// �����Զ��������Ⱦ
	}
	if (BowArrow)
	{
		BowArrow->SetRenderCustomDepth(false);	// �����Զ��������Ⱦ
	}
}

int32 AAuraEnemy::GetCharacterLevel_Implementation() const
{
	return Level;
}
