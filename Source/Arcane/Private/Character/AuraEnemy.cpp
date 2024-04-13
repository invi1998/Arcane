// Copyright INVI1998


#include "Character/AuraEnemy.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Arcane/Arcane.h"
#include "Components/WidgetComponent.h"
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

}

void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	InitAbilityActorInfo();	// ��ʼ������ϵͳ���������ӵ���ߺ�������

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

		// ֱ��������㲥һ�Σ���ΪѪ���ĳ�ʼ��
		OnHealthChanged.Broadcast(AuraAS->GetHealth());	// �㲥����ֵ�ı�
		OnMaxHealthChanged.Broadcast(AuraAS->GetMaxHealth());	// �㲥�������ֵ�ı�
	}
}

void AAuraEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);	// ��ʼ������ϵͳ���������ӵ���ߺ�������

	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();	// ���ü���Actor��Ϣ

	InitializeDefaultAttributes();
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

}

void AAuraEnemy::UnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);	// ȡ���Զ��������Ⱦ
	if (Weapon)
	{
		Weapon->SetRenderCustomDepth(false);	// ȡ���Զ��������Ⱦ
	}
}

int32 AAuraEnemy::GetPlayerLevel() const
{
	return Level;
}
