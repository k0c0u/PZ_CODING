#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/ReloadableInterface.h"
#include "BaseWeapon.generated.h"

class USkeletalMeshComponent;
class ABaseProjectile;


UCLASS()
class PZ_CODING_API ABaseWeapon : public AActor, public IReloadableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseWeapon();
	
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void StartFire(); //должна вызываться когда игрок нажимает ЛКМ
	
	void StopFire();

	UFUNCTION(BlueprintCallable)
	int32 GetAmmoPerClip() const {return CurrentAmmoInClip;}
	UFUNCTION(BlueprintCallable)
	int32 GetCurrentAmmo() const {return CurrentAmmo;}
	
	//////// INTERFACE  IReloadableInterface /////////////
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Reload();
	virtual void Reload_Implementation(); // должна вызываться на кнопку R и когда после выстрела у оружия нет патронов
	//////// INTERFACE  IReloadableInterface /////////////
	
protected:
	
	virtual void BeginPlay() override;

	void Fire();
	bool GetTraceStartEnd(AActor* MyOwner, FVector& TraceStart, FVector& TraceEnd, FVector& ShootDirection) const;
	bool GetActorViewPoint(AActor* MyOwner, FVector& ViewLocation, FRotator& ViewRotation) const;
	void WeaponTrace(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd, AActor* MyOwner);
	void LineTraceShot(AActor* MyOwner);
	void ProjectileShot(AActor* MyOwner);
	bool CanFire() const; // проверяет кол-во патронов и должна вызываться когда игрок пытается выстрелить
	//bool CanReload() const; // проверяет можно ли перезарядить оружи
	virtual bool CanReload() const override;
	void UseAmmo(); // использует патроны в магазине
	
	bool IsAmmoEmpty() const;
	bool IsClipEmpty() const;
	void ClearReloadTimer();
	
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerFire();
	
	UFUNCTION(Server, Reliable)
	void ServerReload();
	/*UFUNCTION(Server, Reliable)
	void HandleFire();*/
	
	UPROPERTY(EditDefaultsOnly, Category = " Projectile")
	TSubclassOf<ABaseProjectile> ProjectileClass;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon")
	USkeletalMeshComponent* WeaponMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	float Damage = 30.0f; // кол-во урона которое наносит оружие

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	float ReloadDuration = 1.0f; //время перезарядки
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	float TraceMaxDistance = 1000.f; // дальность выстрела (трейса)

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName MuzzleSocketName = "MuzzleSocket"; //имя сокета, назначенное в меше

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	int32 MaxAmmo = 2; // максимальное кол-во патронов
	UPROPERTY(Replicated)
	int32 CurrentAmmo = 0; // текущее количество патронов у персонажа

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	int32 AmmoPerClip = 3; // кол-во патронов в магазине
	UPROPERTY(Replicated)
	int32 CurrentAmmoInClip = 0; // текущее количество патронов в магазине

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	float TimeBetweenShots = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = " Projectile")
	bool bProjectile = false;

	bool bIsReloading = false;

	FTimerHandle FireHandle;
	FTimerHandle ReloadHandle;
};