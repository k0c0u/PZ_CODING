// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseInventoryItem.h"
#include "Engine/AssetManager.h"
#include "InventoryAssetManager.generated.h"

class UBaseInventoryItem;


UCLASS()
class PZ_CODING_API UInventoryAssetManager : public UAssetManager
{
	GENERATED_BODY()
	
public:
	/** Static types for items */
	static const FPrimaryAssetType PotionItemType;
	static const FPrimaryAssetType SkillItemType;
	static const FPrimaryAssetType WeaponItemType;

	/** Returns the current AssetManager object */
	static UInventoryAssetManager& GetInventoryAssetManager();

	/**
	 * Synchronously loads an InvItem subclass, this can hitch but is useful when you cannot wait for an async load
	 * This does not maintain a reference to the item so it will garbage collect if not loaded some other way
	 *
	 * @param PrimaryAssetId The asset identifier to load
	 * @param bLogWarning If true, this will log a warning if the item failed to load
	 */
	UBaseInventoryItem* ForceLoadItem(const FPrimaryAssetId& PrimaryAssetId, bool bLogWarning = true);
};
