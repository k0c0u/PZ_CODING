// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Items/PotionItem.h"

#include "Inventory/InventoryAssetManager.h"

UPotionItem::UPotionItem()
{
	ItemType = UInventoryAssetManager::PotionItemType;
}
