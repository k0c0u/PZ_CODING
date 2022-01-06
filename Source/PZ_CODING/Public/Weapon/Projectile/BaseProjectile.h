#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "BaseProjectile.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UProjectileMovementComponent;
class UParticleSystem;

UCLASS()
class PZ_CODING_API ABaseProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ABaseProjectile();
	
	virtual void Tick(float DeltaTime) override;
	
protected:
	
	virtual void BeginPlay() override;

//////PZ_09
	virtual void Destroyed() override;

	UFUNCTION(Category = "Projectile")
	void OnProjectileImpact(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* SphereComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditAnywhere, Category = "Effects")
	UParticleSystem* ExplosionEffect;

	UPROPERTY(EditAnywhere, Category = "Damage")
	TSubclassOf<class UDamageType> DamageType;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Damage")
	float Damage;
	
};
