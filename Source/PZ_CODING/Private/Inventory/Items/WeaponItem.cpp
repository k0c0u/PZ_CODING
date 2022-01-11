// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Items/WeaponItem.h"

#include "Inventory/InventoryAssetManager.h"

UWeaponItem::UWeaponItem()
{
	ItemType = UInventoryAssetManager::WeaponItemType;
}
