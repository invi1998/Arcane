#include "AuraAbilityTypes.h"

bool FAuraGameplayEffectContext::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
	uint32 RepBits = 0;
	
	if (Ar.IsSaving())
	{
		if (bReplicateInstigator && Instigator.IsValid())
		{
			RepBits |= 1 << 0;
		}
		if (bReplicateEffectCauser && EffectCauser.IsValid())
		{
			RepBits |= 1 << 1;
		}
		if (AbilityCDO.IsValid())
		{
			RepBits |= 1 << 2;
		}
		if (bReplicateSourceObject && SourceObject.IsValid())
		{
			RepBits |= 1 << 3;
		}
		if (Actors.Num() > 0)
		{
			RepBits |= 1 << 4;
		}
		if (HitResult.IsValid())
		{
			RepBits |= 1 << 5;
		}
		if (bHasWorldOrigin)
		{
			RepBits |= 1 << 6;
		}
		if (bIsBlockedHit)	// 是否格挡
		{
			RepBits |= 1 << 7;
		}
		if (bIsCriticalHit)	// 是否暴击
		{
			RepBits |= 1 << 8;
		}
		if (bIsSuccessfulDebuff)	// 是否成功施加Debuff
		{
			RepBits |= 1 << 9;
		}
		if (DebuffDamage > 0)
		{
			RepBits |= 1 << 10;
		}
		if (DebuffFrequency > 0)
		{
			RepBits |= 1 << 11;
		}
		if (DebuffDuration > 0)
		{
			RepBits |= 1 << 12;
		}
		if (DamageType.IsValid())
		{
			RepBits |= 1 << 13;
		}
		// 判断死亡冲量是否是非零向量，是才序列化
		if (!DeathImpulse.IsZero())
		{
			RepBits |= 1 << 14;
		}
		if (!KnockbackImpulse.IsZero())
		{
			RepBits |= 1 << 15;
		}
		if (bIsRadialDamage)
		{
			RepBits |= 1 << 16;

			if (RadialDamageInnerRadius > 0)
			{
				RepBits |= 1 << 17;
			}
			if (RadialDamageOuterRadius > 0)
			{
				RepBits |= 1 << 18;
			}
			if (!RadialDamageOrigin.IsZero())
			{
				RepBits |= 1 << 19;
			}
		}
	}

	Ar.SerializeBits(&RepBits, 19);

	if (RepBits & (1 << 0))
	{
		Ar << Instigator;
	}
	if (RepBits & (1 << 1))
	{
		Ar << EffectCauser;
	}
	if (RepBits & (1 << 2))
	{
		Ar << AbilityCDO;
	}
	if (RepBits & (1 << 3))
	{
		Ar << SourceObject;
	}
	if (RepBits & (1 << 4))
	{
		SafeNetSerializeTArray_Default<31>(Ar, Actors);
	}
	if (RepBits & (1 << 5))
	{
		if (Ar.IsLoading())
		{
			if (!HitResult.IsValid())
			{
				HitResult = TSharedPtr<FHitResult>(new FHitResult());
			}
		}
		HitResult->NetSerialize(Ar, Map, bOutSuccess);
	}
	if (RepBits & (1 << 6))
	{
		Ar << WorldOrigin;
		bHasWorldOrigin = true;
	}
	else
	{
		bHasWorldOrigin = false;
	}

	if (RepBits & (1 << 7))
	{
		Ar << bIsBlockedHit;
	}
	if (RepBits & (1 << 8))
	{
		Ar << bIsCriticalHit;
	}
	if (RepBits & (1 << 9))
	{
		Ar << bIsSuccessfulDebuff;
	}
	if (RepBits & (1 << 10))
	{
		Ar << DebuffDamage;
	}
	if (RepBits & (1 << 11))
	{
		Ar << DebuffFrequency;
	}
	if (RepBits & (1 << 12))
	{
		Ar << DebuffDuration;
	}
	if (RepBits & (1 << 13))
	{
		if (Ar.IsLoading())
		{
			if (!DamageType.IsValid())
			{
				DamageType = TSharedPtr<FGameplayTag>(new FGameplayTag());
			}
		}
		DamageType->NetSerialize(Ar, Map, bOutSuccess);
	}
	if (RepBits & (1 << 14))
	{
		// 虽然DeathImpulse和其他结构体变量一样，都是结构体
		// 但是在反序列化时，我们不需要像其他结构体一样，通过传递指针的方式来序列化
		// 因为DeathImpulse是Vector类型，而Vector类型是可以直接序列化的，它内部有独特的NetSerialize方法，我们只需要调用即可
		DeathImpulse.NetSerialize(Ar, Map, bOutSuccess);
	}
	if (RepBits & (1 << 15))
	{
		KnockbackImpulse.NetSerialize(Ar, Map, bOutSuccess);
	}
	if (RepBits & (1 << 16))
	{
		Ar << bIsRadialDamage;
	}
	if (RepBits & (1 << 17))
	{
		Ar << RadialDamageInnerRadius;
	}
	if (RepBits & (1 << 18))
	{
		Ar << RadialDamageOuterRadius;
	}
	if (RepBits & (1 << 19))
	{
		RadialDamageOrigin.NetSerialize(Ar, Map, bOutSuccess);
	}

	if (Ar.IsLoading())
	{
		AddInstigator(Instigator.Get(), EffectCauser.Get()); // Just to initialize InstigatorAbilitySystemComponent
	}

	bOutSuccess = true;
	return true;
}
