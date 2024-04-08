// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "OverlayWidgetController.generated.h"

struct FOnAttributeChangeData;		// ���Ըı�����

// ����������̬�ಥί�У���Ϊ��ϣ������ͼ��Ϊ���Ƿַ��¼�
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, float, NewHealth);	// ����ֵ�ı�, һ���������µ�����ֵ
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxHealthChangedSignature, float, NewMaxHealth);		// �������ֵ�ı�, һ���������µ��������ֵ
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnManaChangedSignature, float, NewMana);		// ����ֵ�ı�, һ���������µķ���ֵ
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxManaChangedSignature, float, NewMaxMana);		// �����ֵ�ı�, һ���������µ������ֵ


// ��ṹ�����ڴ洢UIС�����е����ݣ���������Ļ����ʾ��Ϣ
USTRUCT(BlueprintType)		// ����Ϊ��ͼ����
struct FUIWidgetRow : public FTableRowBase 	// UIС�����У��̳���FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)		// ����Ϊ�ɱ༭���κεط�����ͼ�ɶ�
	FGameplayTag MessageTag = FGameplayTag::EmptyTag;		// ��Ϣ��ǩ

	FText MessageText = FText::GetEmpty();		// ��Ϣ�ı�

	UPROPERTY(EditAnywhere, BlueprintReadOnly)		// ����Ϊ�ɱ༭���κεط�����ͼ�ɶ�
	TSubclassOf<class UAuraUserWidget> MessageWidgetClass;		// С�����࣬����������ǿ�������ͼ�����ⶨ�ƣ�������ʾ�ı���ͼƬ�ȵ�

	UPROPERTY(EditAnywhere, BlueprintReadOnly)		// ����Ϊ�ɱ༭���κεط�����ͼ�ɶ�
	UTexture2D* MessageIcon = nullptr;		// ��Ϣͼ��
};

/**
 * OverlayWidgetController, ���Ӳ�ؼ�������, ����ΪBlueprintType, Blueprintable������Ϊ����ϣ������ͼ��ʹ����
 */
UCLASS(BlueprintType, Blueprintable)
class ARCANE_API UOverlayWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;		// �㲥��ʼֵ
	virtual void BindCallbacksToDependencies() override;		// �󶨻ص���������

	/*
	 * ��Ϊ���Ǵ�����һ����̬�ಥί�У�����������Ҫһ����ͼ�ɷ�����¼���������
	 * һ�����ǵ�С������ͼ��WBP���Ӳ�����κ�������������������ܹ����ʵ���������������ǾͿ��Է���һ���¼����������ί��
	 */

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")		// ����Ϊ��ͼ�ɷ��䣬����ΪGAS�µ�Attributes
	FOnHealthChangedSignature OnHealthChanged;		// ����ֵ�ı�

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")		// ����Ϊ��ͼ�ɷ��䣬����ΪGAS�µ�Attributes
	FOnMaxHealthChangedSignature OnMaxHealthChanged;		// �������ֵ�ı�

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")		// ����Ϊ��ͼ�ɷ��䣬����ΪGAS�µ�Attributes
	FOnManaChangedSignature OnManaChanged;		// ����ֵ�ı�

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")		// ����Ϊ��ͼ�ɷ��䣬����ΪGAS�µ�Attributes
	FOnMaxManaChangedSignature OnMaxManaChanged;		// �����ֵ�ı�

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Widget Data")		// ����Ϊ�ɱ༭���κεط�����ͼ�ɶ�
	TObjectPtr<UDataTable> MessageWidgetDataTable;		// ��ϢС�������ݱ�

	void HealthChanged(const FOnAttributeChangeData& Data) const;		// ����ֵ�ı䣬��������ΪFOnAttributeChangeData������һ���ṹ�壬���ڴ洢���Ըı������
	void MaxHealthChanged(const FOnAttributeChangeData& Data) const;		// �������ֵ�ı䣬��������ΪFOnAttributeChangeData������һ���ṹ�壬���ڴ洢���Ըı������
	void ManaChanged(const FOnAttributeChangeData& Data) const;		// ����ֵ�ı䣬��������ΪFOnAttributeChangeData������һ���ṹ�壬���ڴ洢���Ըı������
	void MaxManaChanged(const FOnAttributeChangeData& Data) const;		// �����ֵ�ı䣬��������ΪFOnAttributeChangeData������һ���ṹ�壬���ڴ洢���Ըı������
};
