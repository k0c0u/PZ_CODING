#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"

class USkeletalMeshComponent;

UCLASS()
class PZ_CODING_API ABaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseWeapon();
	
	void StartFire(); //должна вызываться когда игрок нажимает ЛКМ
	void StopFire();
	void Reload();	// должна вызываться на кнопку R и когда после выстрела у оружия нет патронов
	
protected:
	
	virtual void BeginPlay() override;
	
	bool CanFire() const; // проверяет кол-во патронов и должна вызываться когда игрок пытается выстрелить
	bool CanReload() const; // проверяет можно ли перезарядить оружие
	void UseAmmo(); // использует патроны в магазине
	void WeaponTrace(); // Реализация HitSca
	bool IsAmmoEmpty() const;
	bool IsClipEmpty() const;
	void ClearReloadTimer();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon")
	USkeletalMeshComponent* WeaponMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	float Damage = 30.0f; // кол-во урона которое наносит оружие

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	float ReloadDuration = 1.0f; //время перезарядки
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	float Range = 1000.f; // дальность выстрела (трейса)

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName MuzzleSocketName = "MuzzleSocket"; //имя сокета, назначенное в меше

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	int32 MaxAmmo = 2; // максимальное кол-во патронов
	int32 CurrentAmmo = 0; // текущее количество патронов у персонажа

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	int32 AmmoPerClip = 3; // кол-во патронов в магазине
	int32 CurrentAmmoInClip = 0; // текущее количество патронов в магазине

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	float TimeBetweenShots = 0.2f;
	
	bool bIsReloading = false;

	FTimerHandle FireHandle;
	FTimerHandle ReloadHandle;
};
