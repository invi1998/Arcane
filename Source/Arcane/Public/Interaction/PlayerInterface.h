// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ARCANE_API IPlayerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void AddToEXP(int32 EXP);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetEXP(int32 EXP);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	int32 GetEXP() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void LevelUp(int32 Lv);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetLevel(int32 Lv);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	int32 GetLevelByEXP(int32 EXP) const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	int32 GetAttributePointReward() const;	// ��ȡ���Ե㽱��

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	int32 GetSkillPointReward() const;		// ��ȡ���ܵ㽱��

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void AddAttributePoint(int32 Point);	// ���Լӵ�

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void AddSkillPoint(int32 Point);		// ���ܼӵ�

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	int32 GetAttributePoint() const;		// ��ȡ���Ե�

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	int32 GetSkillPoint() const;			// ��ȡ���ܵ�
};
