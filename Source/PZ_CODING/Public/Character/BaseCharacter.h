#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/ReloadableInterface.h"
#include "BaseCharacter.generated.h"

DECLARE_EVENT(ABaseCharacter, FOnDestroyPlayer);
DECLARE_EVENT(ABaseCharacter, FOnDamagePlayer);

class ABaseWeapon;

UCLASS(config=Game)
class ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	ABaseCharacter();
	
	void StartFire();
	void StopFire();
	void Reload();
	
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Health, meta = (ClampMin = "0.0", ClampMax = "100.0"))
	float Health = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Stamina)
	float Stamina = 0.0f;

	IReloadableInterface* ReloadableInterface;
	
	FOnDestroyPlayer OnDestroyPlayer;
	FOnDamagePlayer OnDamagePlayer;

protected:
	
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	void MoveForward(float Value);
	void MoveRight(float Value);

	void RegenerationHealth();
	void DamageToPlayer();

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);
	
	UPROPERTY(EditAnywhere, Category = "Weapon")
	TSubclassOf<ABaseWeapon> Weapon;
	
	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	ABaseWeapon* CurrentWeapon = nullptr;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName WeaponAttachSocketName = "WeaponSocket";

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Health)
	float RegenHealth = 3.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
	float Damage = 10.0f;
	
	FTimerHandle RegenerationHealthTimerHandle;
	FTimerHandle DamageTimerHandle;

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	
};

