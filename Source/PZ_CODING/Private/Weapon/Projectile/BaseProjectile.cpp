#include "Weapon/Projectile/BaseProjectile.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Character/BaseCharacter.h"

ABaseProjectile::ABaseProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	SphereComponent->InitSphereRadius(37.5f);
	SphereComponent->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	SetRootComponent(SphereComponent);

	/*static ConstructorHelpers::FObjectFinder<UStaticMesh> DefaultMesh(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
	if(DefaultMesh.Succeeded())
	{
		StaticMeshComponent->SetStaticMesh(DefaultMesh.Object);
	}*/
	
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(SphereComponent);
	//StaticMeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, -37.5f));
	StaticMeshComponent->SetRelativeScale3D(FVector(0.75f, 0.75f, 0.75f));

	/*static ConstructorHelpers::FObjectFinder<UParticleSystem> DefaultExplosionEffect(TEXT("/Game/StarterContent/Particles/P_Explosion.P_Explosion"));
	if(DefaultExplosionEffect.Succeeded())
	{
		ExplosionEffect = DefaultExplosionEffect;
	}*/

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovementComponent->SetUpdatedComponent(SphereComponent);
	ProjectileMovementComponent->InitialSpeed = 1500.0f;
	ProjectileMovementComponent->MaxSpeed = 1500.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;

	DamageType = UDamageType::StaticClass();
	
	Damage = 10.0f;

	if(GetLocalRole() == ROLE_Authority)
	{
		SphereComponent->OnComponentHit.AddDynamic(this, &ThisClass::OnProjectileImpact);
	}
}


void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseProjectile::Destroyed()
{
	FVector SpawnLocation = GetActorLocation();
	UGameplayStatics::SpawnEmitterAtLocation(this, ExplosionEffect, SpawnLocation, FRotator::ZeroRotator, true, EPSCPoolMethod::AutoRelease);
}

void ABaseProjectile::OnProjectileImpact(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if(ABaseCharacter* pBaseCharacter = Cast<ABaseCharacter>(OtherActor))
	{
		UGameplayStatics::ApplyPointDamage( pBaseCharacter, Damage, NormalImpulse, Hit, pBaseCharacter->Controller, this, DamageType);
	}
	
	Destroy();
}


void ABaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

