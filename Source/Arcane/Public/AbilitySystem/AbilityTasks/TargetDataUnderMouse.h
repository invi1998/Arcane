// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TargetDataUnderMouse.generated.h"

// �㲥ί�У��㲥����Ϊ���λ�ã�����ΪFVector����
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMouseTargetDataSignature, const FVector&, Data);

/**
 * 
 */
UCLASS()
class ARCANE_API UTargetDataUnderMouse : public UAbilityTask
{
	GENERATED_BODY()

public: 
	/**
	 * @brief ����������ʵ�������صĹ�������
	 * @param OwningAbility	ӵ�д����������
	 * @return 
	 */
	// UFUNCTION����ͼ���õĺ���������һ��UTargetDataUnderMouse*���͵�ָ�룬����Ϊ"Ability|Tasks"��
	// DisplayNameΪ"TargetDataNuderMouse"����ʾ����ͼ����ʾ������Ϊ"TargetDataNuderMouse"
	// ����PinΪOwningAbility����ʾ������ͼ����ʾOwningAbility���Pin�����ţ�
	// DefaultToSelfΪOwningAbility����ʾĬ��ֵΪOwningAbility
	// BlueprintInternalUseOnlyΪtrue����ʾֻ������ͼ��ʹ��
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName="TargetDataNuderMouse", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true"))
	static UTargetDataUnderMouse* CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility);

	UPROPERTY(BlueprintAssignable)
	FMouseTargetDataSignature ValidData;	// ����һ��ί�У�ί������ΪFVector���ͣ��⽫��Ϊ�ڵ��ϵ����ִ������

private:
	virtual void Activate() override;	// ��дActivate��������������
	
};
