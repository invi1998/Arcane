// Copyright INVI1998


#include "AbilitySystem/AbilityTasks/TargetDataUnderMouse.h"

#include "AbilitySystemComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/AuraPlayerController.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	// ����һ��UTargetDataUnderMouse*���͵�ָ��
	UTargetDataUnderMouse* MyObj = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);

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

		const FGameplayAbilitySpecHandle AbilitySpecHandle = GetAbilitySpecHandle();	// ��ȡAbilitySpecHandle, ���ڱ�ʶAbility
		const FPredictionKey ActivationPredictionKey = GetActivationPredictionKey();	// ��ȡActivationPredictionKey, ���ڱ�ʶAbility�ļ���Ԥ��

		// �������ǿ���ʹ��AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate()�������ͻ��˵�MouseTargetData
		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(AbilitySpecHandle, ActivationPredictionKey).AddUObject(this, &UTargetDataUnderMouse::OnTargetDataReplicatedCallback);	// ���һ��ί�У����ͻ��˹㲥MouseTargetDataʱ���ͻ����OnTargetDataReplicatedCallback����

		// һ���������ϵ����˼���������Ϳ��Խ���ص��󶨵����ί���ϣ��������ͻ��˹㲥MouseTargetDataʱ���ͻ����OnTargetDataReplicatedCallback����

		// ������������Ѿ���������Ŀ�������Ѿ����㲥�ˣ���ô��������£�������ȻӦ�õ��øûص�
		// ���ԣ���һ�ַ������Լ��������ٿ������Ѿ����յ�Ŀ������ʱ���ø�Ŀ�������ί�У��������ǿ���ͨ��CallReplicatedTargetDataDelegatesIfSet������Ƿ��Ѿ����յ�Ŀ�����ݣ��������û�е������ί�У��Ǿ���ζ������û�е��������������������Ҫ�����ȴ�
		const bool bCalledDelegate = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(AbilitySpecHandle, ActivationPredictionKey);		// ����Ѿ����յ�Ŀ�����ݣ���ô�͵��ø�Ŀ�������ί��
		if (!bCalledDelegate)
		{
			// ���û�н��յ�Ŀ�����ݣ���ô�ͼ����ȴ�
			SetWaitingOnRemotePlayerData();
		}
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

void UTargetDataUnderMouse::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle,
	FGameplayTag ActivationTag)
{
	// һ�������������������ζ�������Ѿ����յ��� replicated Ŀ�����ݣ���������֪����replicated ֻ��ӷ���˵��ͻ��ˣ�
	// ������GAS�У���ᷢ�֣��ͻ���Ҳ���Է��� replicated Ŀ�����ݣ��������һ�����ӣ������ڿͻ��˷����� replicated Ŀ�����ݣ�Ȼ�����˽��յ����������
	// ���ԣ������յ� replicated Ŀ������ʱ���˺������ڷ���˵���

	// ���ԣ�������Ҫ������㲥Ŀ�����ݣ����ǣ�����ͬʱ����Ҫȷ��ASC��AbilitySystemComponent���Ѿ�֪��������ݱ����յ���
	// ��Ϊ�����������յ�ReplicatedĿ������ʱ�����Ὣ���ݴ洢��ASC��AbilityTargetDataMap�У�
	// �������ʱ�����ǾͿ��Ը���ASC�����Ѿ����յ���Ŀ�����ݣ�����ASC��ConsumeClientReplicatedTargetData����������AbilitySpecHandle��ActivationPredictionKey
	// ��ASC�����ٴ洢�������
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());	// ����ASC�����Ѿ����յ���Ŀ������

	// Ȼ�����ǾͿ��Թ㲥Ŀ��������
	// һ���ģ��㲥֮ǰ����Ӧ���ж��Ƿ��кϷ���Ŀ�����ݣ�������������Ѿ����ټ����ô�Ͳ���Ҫ�㲥��
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}

}
