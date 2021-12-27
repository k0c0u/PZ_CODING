// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PZ_04_DestroyActor.generated.h"

////PZ_08/////
UENUM(BlueprintType)
enum class Status : uint8
{
	Stopped     UMETA(DisplayName = "Stopped"),
	Attacking   UMETA(DisplayName = "Attacking"),
};
////PZ_08/////

class ABaseCharacter;
UCLASS()
class PZ_CODING_API APZ_04_DestroyActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APZ_04_DestroyActor();

	////PZ_08/////
	Status CurrentStatus = Status::Attacking;
	////PZ_08/////

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	void DestroyThisActor();
	UFUNCTION()
	void DamagePlayer();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* StaticMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Destroy")
	bool bDestroyForTimer = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Destroy", meta = (EditCondition = "bDestroyForTimer"))
	float TimeToDestroy = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health", meta = (ClampMin = "0.0", ClampMax = "100.0"))
	float Health = 100.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
	float Damage = 10.0f;

	
	FTimerHandle DestroyTimerHandle;

	UPROPERTY()
	ABaseCharacter* BaseCharacter;
};
