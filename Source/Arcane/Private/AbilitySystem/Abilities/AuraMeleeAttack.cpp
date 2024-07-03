// Copyright INVI1998


#include "AbilitySystem/Abilities/AuraMeleeAttack.h"

#include "Character/AuraCharacter.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/KismetSystemLibrary.h"

TArray<FVector> UAuraMeleeAttack::GenerateAttackTraceSockets(const FName& SocketName, bool bWeapon, float ForwardOffset, int32 Num, float TotalLength)
{
	TArray<FVector> TraceSockets;

	if (CurrentActorInfo)
	{
		const AAuraCharacter* OwnerCharacter = Cast<AAuraCharacter>(CurrentActorInfo->AvatarActor.Get());

		if (OwnerCharacter && OwnerCharacter->Implements<UCombatInterface>())
		{
			const USkeletalMeshComponent* Mesh = bWeapon ? ICombatInterface::Execute_GetWeaponMesh(OwnerCharacter) : OwnerCharacter->GetMesh();

			if (Mesh)
			{
				const FVector SocketLocation = Mesh->GetSocketLocation(SocketName);

				// 获取插槽的前向向量（及得归一化）
				const FVector ForwardVector = Mesh->GetSocketRotation(SocketName).Vector().GetSafeNormal();

				//// 绘制插槽位置
				//UKismetSystemLibrary::DrawDebugSphere(GetWorld(), SocketLocation, 10.f, 12, FLinearColor::Red, 5.f);
				//// 绘制插槽前向向量
				//UKismetSystemLibrary::DrawDebugArrow(GetWorld(), SocketLocation, SocketLocation + ForwardVector * 100.f, 100.f, FLinearColor::Green, 5.f);

				// 基于偏移量计算新的插值起始位置
				const FVector StartLocation = SocketLocation + (ForwardVector * ForwardOffset);

				if (Num == 1)
				{
					TraceSockets.Add(StartLocation);
				}
				else if (Num > 1)
				{
					// 生成一组插槽位置
					const float offset = TotalLength / (Num - 1);
					for (int32 i = 0; i < Num; i++)
					{
						TraceSockets.Add(StartLocation + (ForwardVector * offset * i));
					}
				}
				
			}
		}
	}

	return TraceSockets;
}
