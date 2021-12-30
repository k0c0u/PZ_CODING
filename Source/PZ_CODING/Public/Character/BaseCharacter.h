#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/ReloadableInterface.h"
#include "BaseCharacter.generated.h"


DECLARE_EVENT(ABaseCharacter, FOnDestroyPlayer);
DECLARE_EVENT(ABaseCharacter, FOnDamagePlayer);

class ABaseWeapon;
class ABaseProjectile;

////PZ_08/////
/*USTRUCT(BlueprintType)
struct FHealthData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Health, meta = (ClampMin = "0.0", ClampMax = "100.0"))
	float Health = 100.0f;
};*/
////PZ_08/////


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
	//////PZ_09/////
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	//////PZ_09/////
protected:
	
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	
	void MoveForward(float Value);
	void MoveRight(float Value);

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

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface
	
public:

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void StartFire();
	
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void StopFire();
	
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void Reload();

	UFUNCTION(BlueprintCallable, Category = "Health")
	void SetCurrentHealth(float HealthValue);
	
	UFUNCTION(BlueprintCallable, BlueprintPure,Category = "Health")
	FORCEINLINE float GetMaxHealth() const {return MaxHealth;}

	UFUNCTION(BlueprintCallable, BlueprintPure,Category = "Health")
	FORCEINLINE float GetCurrentHealth() const {return CurrentHealth;}
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Stamina)
	float Stamina = 0.0f;
	
	IReloadableInterface* ReloadableInterface;
	
	FOnDestroyPlayer OnDestroyPlayer;
	FOnDamagePlayer OnDamagePlayer;

	////PZ_08/////
	//FHealthData HealthData;
	////PZ_08/////
	///
protected:

//////PZ_09/////

	UFUNCTION(Server, Reliable)
	void HandleFire();

	FTimerHandle FiringTimer;

	UFUNCTION()
	void OnRep_CurrentHealth();

	void OnHealthUpdate();

	UPROPERTY(EditAnywhere, ReplicatedUsing = OnRep_CurrentHealth, Category = "Health")
	float CurrentHealth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health", meta = (ClampMin = "0.0", ClampMax = "100.0"))
	float MaxHealth;

	UPROPERTY(EditDefaultsOnly, Category = " Projectile")
	float FireRate;

	bool bIsFiringWeapon;

	UPROPERTY(EditDefaultsOnly, Category = " Projectile")
	TSubclassOf<ABaseProjectile> ProjectileClass;
//////PZ_09////
	
	UPROPERTY(EditAnywhere, Category = "Weapon")
	TSubclassOf<ABaseWeapon> Weapon;
	
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Weapon")
	ABaseWeapon* CurrentWeapon = nullptr;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName WeaponAttachSocketName = "WeaponSocket";

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Health)
	float RegenHealth = 3.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
	float Damage = 10.0f;

	/*void RegenerationHealth();
	void DamageToPlayer();*/

	/*FTimerHandle RegenerationHealthTimerHandle;
	FTimerHandle DamageTimerHandle;*/
	
public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	
};

