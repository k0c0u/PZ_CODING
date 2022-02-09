// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/BaseInventoryItem.h"

FString UBaseInventoryItem::GetIdentifierString()
{
	return GetPrimaryAssetId().ToString();
}

FPrimaryAssetId UBaseInventoryItem::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(ItemType, GetFName());;
}
