// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "AuraEffectActor.generated.h"

// ����Ч��Ӧ�ò���
UENUM(BlueprintType)
enum class EEffectApplicationPolicy
{
	ApplyOnOverlap,		// ���ص�ʱӦ��Ч��
	ApplyOnEndOverlap,	// �ڽ����ص�ʱӦ��Ч��
	DoNotApply			// ��Ӧ��Ч��
};

// ����Ч���Ƴ����ԣ�ʵ���ϣ�Ч���Ƴ�һ��ֻ�������Ч������Ϊ˲ʱЧ���ͳ���Ч�������������ʱ�Զ��Ƴ�
UENUM(BlueprintType)
enum class EEffectRemovePolicy
{
	RemoveOnEndOverlap,	// �ڽ����ص�ʱ�Ƴ�Ч��
	DoNotRemove			// ���Ƴ�Ч��
};

UCLASS()
class ARCANE_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AAuraEffectActor();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass);	// ��Ч��Ӧ�õ�Ŀ�꣬����Ŀ���GameplayEffect��

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Aura Effect")
	bool bDestroyOnRemoveEffect = false;	// �Ƴ�Ч��ʱ�Ƿ�����Actor

	/*
	 * ��Ϸ�е�Ч����Ϊ���֣�˲ʱЧ��������Ч��������Ч��
	 */

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	TSubclassOf<UGameplayEffect> InstantGameplayEffectClass;	// ˲ʱ���͵�GameplayEffect��

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	TSubclassOf<UGameplayEffect> DurationGameplayEffectClass;	// �������͵�GameplayEffect��

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	TSubclassOf<UGameplayEffect> InfiniteGameplayEffectClass;	// �������͵�GameplayEffect��

	/*
	 * 3����ϷЧ����Ӧ�ò���
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	EEffectApplicationPolicy InstantEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;	// ˲ʱЧ��Ӧ�ò���

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	EEffectApplicationPolicy DurationEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;	// ����Ч��Ӧ�ò���

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	EEffectApplicationPolicy InfiniteEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;	// (��������)����Ч��Ӧ�ò���

	/*
	 * ��ϷЧ�����Ƴ����� ��ֻ�������Ч��������Ч�������ͨ�����Կ����Ƴ���Ҳ��������ӣ�
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	EEffectRemovePolicy InfiniteEffectRemovePolicy = EEffectRemovePolicy::RemoveOnEndOverlap;		// (��������)����Ч���Ƴ�����

	/*
	 * ��ϷЧ��������ϵͳ�����ӳ��Map����Map���������Ƴ�Ч��
	 */
	TMap<FActiveGameplayEffectHandle, UAbilitySystemComponent*> ActiveGameplayEffectsMap;	// �Ч����� ���ڴ洢��ϷЧ��������ϵͳ�����ӳ��

	/*
	 * ���Ժ���
	 */
	UFUNCTION(BlueprintCallable)
	void OnOverlap(AActor* TargetActor);	// �ص�ʱ�Ĳ��Ժ���

	UFUNCTION(BlueprintCallable)
	void OnEndOverlap(AActor* TargetActor);	// �����ص�ʱ�Ĳ��Ժ���


private:
};
