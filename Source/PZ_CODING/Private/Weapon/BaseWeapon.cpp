#include "Weapon/BaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

ABaseWeapon::ABaseWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	RootComponent = WeaponMesh;
}

void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();

	CurrentAmmo = MaxAmmo;
	CurrentAmmoInClip = AmmoPerClip;
}

void ABaseWeapon::StartFire()
{
	if (!CanFire()) {return;}
	
	GetWorldTimerManager().SetTimer(FireHandle, this, &ABaseWeapon::WeaponTrace, TimeBetweenShots, true);
}

void ABaseWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(FireHandle);
}

bool ABaseWeapon::CanFire() const
{
	return CurrentAmmoInClip > 0 && CurrentAmmo > -1 && !bIsReloading;
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
}

void ABaseWeapon::UseAmmo()
{
	if (CurrentAmmoInClip == 0) {return;}

	--CurrentAmmoInClip;

	if (IsClipEmpty() && !IsAmmoEmpty())
	{
		StopFire();
	}
}

void ABaseWeapon::WeaponTrace()
{
	AActor* MyOwner = GetOwner();
	if(!MyOwner) {return;}
	
	FVector EyeLocation;
	FRotator EyeRotation;

	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	//FVector MuzzleLocation = WeaponMesh->GetSocketLocation(MuzzleSocketName);
	FVector ShootDirection = EyeRotation.Vector();
	FVector TraceEnd = EyeLocation + ShootDirection * Range;
	
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(MyOwner);
	Params.AddIgnoredActor(this);

	FHitResult Hit;
	if (!GetWorld()) {return;}
	GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, ECollisionChannel::ECC_Visibility, Params);
	
	if(Hit.bBlockingHit)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, Hit.GetActor()->GetName());
		UGameplayStatics::ApplyPointDamage(Hit.GetActor(), Damage, ShootDirection, Hit, MyOwner->GetInstigatorController(), MyOwner, UDamageType::StaticClass());
	}
	//DecreaseAmmo
	UseAmmo();
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