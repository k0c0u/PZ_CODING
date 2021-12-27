#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "PZ_05_BoxCollision.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnChangeColorDelegate);
DECLARE_DYNAMIC_DELEGATE(FOnChangeLocationDelegate);

class ABaseCharacter;

UCLASS()
class PZ_CODING_API APZ_05_BoxCollision : public AActor
{
	GENERATED_BODY()
	
public:	
	
	APZ_05_BoxCollision();

	/*UPROPERTY(BlueprintAssignable, BlueprintCallable,Category="Collision")
	FOnChangeColorDelegate OnChangeColor;

	FOnChangeLocationDelegate OnChangeLocation;*/

protected:
	
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	/*UFUNCTION()
	void ChangeLocation();*/
	
	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	UFUNCTION()
	void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* BoxComponent = nullptr;

	////PZ_08/////
	UFUNCTION()
	void DamagePlayer(ABaseCharacter* BaseCharacter);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float Damage = 2.0f;

	FTimerHandle DamagePlayerTimerHandle;
	FTimerDelegate TimerDel;
};



