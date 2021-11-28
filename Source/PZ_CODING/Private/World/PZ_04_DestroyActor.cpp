#include "World/PZ_04_DestroyActor.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Character/BaseCharacter.h"

// Sets default values
APZ_04_DestroyActor::APZ_04_DestroyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(StaticMesh);
}

// Called when the game starts or when spawned
void APZ_04_DestroyActor::BeginPlay()
{
	Super::BeginPlay();

	if(bDestroyForTimer)
	{
		// 2.Timer that destroys the cube after a certain amount of time
		GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &ThisClass::DestroyThisActor, TimeToDestroy, false);
	}

	if(!GetWorld()){return;}
	
	if((BaseCharacter = Cast<ABaseCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0))) != nullptr)
	{
		BaseCharacter->OnDestroyPlayer.AddUFunction(this, "DestroyThisActor");
		BaseCharacter->OnDamagePlayer.AddUFunction(this,"DamagePlayer");
	}
	
}

void APZ_04_DestroyActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	BaseCharacter->OnDestroyPlayer.RemoveAll(this);
}

void APZ_04_DestroyActor::DestroyThisActor()
{
	Destroy();
}

void APZ_04_DestroyActor::DamagePlayer()
{
	if(Health <= 0) {return;}
	
	Health -= Damage;
	
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("HealthActor %f"), Health));
}