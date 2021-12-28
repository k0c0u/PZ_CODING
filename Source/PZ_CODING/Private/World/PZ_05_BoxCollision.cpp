#include "World/PZ_05_BoxCollision.h"
#include "Character/BaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Character/BaseCharacter.h"

APZ_05_BoxCollision::APZ_05_BoxCollision()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	SetRootComponent(BoxComponent);
	BoxComponent->bHiddenInGame = false;
}

void APZ_05_BoxCollision::BeginPlay()
{
	Super::BeginPlay();
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &APZ_05_BoxCollision::BeginOverlap);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &APZ_05_BoxCollision::EndOverlap);
	//OnChangeColor.AddDynamic(this, &APZ_05_BoxCollision::ChangeLocation);
}

void APZ_05_BoxCollision::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	BoxComponent->OnComponentBeginOverlap.RemoveDynamic(this, &APZ_05_BoxCollision::BeginOverlap);
	BoxComponent->OnComponentEndOverlap.RemoveDynamic(this, &APZ_05_BoxCollision::EndOverlap);
	
	if(GetWorldTimerManager().IsTimerActive(DamagePlayerTimerHandle))
	{
		GetWorldTimerManager().ClearTimer(DamagePlayerTimerHandle);
	}
}

/*void APZ_05_BoxCollision::ChangeLocation()
{
	const FVector NewLocation = (GetActorLocation() + FVector(100.0f,0.0f,0.0f));
	SetActorLocation(NewLocation);
}*/

void APZ_05_BoxCollision::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//OnChangeColor.Broadcast();

	////PZ_08/////
	if(ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(OtherActor))
	{
		TimerDel.BindUFunction(this, FName("DamagePlayer"), OtherActor);
		GetWorldTimerManager().SetTimer(DamagePlayerTimerHandle, TimerDel, 2.0f, true);
	}
}

////PZ_08/////
void APZ_05_BoxCollision::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(OtherActor);
	if(!BaseCharacter) {return;}

	if(BaseCharacter != OtherActor) {return;}

	GetWorldTimerManager().ClearTimer(DamagePlayerTimerHandle);
}

void APZ_05_BoxCollision::DamagePlayer(ABaseCharacter* BaseCharacter)
{
	UGameplayStatics::ApplyDamage(BaseCharacter, Damage, nullptr, this, UDamageType::StaticClass());
}


