// Copyright INVI1998


#include "AbilitySystem/AbilityTasks/TargetDataUnderMouse.h"

#include "AbilitySystemComponent.h"
#include "Player/AuraPlayerController.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	// ����һ��UTargetDataUnderMouse*���͵�ָ��
	UTargetDataUnderMouse* MyObj = NewObject<UTargetDataUnderMouse>(OwningAbility);
	// �������ָ��
	return MyObj;
}

void UTargetDataUnderMouse::Activate()
{
	const bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();
	if (bIsLocallyControlled)
	{
		SendMouseTargetData();
	}
	else
	{
		// TODO: �ڷ���ˣ������ͻ��˵�MouseTargetData
	}

}

void UTargetDataUnderMouse::SendMouseTargetData()
{
	// ����һ��FScopedPredictionWindow�������ڹ���Ԥ�ⴰ�ڣ��������������������ʱ�Զ����٣�����Ҫ����һ��AbilitySystemComponentָ�룬�Լ�һ���Ƿ�ʹ��Ԥ��Ĳ���ֵ����������ʹ��Ԥ�⣬���Դ���true��Ĭ��ֵ��
	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get());

	// ��ǰ�������ڵĴ��뽫ʹ��Ԥ�⣬����ζ�����ǿ����ڿͻ�����ִ����Щ���룬������ȴ�����������Ӧ��
	// ������˵�֪���Ԥ��ʱ�������ڷ������ִ����ͬ�Ĵ��룬Ȼ��ȽϽ������������ͬ����ô����˻�����ͻ��˵�Ԥ�⣬�����ͱ�֤�˿ͻ��˺ͷ���˵�һ����

	// �����̳��� FGameplayAbilityTargetData�������������� Ability ϵͳ��Ŀ��ѡ��Ͷ�λ��Ϣ�Ļ����ṹ
	FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();	// ����һ���µ�FGameplayAbilityTargetData_SingleTargetHit����

	FGameplayAbilityTargetDataHandle DataHandle;	// ����һ��FGameplayAbilityTargetDataHandle����

	AAuraPlayerController* PC = Cast<AAuraPlayerController>(Ability->GetCurrentActorInfo()->PlayerController.Get());	// ��ȡ��ҿ�����
	Data->HitResult = PC->GetCursorHitResult();	// ��ȡ��ҿ������Ĺ�����н��
	DataHandle.Add(Data);	// ��Data��ӵ�DataHandle��

	FGameplayTag ApplicationTag;

	// �ͻ��˵���ServerSetReplicatedTargetData����DataHandle���ݸ������
	AbilitySystemComponent->ServerSetReplicatedTargetData(
		GetAbilitySpecHandle(),		// ��ȡAbilitySpecHandle, ���ڱ�ʶAbility
		GetActivationPredictionKey(),	// ��ȡActivationPredictionKey, ���ڱ�ʶAbility�ļ���Ԥ��
		DataHandle,		// ����DataHandle, ���ڴ���Ŀ�����ݣ������ǹ�����н�������������DataHandle.Add(Data);
		ApplicationTag,		// ����ApplicationTag, ���ڱ�ʶAbility��Ӧ�ñ�ǩ
		AbilitySystemComponent->ScopedPredictionKey	// ����ScopedPredictionKey, ���ڱ�ʶAbility��Ԥ�����Scoped��ζ�����Ԥ���ֻ�����Ability����Ч�������������Ǵ��������ScopedPrediction�������������
	);

	// �㲥����ϵͳ��Ŀ������
	// ���ǹ㲥֮ǰ����Ҫ���ж��Ƿ��кϷ���Ŀ�����ݣ�������������Ѿ����ټ����ô�Ͳ���Ҫ�㲥��
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		// ����Broadcast��Ҫ����һ��FVector���͵Ĳ������������������Ϊ����DataHandle����������UTargetDataUnderMouse.h�иı���ValidData������ΪFGameplayAbilityTargetDataHandle
		// �������Ǿ��ܼ���ͨ���㲥��������DataHandle�����Ի�ȡ���н����Ŀ�������а������κ��������ݣ�Ȼ����Ability��ͨ��ί�еĻص������л�ȡDataHandle
		ValidData.Broadcast(DataHandle);
	}
}
