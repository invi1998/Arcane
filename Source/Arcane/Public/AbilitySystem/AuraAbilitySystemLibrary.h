// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Data/CharacterClassInfo.h"

#include "AuraAbilitySystemLibrary.generated.h"

class UAbilitySystemComponent;

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

	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|WidgetController")
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);

	// ���ݽ�ɫְҵ�ͽ�ɫ�ȼ���ʼ����ɫ����
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|CharacterClassDefault")
	static void InitCharacterAttributesByClassAndLevel(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC);
	
};
