// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Items/SkillItem.h"

#include "Inventory/InventoryAssetManager.h"

USkillItem::USkillItem()
{
	ItemType = UInventoryAssetManager::SkillItemType;
}
