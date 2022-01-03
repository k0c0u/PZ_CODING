#include "Character/BaseCharacter.h"

#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

#include "Net/UnrealNetwork.h"
#include "Weapon/BaseWeapon.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TimerManager.h"


DEFINE_LOG_CATEGORY_STATIC(LogBaseCharacter, All, All);

//////////////////////////////////////////////////////////////////////////


ABaseCharacter::ABaseCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	/*bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;*/

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	CameraBoom->SocketOffset = FVector(0.0f, 100.0f, 80.0f);

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	//Set Health
	MaxHealth = 100.0f;
	CurrentHealth = MaxHealth;
	
	//ProjectileClass = ABaseProjectile::StaticClass();
	FireRate = 0.25f;
	bIsFiringWeapon = false;
}

void ABaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABaseCharacter, CurrentHealth);
	DOREPLIFETIME(ABaseCharacter, CurrentWeapon);
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (ensureMsgf(!WeaponClass, TEXT("ABaseCharacter::BeginPlay() WeaponClass is nullptr")))
	{
		UE_LOG(LogBaseCharacter, Error, TEXT("WeaponClass not selected"));
		return;
	}
	
	if(HasAuthority()) //if(GetLocalRole() == ROLE_Authority)
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParameters.Owner = this;
	
		CurrentWeapon = GetWorld()->SpawnActor<ABaseWeapon>(WeaponClass, SpawnParameters);
		if(!CurrentWeapon)
		{
			UE_LOG(LogBaseCharacter, Error, TEXT("CurrentWeapon in Character equal nullptr"));
			return;
		}
		
		CurrentWeapon->AttachToComponent(GetMesh(),FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponAttachSocketName);
	}
	
	// 1.Timer that restores the player's health up to 100 HP (3 health, every 2 seconds)
	//GetWorldTimerManager().SetTimer(RegenerationHealthTimerHandle, this, &ThisClass::RegenerationHealth, 2.0f, true);

	// 3. Timer that deals damage (10 damage, every second)
	//GetWorldTimerManager().SetTimer(DamageTimerHandle, this, &ThisClass::DamageToPlayer, 1.0f, true);
}

void ABaseCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if(CurrentWeapon)
	{
		CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	}
}
////PZ_08/////
float ABaseCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	float DamageApplied = CurrentHealth - DamageAmount;
	SetCurrentHealth(DamageApplied);
	return DamageApplied;
	
	////PZ_08/////
	//HealthData.Health -= DamageAmount;
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, FString::Printf(TEXT("HealthPlayer %f"), HealthData.Health));
	
}
////PZ_08/////

//////////////////////////////////////////////////////////////////////////
// Input

void ABaseCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ABaseCharacter::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ABaseCharacter::StopFire);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &ABaseCharacter::Reload);

	PlayerInputComponent->BindAxis("MoveForward", this, &ABaseCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABaseCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ABaseCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ABaseCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ABaseCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ABaseCharacter::TouchStopped);
}

void ABaseCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void ABaseCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void ABaseCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ABaseCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ABaseCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ABaseCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void ABaseCharacter::StartFire()
{
	if(CurrentWeapon)
	{
		CurrentWeapon->StartFire();
	}
	

	//////PZ_09/////
	/*if(!bIsFiringWeapon)
	{
		bIsFiringWeapon = true;
		GetWorldTimerManager().SetTimer(FiringTimer, this, &ThisClass::StopFire, FireRate, false);
		HandleFire();
	}*/
	//////PZ_09/////
	
}

void ABaseCharacter::StopFire()
{
	
	if(CurrentWeapon)
	{
		CurrentWeapon->StopFire();
	}
	
	//////PZ_09/////
	//bIsFiringWeapon = false;
	//////PZ_09/////
}

void ABaseCharacter::Reload()
{
	//CurrentWeapon->Reload();
	//UKismetSystemLibrary::DoesImplementInterface(CurrentWeapon, UReloadableInterface::StaticClass());
	
	if(CurrentWeapon->GetClass()->ImplementsInterface(UReloadableInterface::StaticClass()))
	{
		ReloadableInterface = CurrentWeapon;
		
		if(!ReloadableInterface) {return;}
		
		if(ReloadableInterface->CanReload())
		{
			IReloadableInterface::Execute_Reload(CurrentWeapon);
		}
	}
}

void ABaseCharacter::SetCurrentHealth(float HealthValue)
{
	if(GetLocalRole() == ROLE_Authority)
	{
		CurrentHealth = FMath::Clamp(HealthValue, 0.0f, MaxHealth);
		OnHealthUpdate();
	}
}

void ABaseCharacter::OnRep_CurrentHealth()
{
	OnHealthUpdate();
}

void ABaseCharacter::OnHealthUpdate()
{
	if(IsLocallyControlled())
	{
		const FString HealthMessage = FString::Printf(TEXT("You now have %f health remaining"), CurrentHealth);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, HealthMessage);

		if(CurrentHealth <=0)
		{
			const FString DeathMessage = FString::Printf(TEXT("You killed"));
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, DeathMessage);
		}
	}

	if(GetLocalRole() == ROLE_Authority)
	{
		const FString HealthMessage = FString::Printf(TEXT(" %s now has %f health remaining"),*GetFName().ToString(), CurrentHealth);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, HealthMessage);
	}
}

/*void ABaseCharacter::RegenerationHealth()
{
	if(Health < 100)
	{
		Health += RegenHealth;
	}
}

void ABaseCharacter::DamageToPlayer()
{
// 2.	
	if(Health > 0)
	{
		Health -= Damage;
		
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, FString::Printf(TEXT("HealthPlayer %f"), Health));

		// 5. When dealing damage to player, actor dealing damage too
		if(OnDamagePlayer.IsBound())
		{
			OnDamagePlayer.Broadcast();
		}
	}
	else
	{
		// 4. When destroying a player, destroy actor too
		if(OnDestroyPlayer.IsBound())
		{
			OnDestroyPlayer.Broadcast();
		}
		Destroy();
	}
}

	checkf(WeaponClass, TEXT("WeaponClass not selected"));
	
	check(WeaponClass);
	
	if (!ensure(WeaponClass))
	{
		UE_LOG(LogBaseCharacter, Error, TEXT("WeaponClass not selected"));
		return;
	}

	//ensureMsgf(!WeaponClass, TEXT("WeaponClass not selected!"));
	//ensureAlwaysMsgf(WeaponClass, TEXT( "ABaseCharacter::BeginPlay() WeaponClass is null" ) );
*/

