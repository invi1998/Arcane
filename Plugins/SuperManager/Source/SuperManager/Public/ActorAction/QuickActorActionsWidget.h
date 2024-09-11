// Copyright INVI_1998

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "QuickActorActionsWidget.generated.h"

UENUM(BlueprintType)
enum class E_DuplicateAxis : uint8
{
	EDA_XAxis	UMETA(DisplayName = "X Axis"),
	EDA_YAxis	UMETA(DisplayName = "Y Axis"),
	EDA_ZAxis	UMETA(DisplayName = "Z Axis"),

	EDA_MAX		UMETA(Hidden)
};

USTRUCT(BlueprintType)
struct FRandomActorTransform
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RandomTransform")
	bool bRandomYaw = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RandomTransform", meta = (EditCondition = "bRandomYaw"))
	float MinYaw = -180.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RandomTransform", meta = (EditCondition = "bRandomYaw"))
	float MaxYaw = 180.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RandomTransform")
	bool bRandomPitch = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RandomTransform", meta = (EditCondition = "bRandomPitch"))
	float MinPitch = -180.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RandomTransform", meta = (EditCondition = "bRandomPitch"))
	float MaxPitch = 180.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RandomTransform")
	bool bRandomRoll = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RandomTransform", meta = (EditCondition = "bRandomRoll"))
	float MinRoll = -180.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RandomTransform", meta = (EditCondition = "bRandomRoll"))
	float MaxRoll = 180.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RandomTransform")
	bool bRandomScale = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RandomTransform", meta = (EditCondition = "bRandomScale", ClampMin = "0.0001"))
	float MinScale = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RandomTransform", meta = (EditCondition = "bRandomScale", ClampMin = "0.0001"))
	float MaxScale = 1.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RandomTransform")
	bool bRandomLocationXAixs = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RandomTransform", meta = (EditCondition = "bRandomLocationXAixs"))
	float MinLocationX = -100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RandomTransform", meta = (EditCondition = "bRandomLocationXAixs"))
	float MaxLocationX = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RandomTransform")
	bool bRandomLocationYAixs = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RandomTransform", meta = (EditCondition = "bRandomLocationYAixs"))
	float MinLocationY = -100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RandomTransform", meta = (EditCondition = "bRandomLocationYAixs"))
	float MaxLocationY = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RandomTransform")
	bool bRandomLocationZAixs = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RandomTransform", meta = (EditCondition = "bRandomLocationZAixs"))
	float MinLocationZ = -100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RandomTransform", meta = (EditCondition = "bRandomLocationZAixs"))
	float MaxLocationZ = 100.0f;

};

/**
 * 
 */
UCLASS()
class SUPERMANAGER_API UQuickActorActionsWidget : public UEditorUtilityWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "ActorBatchSelection")
	void SelectAllActorsWithSimilarName();	// 选择所有名称相似的Actor

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ActorBatchSelection")
	TEnumAsByte<ESearchCase::Type> SelectionMode = ESearchCase::IgnoreCase;	// 选择模式:默认忽略大小写

	UFUNCTION(BlueprintCallable, Category = "ActorBatchSelection")
	void DuplicateSelectedActors();		// 复制选中的Actor

	// 复制轴向（X、Y、Z）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ActorDuplication")
	E_DuplicateAxis DuplicateAxis = E_DuplicateAxis::EDA_XAxis;	// 复制轴向

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ActorDuplication")
	int DuplicateCount = 1;		// 复制数量

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ActorDuplication")
	float DuplicateOffset = 100.0f;		// 复制偏移

	UFUNCTION(BlueprintCallable, Category = "RandomizeActorTransform")
	void RandomizeSelectedActorsTransform();	// 随机变换选中的Actor

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RandomizeActorTransform")
	FRandomActorTransform RandomTransform;	// 随机变换

private:
	UPROPERTY()
	class UEditorActorSubsystem* EditorActorSubsystem;

	bool GetEditorActorSubsystem();
	
};
