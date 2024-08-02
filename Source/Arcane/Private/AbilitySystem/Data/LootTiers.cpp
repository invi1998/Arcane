// Copyright INVI1998


#include "AbilitySystem/Data/LootTiers.h"

TArray<FLootItem> ULootTiers::GetLootItems() const
{
	TArray<FLootItem> Items;
	for (const FLootItem& Item : LootItems)
	{
		for (int32 i = 0; i < Item.MaxNumberToSpawn; i++)
		{
			// 根据掉落几率计算是否掉落
			if (FMath::FRandRange(1.f, 100.f) <= Item.ChanceToDrop)
			{
				FLootItem NewItem;
				NewItem.LootClass = Item.LootClass;
				NewItem.ChanceToDrop = Item.ChanceToDrop;
				NewItem.MaxNumberToSpawn = Item.MaxNumberToSpawn;
				NewItem.bLootLevelOverride = Item.bLootLevelOverride;

				Items.Add(NewItem);
			}
		}
	}
	return Items;
}
