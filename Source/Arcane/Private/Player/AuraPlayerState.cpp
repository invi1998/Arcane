// Copyright INVI1998


#include "Player/AuraPlayerState.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"

AAuraPlayerState::AAuraPlayerState()
{
	NetUpdateFrequency = 100.0f;		// 设置客户端每秒100次更新，服务端尝试更新客户端的频率

	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>(TEXT("AbilitySystemComponent"));	// 创建能力系统组件
	AbilitySystemComponent->SetIsReplicated(true);	// 设置为可复制
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);	// 设置复制模式，混合模式，对于多人游戏来说，特别是玩家控制的角色，设置为Mixed是最好的选择

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>(TEXT("AttributeSet"));		// 创建属性集

}

UAbilitySystemComponent* AAuraPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAttributeSet* AAuraPlayerState::GetAttributeSet() const
{
	return AttributeSet;
}
