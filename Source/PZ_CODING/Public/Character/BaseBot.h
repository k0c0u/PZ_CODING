// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseBot.generated.h"

UCLASS()
class PZ_CODING_API ABaseBot : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseBot();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category=Healt)
	float Health = 0.0f;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category=Speed)
	float Speed = 0.0f;
};
