// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "OverlayWidgetController.generated.h"

// ��ṹ�����ڴ洢UIС�����е����ݣ���������Ļ����ʾ��Ϣ
USTRUCT(BlueprintType)		// ����Ϊ��ͼ����
struct FUIWidgetRow : public FTableRowBase 	// UIС�����У��̳���FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)		// ����Ϊ�ɱ༭���κεط�����ͼ�ɶ�
	FGameplayTag MessageTag = FGameplayTag::EmptyTag;		// ��Ϣ��ǩ

	UPROPERTY(EditAnywhere, BlueprintReadOnly)		// ����Ϊ�ɱ༭���κεط�����ͼ�ɶ�)
	FText MessageText = FText::GetEmpty();		// ��Ϣ�ı�

	UPROPERTY(EditAnywhere, BlueprintReadOnly)		// ����Ϊ�ɱ༭���κεط�����ͼ�ɶ�
	TSubclassOf<class UAuraUserWidget> MessageWidgetClass;		// С�����࣬����������ǿ�������ͼ�����ⶨ�ƣ�������ʾ�ı���ͼƬ�ȵ�

	UPROPERTY(EditAnywhere, BlueprintReadOnly)		// ����Ϊ�ɱ༭���κεط�����ͼ�ɶ�
	UTexture2D* MessageIcon = nullptr;		// ��Ϣͼ��

	UPROPERTY(EditAnywhere, BlueprintReadOnly)		// ����Ϊ�ɱ༭���κεط�����ͼ�ɶ�
	UTexture2D* BackgroundIcon = nullptr;		// ��Ϣ����

};

struct FOnAttributeChangeData;		// ���Ըı�����

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangeSignature, float, NewValue);		// ���Ըı�ί�У�����һ��float���͵���ֵ

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetRowSignature, FUIWidgetRow, MessageWidgetRow);		// ��ϢС�����У�һ����������ϢС������

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
	FOnAttributeChangeSignature OnHealthChanged;		// ����ֵ�ı�

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")		// ����Ϊ��ͼ�ɷ��䣬����ΪGAS�µ�Attributes
	FOnAttributeChangeSignature OnMaxHealthChanged;		// �������ֵ�ı�

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")		// ����Ϊ��ͼ�ɷ��䣬����ΪGAS�µ�Attributes
	FOnAttributeChangeSignature OnManaChanged;		// ����ֵ�ı�

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")		// ����Ϊ��ͼ�ɷ��䣬����ΪGAS�µ�Attributes
	FOnAttributeChangeSignature OnMaxManaChanged;		// �����ֵ�ı�

	UPROPERTY(BlueprintAssignable, Category="GAS|Message")		// ����Ϊ��ͼ�ɷ��䣬����ΪGAS�µ�Attributes
	FMessageWidgetRowSignature MessageWidgetRowDelegate;		// ��ϢС���� ��ί��

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Widget Data")		// ����Ϊ�ɱ༭���κεط�����ͼ�ɶ�
	TObjectPtr<UDataTable> MessageWidgetDataTable;		// ��ϢС�������ݱ�

	template<typename T>
	T* GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag) const;		// ͨ����ǩ��ȡ���ݱ���

};

template <typename T>
T* UOverlayWidgetController::GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag) const
{
	if (!DataTable)
	{
		return nullptr;
	}

	const FString ContextString = TEXT("");
	return DataTable->FindRow<T>(*Tag.GetTagName().ToString(), ContextString, true);
}
