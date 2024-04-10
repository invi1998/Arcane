// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AuraAbilitySystemLibrary.generated.h"

/**
 * 
 */
UCLASS()
class ARCANE_API UAuraAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// ��Ϊ��̬�������ܷ���ʵ�������󣬾�̬�����������౾��Ͳ��ᱻ������World�У�������Ҫ����һ��WorldContextObject�����ڻ�ȡ��ǰ��World
	// ������Ҳ����Ϊʲô�ܶຯ�����������ж���Ҫһ��WorldContextObject��ԭ��
	// ���磺 ͨ��WorldContextObject��ȡ��ǰ��World��Ȼ��ͨ��World��ȡGameInstance����ͨ��GameInstance��ȡ�Զ����GameInstance�ȵȵ�

	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|WidgetController") // BlueprintPure��ʾ����һ������ͼ����������Ҫ�κ����ţ���ֻ��ִ��ĳ�ֲ����������ؽ��
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);
	
};
