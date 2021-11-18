#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "PZ_05_BoxCollision.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnChangeColorDelegate);
DECLARE_DYNAMIC_DELEGATE(FOnChangeLocationDelegate);


UCLASS()
class PZ_CODING_API APZ_05_BoxCollision : public AActor
{
	GENERATED_BODY()
	
public:	
	
	APZ_05_BoxCollision();

	UPROPERTY(BlueprintAssignable, BlueprintCallable,Category="Collision")
	FOnChangeColorDelegate OnChangeColor;

	FOnChangeLocationDelegate OnChangeLocation;

protected:
	
	virtual void BeginPlay() override;

	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void ChangeLocation();
	
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* BoxComponent = nullptr;
};



