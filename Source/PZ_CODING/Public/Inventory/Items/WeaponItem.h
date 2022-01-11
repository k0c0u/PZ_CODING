// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/BaseInventoryItem.h"
#include "WeaponItem.generated.h"

/**
 * 
 */
UCLASS()
class PZ_CODING_API UWeaponItem : public UBaseInventoryItem
{
	GENERATED_BODY()

public:

	UWeaponItem();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon")
	TSubclassOf<AActor> WeaponActor;
};
