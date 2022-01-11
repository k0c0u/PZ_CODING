#include "Weapon/BaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "TimerManager.h"
#include "Character/BaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Weapon/Projectile/BaseProjectile.h"


DEFINE_LOG_CATEGORY_STATIC(LogBaseWeapon, All, All);

ABaseWeapon::ABaseWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
	
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	RootComponent = WeaponMesh;

	SetReplicates(true);
	
}

void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();

	CurrentAmmo = MaxAmmo;
	CurrentAmmoInClip = AmmoPerClip;

	if (ensureMsgf(!ProjectileClass, TEXT("ABaseWeapon::BeginPlay() ProjectileClass is nullptr")))
	{
		UE_LOG(LogBaseWeapon, Error, TEXT("ProjectileClass not selected"));
		return;
	}
}

void ABaseWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABaseWeapon, CurrentAmmo);
	DOREPLIFETIME(ABaseWeapon, CurrentAmmoInClip);
}

void ABaseWeapon::StartFire()
{
	if (!CanFire()) {return;}

	if(GetLocalRole() < ROLE_Authority)
	{
		GetWorldTimerManager().SetTimer(FireHandle, this, &ABaseWeapon::ServerFire, TimeBetweenShots, true);
		
	}
	else
	{
		GetWorldTimerManager().SetTimer(FireHandle, this, &ABaseWeapon::Fire, TimeBetweenShots, true);
	}
}

void ABaseWeapon::StopFire()
{
	if(GetLocalRole() < ROLE_Authority)
	{
		GetWorldTimerManager().ClearTimer(FireHandle);
		
	}
	else
	{
		GetWorldTimerManager().ClearTimer(FireHandle);
	}
	//GetWorldTimerManager().ClearTimer(FireHandle);
}

bool ABaseWeapon::CanFire() const
{
	return CurrentAmmoInClip > 0 && CurrentAmmo >= 0 && !bIsReloading;
}

bool ABaseWeapon::CanReload() const
{
	return CurrentAmmoInClip < AmmoPerClip && CurrentAmmo > 0 && !bIsReloading;
}

void ABaseWeapon::Reload_Implementation()
{
	if (!CanReload()){ return;}
	
	StopFire();
	
	bIsReloading = true;
	
	if (CurrentAmmo == 0) {return;}
	
	--CurrentAmmo;
	
	CurrentAmmoInClip = AmmoPerClip;
	GetWorldTimerManager().SetTimer(ReloadHandle, this, &ABaseWeapon::ClearReloadTimer, ReloadDuration, false);

	if(!GetWorld()->IsServer())
	{
		ServerReload();
	}
}

void ABaseWeapon::UseAmmo()
{
	if (IsClipEmpty()) {return;}

	--CurrentAmmoInClip;

	if (IsClipEmpty() && !IsAmmoEmpty() || IsAmmoEmpty())
	{
		StopFire();
	}
}

bool ABaseWeapon::IsAmmoEmpty() const
{
	return CurrentAmmo == 0 && IsClipEmpty();
}

bool ABaseWeapon::IsClipEmpty() const
{
	return CurrentAmmoInClip == 0;
}

void ABaseWeapon::ClearReloadTimer()
{
	bIsReloading = false;
}

void ABaseWeapon::ServerReload_Implementation()
{
	Reload();
}

void ABaseWeapon::ServerFire_Implementation()
{
	Fire();
}

bool ABaseWeapon::ServerFire_Validate()
{
	return CanFire();
}
void ABaseWeapon::Fire()
{
	if (!GetWorld() || IsAmmoEmpty())
	{
		StopFire();
		return;
	}
	
	AActor* MyOwner = GetOwner();
	if(!MyOwner) {return;}
	
	!bProjectile ? LineTraceShot(MyOwner) : ProjectileShot(MyOwner);
	
	//DecreaseAmmo
	UseAmmo();
}

void ABaseWeapon::WeaponTrace(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd, AActor* MyOwner)
{
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(MyOwner);
	Params.AddIgnoredActor(this);
	
	if (!GetWorld()) {return;}
	//GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, ECollisionChannel::ECC_Visibility, Params);
	
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(MyOwner);
	UKismetSystemLibrary::LineTraceSingle(this, TraceStart, TraceEnd,
		UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility), false, ActorsToIgnore,
		EDrawDebugTrace::ForDuration, HitResult,true, FLinearColor::Red,
		FLinearColor::White, 0.3f);
}

bool ABaseWeapon::GetTraceStartEnd(AActor* MyOwner, FVector& TraceStart, FVector& TraceEnd, FVector& ShootDirection) const
{
	FVector EyeLocation;
	FRotator EyeRotation;
	if (!GetActorViewPoint(MyOwner, EyeLocation, EyeRotation)) return false;

	TraceStart = EyeLocation;
	ShootDirection = EyeRotation.Vector();
	TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
	return true;
}

bool ABaseWeapon::GetActorViewPoint(AActor* MyOwner, FVector& ViewLocation, FRotator& ViewRotation) const 
{
	if (!MyOwner) return false;
	
	MyOwner->GetActorEyesViewPoint(ViewLocation, ViewRotation);
	return true;
}
/*
void ABaseWeapon::HandleFire_Implementation()
{
	FVector SpawnLocation = GetActorLocation() + (GetControlRotation().Vector() * 100.0f) + (GetActorUpVector() * 50.0f);
	FRotator SpawnRotation = GetControlRotation();

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Instigator = GetInstigator();
	SpawnParameters.Owner = this;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	

	ABaseProjectile* BaseProjectile = GetWorld()->SpawnActor<ABaseProjectile>(ProjectileClass, SpawnLocation,  SpawnRotation, SpawnParameters);
}
*/
void ABaseWeapon::LineTraceShot(AActor* MyOwner)
{
	FVector TraceStart, TraceEnd, ShootDirection;
	
	if (!GetTraceStartEnd(MyOwner, TraceStart, TraceEnd, ShootDirection))
	{
		StopFire();
		return;
	}
	FHitResult Hit;
	WeaponTrace(Hit, TraceStart, TraceEnd, MyOwner);
	
	if(Hit.bBlockingHit)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, Hit.GetActor()->GetName());
		UGameplayStatics::ApplyPointDamage(Hit.GetActor(), Damage, ShootDirection, Hit, MyOwner->GetInstigatorController(), MyOwner, UDamageType::StaticClass());
	}
}

void ABaseWeapon::ProjectileShot(AActor* MyOwner)
{
	ABaseCharacter* pBaseCharacter = Cast<ABaseCharacter>(MyOwner);
	
	//FVector SpawnLocation = GetActorLocation() + (pBaseCharacter->GetControlRotation().Vector() * 100.0f) + (GetActorUpVector() * 50.0f);
	FVector SpawnLocation = WeaponMesh->GetSocketLocation(MuzzleSocketName);
	
	FRotator SpawnRotation = WeaponMesh->GetSocketRotation(MuzzleSocketName);
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = pBaseCharacter;
	
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	
	ABaseProjectile* BaseProjectile = GetWorld()->SpawnActor<ABaseProjectile>(ProjectileClass, SpawnLocation,  SpawnRotation, SpawnParams);
	

	/*const FVector Direction = (EndPoint - StartPoint).GetSafeNormal();
	const FTransform SpawnTransform(FRotator::ZeroRotator, StartPoint);
	ABaseCharacter* pBaseCharacter = Cast<ABaseCharacter>(MyOwner);
	if(!pBaseCharacter){return;}
	ABaseProjectile* Projectile = GetWorld()->SpawnActorDeferred<ABaseProjectile>(ProjectileClass, SpawnTransform, this, pBaseCharacter, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	if (Projectile)
	{
		Projectile->SetShotDirection(Direction);
		//Projectile->SetOwner(GetOwner());
		Projectile->SetDamage(AmmoTypes[CurrentAmmoType].BaseDamage);
		Projectile->FinishSpawning(SpawnTransform);
	}
*/
	
}