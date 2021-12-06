// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PZ_07_BasePawn.h"
#include "Components/SphereComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APZ_07_BasePawn::APZ_07_BasePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComp->SetSphereRadius(50.0f);
	SphereComp->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	SetRootComponent(SphereComp);

	MovementComp = CreateDefaultSubobject<UPawnMovementComponent, UFloatingPawnMovement>(TEXT("MovementComponent"));
	MovementComp->SetUpdatedComponent(SphereComp);
	
	bUseControllerRotationPitch = true;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;
}

// Called when the game starts or when spawned
void APZ_07_BasePawn::BeginPlay()
{
	Super::BeginPlay();

	APlayerCameraManager* CM = UGameplayStatics::GetPlayerCameraManager(this, 0);
	CM->OnBlendComplete().AddUObject(this, &APZ_07_BasePawn::CameraBlendComplete);
	ViewActor = CM->GetViewTargetPawn();
}

void APZ_07_BasePawn::MoveForward(float Value)
{
	if(Value != 0.0f)
	{
		AddMovementInput(ViewActor->GetActorForwardVector(), Value);
	}
}

void APZ_07_BasePawn::MoveRight(float Value)
{
	if(Value != 0.0f)
	{
		AddMovementInput(ViewActor->GetActorRightVector(), Value);
	}
}

void APZ_07_BasePawn::CameraBlendComplete()
{
	ViewActor = GetController()->GetViewTarget();
}

// Called to bind functionality to input
void APZ_07_BasePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);
	
	PlayerInputComponent->BindAxis("MoveForward", this, &APZ_07_BasePawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APZ_07_BasePawn::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
}

