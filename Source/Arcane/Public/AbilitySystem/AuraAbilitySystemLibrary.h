// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Data/CharacterClassInfo.h"

#include "AuraAbilitySystemLibrary.generated.h"

class UAbilitySystemComponent;
struct FWidgetControllerParams;

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

	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|AbilitySystemComponent")
	static bool MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutParams, AAuraHUD*& OutAuraHUD);

	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|WidgetController") // BlueprintPure��ʾ����һ������ͼ����������Ҫ�κ����ţ���ֻ��ִ��ĳ�ֲ����������ؽ��
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|WidgetController")
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|WidgetController")
	static USpellMenuWidgetController* GetSpellMenuWidgetController(const UObject* WorldContextObject);

	// ���ݽ�ɫְҵ�ͽ�ɫ�ȼ���ʼ����ɫ����
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|CharacterClassDefault")
	static void InitCharacterAttributesByClassAndLevel(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC);

	// ��ʼ����ɫ������
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|CharacterClassDefault")
	static void GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass);

	// ��ȡCharacterClassInfo
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|CharacterClassDefault")
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject);

	// ��AuraGampelayEffectContext�л�ȡ����Ϣ
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static bool IsBlockedHit(const FGameplayEffectContextHandle& ContextHandle);

	// ��AuraGampelayEffectContext�л�ȡ������Ϣ
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& ContextHandle);

	// ���ñ�����Ϣ UPARAM(ref)��ʾ����������ã�������ֵ�����������������ͼ������Ϊ�������ŵ�
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& ContextHandle, bool bCriticalHit);

	// ���ø���Ϣ UPARAM(ref)��ʾ����������ã�������ֵ�����������������ͼ������Ϊ�������ŵ�
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetBlockedHit(UPARAM(ref) FGameplayEffectContextHandle& ContextHandle, bool bBlockedHit);

	// ��ȡ�뾶�ڵ����д����� �����������Ķ��󣬴���������飬������Ե�������飬�������ĵ㣬����뾶��
	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|GameplayMechanice")
	static void GetLivePlayerWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutPlayers, const TArray<AActor*>& IgnoreActors, const FVector& Origin, float Radius);

	// ��ȡ���߼���ڵ����д����� �����������Ķ��󣬴���������飬������Ե�������飬����������㣬�����յ㣩
	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|GameplayMechanice")
	static void GetLivePlayerWithinRaycast(const UObject* WorldContextObject, TArray<AActor*>& OutPlayers, const TArray<AActor*>& IgnoreActors, const FVector& Start, const FVector& End);

	// ��ȡ�������ص�����ڵ����д����� �����������Ķ��󣬴���������飬������Ե�������飬�������������ģ�����������ߴ磩
	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|GameplayMechanice")
	static void GetLivePlayerWithinBoxOverlap(const UObject* WorldContextObject, TArray<AActor*>& OutPlayers, const TArray<AActor*>& IgnoreActors, const FVector& Origin, const FVector& BoxExtent);

	// ��ȡǰ�������ڵ����д����� �����������Ķ��󣬴���������飬������Ե�������飬�������ĵ㣬����ǰ���򣬴������νǶȣ�����뾶��
	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|GameplayMechanice")
	static void GetLivePlayerWithinForwardSector(const UObject* WorldContextObject, TArray<AActor*>& OutPlayers, const TArray<AActor*>& IgnoreActors, const FVector& Origin, const FVector& Forward, float Angle, float Radius);

	// �Ƿ����Ѿ������������Ķ��󣬴����Լ�������Ŀ�꣩
	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|GameplayMechanice")
	static bool IsFriendly(AActor* FirstActor, AActor* SecondActor);

	// ��ȡ����ľ��齱�������������Ķ��󣬴������ȼ��� ����ְҵ��
	static int32 GetMonsterEXPRewardByClassAndLv(const UObject* WorldContextObject, ECharacterClass CharacterClass, int32 CharacterLv);

};
