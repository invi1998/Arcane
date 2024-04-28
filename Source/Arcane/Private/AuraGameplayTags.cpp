// Copyright INVI1998


#include "AuraGameplayTags.h"

#include "GameplayTagsManager.h"

FAuraGameplayTags FAuraGameplayTags::GameplayTags;	// 静态变量初始化

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
	GameplayTags.Abilities_Fire_FireBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Fire.FireBolt"),
		FString("FireBolt Ability")
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

}
