// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "PZ_07_SpyCameraActor.generated.h"


 class APZ_07_BasePawn;
 
UCLASS()
class PZ_CODING_API APZ_07_SpyCameraActor : public ACameraActor
{
	GENERATED_BODY()

public:
	
	APZ_07_SpyCameraActor();

protected:
	
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	void BoxBeginOverlap (UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void BoxEndOverlap (UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void DetectionPlayer();
	
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* BoxComp;
	
	FTimerHandle DetectionTimer;

	APZ_07_BasePawn* BasePawn = nullptr;
};
