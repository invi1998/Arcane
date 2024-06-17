// Copyright INVI1998


#include "AuraGameplayTags.h"

#include "GameplayTagsManager.h"

FAuraGameplayTags FAuraGameplayTags::GameplayTags;	// 静态变量初始化

FString FAuraGameplayTags::GetDamageCnName(const FGameplayTag& Tag)
{
	TMap<FGameplayTag, FString> DamageCnNameMap = {
			{GameplayTags.Damage_Fire, FString(TEXT("火焰伤害"))},
			{GameplayTags.Damage_Ice, FString(TEXT("冰霜伤害"))},
			{GameplayTags.Damage_Lightning, FString(TEXT("闪电伤害"))},
			{GameplayTags.Damage_Poison, FString(TEXT("毒素伤害"))},
			{GameplayTags.Damage_Arcane, FString(TEXT("奥术伤害"))},
			{GameplayTags.Damage_Physical, FString(TEXT("物理伤害"))}
		};

	if (DamageCnNameMap.Contains(Tag))
	{
		return DamageCnNameMap[Tag];
	}
	return FString(TEXT("未知伤害类型"));
}

void FAuraGameplayTags::InitializeNativeGameplayTags()
{
	// 添加原生标签，属性->次要属性->护甲，描述
	// 将这个标签添加到GameplayTagsManager中
	
/*
	 * Primary Attributes
	 */
	GameplayTags.Attributes_Primary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Strength"),
		FString("Increases physical damage")
	);

	GameplayTags.Attributes_Primary_Agility = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Agility"),
		FString("Increases Speed")
	);

	GameplayTags.Attributes_Primary_Intelligence = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Intelligence"),
		FString("Increases magical damage")
	);

	GameplayTags.Attributes_Primary_Resilience = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Resilience"),
		FString("Increases Armor and Armor Penetration")
	);

	GameplayTags.Attributes_Primary_Vigor = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Vigor"),
		FString("Increases Health")
	);

	/*
	 * Secondary Attributes
	 */

	GameplayTags.Attributes_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.Armor"),
		FString("Reduces damage taken, improves Block Chance")
	);

	GameplayTags.Attributes_Secondary_ArmorPenetration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.ArmorPenetration"),
		FString("Ignores Percentage of enemy Armor, increases Critical Hit Chance")
	);

	GameplayTags.Attributes_Secondary_BlockChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.BlockChance"),
		FString("Chance to cut incoming damage in half")
	);

	GameplayTags.Attributes_Secondary_CriticalHitChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalHitChance"),
		FString("Chance to double damage plus critical hit bonus")
	);

	GameplayTags.Attributes_Secondary_CriticalHitDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalHitDamage"),
		FString("Bonus damage added when a critical hit is scored")
	);

	GameplayTags.Attributes_Secondary_CriticalHitResistance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalHitResistance"),
		FString("Reduces Critical Hit Chance of attacking enemies")
	);

	GameplayTags.Attributes_Secondary_HealthRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.HealthRegeneration"),
		FString("Amount of Health regenerated every 1 second")
	);

	GameplayTags.Attributes_Secondary_ManaRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.ManaRegeneration"),
		FString("Amount of Mana regenerated every 1 second")
	);

	GameplayTags.Attributes_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxHealth"),
		FString("Maximum amount of Health obtainable")
	);

	GameplayTags.Attributes_Secondary_MaxMana = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxMana"),
		FString("Maximum amount of Mana obtainable")
	);

	GameplayTags.Attributes_Secondary_Resistance_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.Resistance.Fire"),
		FString("Reduces Fire Damage taken")
	);

	GameplayTags.Attributes_Secondary_Resistance_Ice = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.Resistance.Ice"),
		FString("Reduces Ice Damage taken")
	);

	GameplayTags.Attributes_Secondary_Resistance_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.Resistance.Lightning"),
		FString("Reduces Lightning Damage taken")
	);

	GameplayTags.Attributes_Secondary_Resistance_Poison = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.Resistance.Poison"),
		FString("Reduces Poison Damage taken")
	);

	GameplayTags.Attributes_Secondary_Resistance_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.Resistance.Arcane"),
		FString("Reduces Arcane Damage taken")
	);

	GameplayTags.Attributes_Secondary_Resistance_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.Resistance.Physical"),
		FString("Reduces Physical Damage taken")
	);

	GameplayTags.Attributes_Meta_RewardExperience = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Meta.RewardExperience"),
		FString("Experience Reward")
	);

	/*
	 * Input 按键输入
	 */
	GameplayTags.InputTag_LeftMouseButton = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.LeftMouseButton"),
		FString("Input Tag for the Left Mouse Button")
	);

	GameplayTags.InputTag_RightMouseButton = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.RightMouseButton"),
		FString("Input Tag for the Right Mouse Button")
	);

	GameplayTags.InputTag_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.1"),
		FString("Input Tag for the 1 Key")
	);

	GameplayTags.InputTag_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.2"),
		FString("Input Tag for the 2 Key")
	);

	GameplayTags.InputTag_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.3"),
		FString("Input Tag for the 3 Key")
	);

	GameplayTags.InputTag_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.4"),
		FString("Input Tag for the 4 Key")
	);

	GameplayTags.InputTag_5 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.5"),
		FString("Input Tag for the 5 Key")
	);

	GameplayTags.InputTag_6 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.6"),
		FString("Input Tag for the 6 Key")
	);

	GameplayTags.InputTag_7 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.7"),
		FString("Input Tag for the 7 Key")
	);

	GameplayTags.InputTag_8 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.8"),
		FString("Input Tag for the 8 Key")
	);

	GameplayTags.InputTag_9 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.9"),
		FString("Input Tag for the 9 Key")
	);

	GameplayTags.InputTag_0 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.0"),
		FString("Input Tag for the 0 Key")
	);

	GameplayTags.InputTag_Passive_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Passive.1"),
		FString("Input Tag for the Passive 1 Key")
	);

	GameplayTags.InputTag_Passive_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Passive.2"),
		FString("Input Tag for the Passive 2 Key")
	);

	/*
	* Damage Types
	*/
	GameplayTags.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage"),
		FString("Damage")
	);

	GameplayTags.Damage_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Fire"),
		FString("Fire Damage")
	);

	GameplayTags.Damage_Ice = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Ice"),
		FString("Ice Damage")
	);

	GameplayTags.Damage_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Lightning"),
		FString("Lightning Damage")
	);

	GameplayTags.Damage_Poison = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Poison"),
		FString("Poison Damage")
	);

	GameplayTags.Damage_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Arcane"),
		FString("Arcane Damage")
	);

	GameplayTags.Damage_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Physical"),
		FString("Physical Damage")
	);

	/*
	 *
	 */
	GameplayTags.DamageTypesToResistance.Add(GameplayTags.Damage_Fire, GameplayTags.Attributes_Secondary_Resistance_Fire);
	GameplayTags.DamageTypesToResistance.Add(GameplayTags.Damage_Ice, GameplayTags.Attributes_Secondary_Resistance_Ice);
	GameplayTags.DamageTypesToResistance.Add(GameplayTags.Damage_Lightning, GameplayTags.Attributes_Secondary_Resistance_Lightning);
	GameplayTags.DamageTypesToResistance.Add(GameplayTags.Damage_Poison, GameplayTags.Attributes_Secondary_Resistance_Poison);
	GameplayTags.DamageTypesToResistance.Add(GameplayTags.Damage_Arcane, GameplayTags.Attributes_Secondary_Resistance_Arcane);
	GameplayTags.DamageTypesToResistance.Add(GameplayTags.Damage_Physical, GameplayTags.Attributes_Secondary_Resistance_Physical);

	/*
	 * DamageType Debuff Tags
	 */
	GameplayTags.Debuff_FireBurn = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.FireBurn"),
		FString("Fire Burn Debuff")
	);
	GameplayTags.Debuff_IceFreeze = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.IceFreeze"),
		FString("Ice Freeze Debuff")
	);
	GameplayTags.Debuff_LightningStun = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.LightningStun"),
		FString("Lightning Stun Debuff")
	);
	GameplayTags.Debuff_PoisonVenom = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.PoisonVenom"),
		FString("Poison Venom Debuff")
	);
	GameplayTags.Debuff_ArcaneCurse = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.ArcaneCurse"),
		FString("Arcane Curse Debuff")
	);
	GameplayTags.Debuff_PhysicalBleed = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.PhysicalBleed"),
		FString("Physical Bleed Debuff")
	);

	GameplayTags.Debuff_Chance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.Chance"),
		FString("Debuff Chance")
	);
	GameplayTags.Debuff_Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.Damage"),
		FString("Debuff Damage")
	);
	GameplayTags.Debuff_Frequency = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.Frequency"),
		FString("Debuff Frequency")
	);
	GameplayTags.Debuff_Duration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.Duration"),
		FString("Debuff Duration")
	);

	GameplayTags.DamageTypesToDebuff.Add(GameplayTags.Damage_Fire, GameplayTags.Debuff_FireBurn);
	GameplayTags.DamageTypesToDebuff.Add(GameplayTags.Damage_Ice, GameplayTags.Debuff_IceFreeze);
	GameplayTags.DamageTypesToDebuff.Add(GameplayTags.Damage_Lightning, GameplayTags.Debuff_LightningStun);
	GameplayTags.DamageTypesToDebuff.Add(GameplayTags.Damage_Poison, GameplayTags.Debuff_PoisonVenom);
	GameplayTags.DamageTypesToDebuff.Add(GameplayTags.Damage_Arcane, GameplayTags.Debuff_ArcaneCurse);
	GameplayTags.DamageTypesToDebuff.Add(GameplayTags.Damage_Physical, GameplayTags.Debuff_PhysicalBleed);

	/*
	 * Effect Tags
	 */
	GameplayTags.Effect_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Effect.HitReact"),
		FString("Hit granted when Hit Reacting")		// 受击反应
	);

	GameplayTags.Effect_DeathReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Effect.DeathReact"),
		FString("Hit granted when Death Reacting")		// 死亡反应
	);

	/*
	 * Ability Tags
	 */
	GameplayTags.Abilities_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Attack"),
		FString("Attack Ability")
	);
	GameplayTags.Abilities_Call = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Call"),
		FString("Call Ability")
	);
	GameplayTags.Abilities_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.HitReact"),
		FString("Hit React Ability")
	);

	GameplayTags.Abilities_None = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.None"),
		FString("No Ability")
	);

	/*
	 * Passive Ability Tags
	 */
	GameplayTags.Abilities_Passive_HaloOfProtection = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Passive.HaloOfProtection"),
		FString("HaloOfProtection Ability")
	);

	GameplayTags.Abilities_Passive_LifeSiphon = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Passive.LifeSiphon"),
		FString("LifeSiphon Ability")
	);

	GameplayTags.Abilities_Passive_ManaSiphon = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Passive.ManaSiphon"),
		FString("ManaSiphon Ability")
	);

	/*
	 * Fire Ability Tags
	 */
	GameplayTags.Abilities_Fire_FireBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Fire.FireBolt"),
		FString("FireBolt Ability")
	);
	GameplayTags.Abilities_Fire_FireBlast = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Fire.FireBlast"),
		FString("FireBlast Ability")
	);
	GameplayTags.Abilities_Fire_FireNova = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Fire.FireNova"),
		FString("FireNova Ability")
	);
	GameplayTags.Abilities_Fire_FireStorm = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Fire.FireStorm"),
		FString("FireStorm Ability")
	);

	/*
	 * Ice Ability Tags
	 */
	GameplayTags.Abilities_Ice_Freeze = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Ice.Freeze"),
		FString("Freeze Ability")
	);
	GameplayTags.Abilities_Ice_FrostBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Ice.FrostBolt"),
		FString("FrostBolt Ability")
	);
	GameplayTags.Abilities_Ice_Blade = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Ice.Blade"),
		FString("Blade Ability")
	);
	GameplayTags.Abilities_Ice_Blizzard = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Ice.Blizzard"),
		FString("Blizzard Ability")
	);

	/*
	 * Poison Ability Tags
	 */
	GameplayTags.Abilities_Poison_PoisonDart = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Poison.PoisonDart"),
		FString("PoisonDart Ability")
	);
	GameplayTags.Abilities_Poison_Venom = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Poison.Venom"),
		FString("Venom Ability")
	);
	GameplayTags.Abilities_Poison_ToxicCloud = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Poison.ToxicCloud"),
		FString("ToxicCloud Ability")
	);
	GameplayTags.Abilities_Poison_PoisonNova = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Poison.PoisonNova"),
		FString("PoisonNova Ability")
	);

	/*
	 * Lightning Ability Tags
	 *
	 */
	GameplayTags.Abilities_Lightning_Electrocute = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Lightning.Electrocute"),
		FString("Electrocute Ability")
	);
	GameplayTags.Abilities_Lightning_Thunder = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Lightning.Thunder"),
		FString("Thunder Ability")
	);
	GameplayTags.Abilities_Lightning_Chain = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Lightning.Chain"),
		FString("Chain Ability")
	);
	GameplayTags.Abilities_Lightning_Storm = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Lightning.Storm"),
		FString("Storm Ability")
	);

	/*
	 * Arcane Ability Tags
	 */
	GameplayTags.Abilities_Arcane_ArcaneMissile = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Arcane.ArcaneMissile"),
		FString("ArcaneMissile Ability")
	);
	GameplayTags.Abilities_Arcane_Spellbind = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Arcane.Spellbind"),
		FString("Spellbind Ability")
	);
	GameplayTags.Abilities_Arcane_Shield = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Arcane.Shield"),
		FString("Shield Ability")
	);
	GameplayTags.Abilities_Arcane_Lgnition = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Arcane.Lgnition"),
		FString("Lgnition Ability")
	);

	/*
	 * Physical Ability Tags
	 */
	GameplayTags.Abilities_Physical_Slash = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Physical.Slash"),
		FString("Slash Ability")
	);
	GameplayTags.Abilities_Physical_Stab = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Physical.Stab"),
		FString("Stab Ability")
	);
	GameplayTags.Abilities_Physical_Swing = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Physical.Swing"),
		FString("Swing Ability")
	);
	GameplayTags.Abilities_Physical_Thrust = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Physical.Thrust"),
		FString("Thrust Ability")
	);

	/*
	 * Ability State Tags
	 */
	GameplayTags.Abilities_State_Locked = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.State.Locked"),
		FString("Ability is Locked")
	);

	GameplayTags.Abilities_State_Eligible = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.State.Eligible"),
		FString("Ability is Eligible")
	);

	GameplayTags.Abilities_State_UnLocked = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.State.UnLocked"),
		FString("Ability is UnLocked")
	);

	GameplayTags.Abilities_State_Equipped = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.State.Equipped"),
		FString("Ability is Equipped")
	);

	/*
	 * Ability Type Tags
	 *
	 */
	GameplayTags.Abilities_Type_Offensive = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Type.Offensive"),
		FString("Offensive Ability")
	);

	GameplayTags.Abilities_Type_Passive = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Type.Passive"),
		FString("Passive Ability")
	);

	GameplayTags.Abilities_Type_None = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Type.None"),
		FString("No Ability")
	);


	/*
	 * Cooldown Tags
	 */
	GameplayTags.Cooldown_Fire_FireBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Fire.FireBolt"),
		FString("FireBolt Cooldown")
	);
	GameplayTags.Cooldown_Fire_FireBlast = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Fire.FireBlast"),
		FString("FireBlast Cooldown")
	);
	GameplayTags.Cooldown_Fire_FireNova = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Fire.FireNova"),
		FString("FireNova Cooldown")
	);
	GameplayTags.Cooldown_Fire_FireStorm = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Fire.FireStorm"),
		FString("FireStorm Cooldown")
	);
	GameplayTags.Cooldown_Ice_Freeze = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Ice.Freeze"),
		FString("Freeze Cooldown")
	);
	GameplayTags.Cooldown_Ice_FrostBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Ice.FrostBolt"),
		FString("FrostBolt Cooldown")
	);
	GameplayTags.Cooldown_Ice_Blade = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Ice.Blade"),
		FString("Blade Cooldown")
	);
	GameplayTags.Cooldown_Ice_Blizzard = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Ice.Blizzard"),
		FString("Blizzard Cooldown")
	);
	GameplayTags.Cooldown_Lightning_Electrocute = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Lightning.Electrocute"),
		FString("Electrocute Cooldown")
	);
	GameplayTags.Cooldown_Lightning_Thunder = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Lightning.Thunder"),
		FString("Thunder Cooldown")
	);
	GameplayTags.Cooldown_Lightning_Chain = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Lightning.Chain"),
		FString("Chain Cooldown")
	);
	GameplayTags.Cooldown_Lightning_Storm = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Lightning.Storm"),
		FString("Storm Cooldown")
	);
	GameplayTags.Cooldown_Arcane_ArcaneMissile = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Arcane.ArcaneMissile"),
		FString("ArcaneMissile Cooldown")
	);
	GameplayTags.Cooldown_Arcane_Spellbind = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Arcane.Spellbind"),
		FString("Spellbind Cooldown")
	);
	GameplayTags.Cooldown_Arcane_Shield = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Arcane.Shield"),
		FString("Shield Cooldown")
	);
	GameplayTags.Cooldown_Arcane_Lgnition = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Arcane.Lgnition"),
		FString("Lgnition Cooldown")
	);
	GameplayTags.Cooldown_Poison_PoisonDart = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Poison.PoisonDart"),
		FString("PoisonDart Cooldown")
	);
	GameplayTags.Cooldown_Poison_Venom = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Poison.Venom"),
		FString("Venom Cooldown")
	);
	GameplayTags.Cooldown_Poison_ToxicCloud = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Poison.ToxicCloud"),
		FString("ToxicCloud Cooldown")
	);
	GameplayTags.Cooldown_Poison_PoisonNova = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Poison.PoisonNova"),
		FString("PoisonNova Cooldown")
	);
	GameplayTags.Cooldown_Physical_Slash = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Physical.Slash"),
		FString("Slash Cooldown")
	);
	GameplayTags.Cooldown_Physical_Stab = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Physical.Stab"),
		FString("Stab Cooldown")
	);
	GameplayTags.Cooldown_Physical_Swing = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Physical.Swing"),
		FString("Swing Cooldown")
	);
	GameplayTags.Cooldown_Physical_Thrust = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Physical.Thrust"),
		FString("Thrust Cooldown")
	);

	/*
	 * Montage Tags
	 */
	GameplayTags.Montage_Attack_Weapon = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.Weapon"),
		FString("Attack Montage for Weapon")
	);

	GameplayTags.Montage_Attack_LeftWeapon = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.LeftWeapon"),
		FString("Attack Montage for Left Weapon")
	);

	GameplayTags.Montage_Attack_LeftWeaponLeftSwing = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.LeftWeaponLeftSwing"),
		FString("Attack Montage for Left Weapon Left Swing")
	);

	GameplayTags.Montage_Attack_LeftWeaponRightSwing = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.LeftWeaponRightSwing"),
		FString("Attack Montage for Left Weapon Right Swing")
	);

	GameplayTags.Montage_Attack_RightWeapon = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.RightWeapon"),
		FString("Attack Montage for Right Weapon")
	);

	GameplayTags.Montage_Attack_RightWeaponLeftSwing = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.RightWeaponLeftSwing"),
		FString("Attack Montage for Right Weapon Left Swing")
	);

	GameplayTags.Montage_Attack_RightWeaponRightSwing = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.RightWeaponRightSwing"),
		FString("Attack Montage for Right Weapon Right Swing")
	);

	GameplayTags.Montage_Attack_Unarmed = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.Unarmed"),
		FString("Attack Montage for Unarmed")
	);

	GameplayTags.Montage_Attack_LeftUnarmed = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.LeftUnarmed"),
		FString("Attack Montage for Left Unarmed")
	);

	GameplayTags.Montage_Attack_RightUnarmed = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.RightUnarmed"),
		FString("Attack Montage for Right Unarmed")
	);

	GameplayTags.Montage_Attack_Bow = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.Bow"),
		FString("Attack Montage for Bow")
	);

	GameplayTags.Montage_Attack_LichCall = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.LichCall"),
		FString("Attack Montage for LichCall")
	);

	GameplayTags.Montage_Attack_LichSpell = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.LichSpell"),
		FString("Attack Montage for LichSpell")
	);

	/*
	 * Player Block Tags（阻挡输入）
	 */
	GameplayTags.Player_Block_InputPressed = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Player.Block.InputPressed"),
		FString("Player Press is Blocking")
	);

	GameplayTags.Player_Block_InputReleased = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Player.Block.InputReleased"),
		FString("Player Release is Blocking")
	);

	GameplayTags.Player_Block_InputHeld = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Player.Block.InputHeld"),
		FString("Player is Holding Block")
	);

	GameplayTags.Player_Block_CursorTrace = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Player.Block.CursorTrace"),
		FString("Player is Blocking Cursor Trace")
	);
}
