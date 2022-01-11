// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Styling/SlateBrush.h"
#include "BaseInventoryItem.generated.h"

/**
 * 
 */
UCLASS(Abstract, BlueprintType)
class PZ_CODING_API UBaseInventoryItem : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Item")
	FPrimaryAssetType ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item")
	FText ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item")
	FText ItemDescription;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item")
	FSlateBrush ItemIcon;

	UFUNCTION(BlueprintCallable)
	FString GetIdentifierString();

	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
};
