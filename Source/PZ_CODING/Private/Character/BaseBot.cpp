// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BaseBot.h"

// Sets default values
ABaseBot::ABaseBot()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABaseBot::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseBot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseBot::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

