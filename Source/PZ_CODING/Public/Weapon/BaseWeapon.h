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

	void Fire(); //должна вызываться когда игрок нажимает ЛКМ
	void CanFire(); // проверяет кол-во патронов и должна вызываться когда игрок пытается выстрелить
	void CanReload(); // проверяет можно ли перезарядить оружие
	void Reload(); // должна вызываться на кнопку R и когда после выстрела у оружия нет патронов
	void UseAmmo(); // использует патроны в магазине
	void WeaponTrace(); // Реализация HitScan

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon")
	USkeletalMeshComponent* WeaponMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	int32 MaxAmmo = 30; // максимальное кол-во патронов

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	float Damage = 30.0f; // кол-во урона которое наносит оружие

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	float ReloadDuration = 1.0f; //время перезарядки
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	float Range = 1000.f; // дальность выстрела (трейса)

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName MuzzleSocketName; //имя сокета, назначенное в меше

	int32 AmmoPerClip = 0; // кол-во патронов в магазине
	int32 CurrentAmmo = 0; // текущее количество патронов у персонажа
	int32 CurrentAmmoInClip = 0; // текущее количество патронов в магазине
	bool bIsReloading = false;

};
