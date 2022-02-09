// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryAssetManager.h"
#include "Inventory/BaseInventoryItem.h"

const FPrimaryAssetType UInventoryAssetManager::PotionItemType = TEXT("Potion");
const FPrimaryAssetType UInventoryAssetManager::SkillItemType = TEXT("Skill");
const FPrimaryAssetType UInventoryAssetManager::WeaponItemType = TEXT("Weapon");

UInventoryAssetManager& UInventoryAssetManager::GetInventoryAssetManager()
{
	UInventoryAssetManager* InventoryAssetManager = Cast<UInventoryAssetManager>(GEngine->AssetManager);

	if(InventoryAssetManager)
	{
		return *InventoryAssetManager;
	}
	else
	{
		UE_LOG(LogTemp, Fatal, TEXT("Invalid AssetManager"))
		return *NewObject<UInventoryAssetManager>();
	}
}

UBaseInventoryItem* UInventoryAssetManager::ForceLoadItem(const FPrimaryAssetId& PrimaryAssetId, bool bLogWarning)
{
	FSoftObjectPath ItemPath = GetPrimaryAssetPath(PrimaryAssetId);
	UBaseInventoryItem* LoadItem = Cast<UBaseInventoryItem>(ItemPath.TryLoad());

	if(bLogWarning && LoadItem == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load item for identifier %s"), *PrimaryAssetId.ToString());
	}

	return LoadItem;
}
