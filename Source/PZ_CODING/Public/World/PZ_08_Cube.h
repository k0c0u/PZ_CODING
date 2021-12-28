// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PZ_08_Cube.generated.h"



UCLASS()
class PZ_CODING_API APZ_08_Cube : public AActor
{
	GENERATED_BODY()
	
public:	
	APZ_08_Cube();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	TSubclassOf<AActor> ActorToSpawn;

protected:
	
	virtual void BeginPlay() override;

	void DestroyActor();
	void ChangeStatus();

	TArray<AActor*> ActorArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	int32 Count = 100;

	FTimerHandle DestroyActorTimerHandle;
	FTimerHandle ChangeStatusTimerHandle;
};
