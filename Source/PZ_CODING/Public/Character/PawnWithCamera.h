#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PawnWithCamera.generated.h"

UCLASS()
class PZ_CODING_API APawnWithCamera : public APawn
{
	GENERATED_BODY()

public:
	
	APawnWithCamera();

protected:
	
	virtual void BeginPlay() override;

	//Input functions
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void PitchCamera(float AxisValue);
	void YawCamera(float AxisValue);
	void ZoomIn();
	void ZoomOut();
	void Jump();
	

	UPROPERTY(EditAnywhere)
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(EditAnywhere)
	class UCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMeshComp;
	
	//Input variables
	FVector MovementInput;
	FVector2D CameraInput;
	float ZoomFactor;
	bool bZoomingIn;
	bool bJumping;
	float JumpHeight;
	float CurrentJumpHeight;
	
public:	
	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
