// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PZ_07_BasePawn.generated.h"

class USphereComponent;
class UPawnMovementComponent;

UCLASS()
class PZ_CODING_API APZ_07_BasePawn : public APawn
{
	GENERATED_BODY()

public:
	
	APZ_07_BasePawn();

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);

	void CameraBlendComplete();
	
	UPROPERTY(VisibleAnywhere)
	USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere)
	UPawnMovementComponent* MovementComp;

	UPROPERTY(Transient)
	AActor* ViewActor = nullptr;
};
