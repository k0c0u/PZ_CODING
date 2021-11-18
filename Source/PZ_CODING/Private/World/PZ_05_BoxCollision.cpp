#include "World/PZ_05_BoxCollision.h"

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
	OnChangeColor.AddDynamic(this, &APZ_05_BoxCollision::ChangeLocation);
}

void APZ_05_BoxCollision::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OnChangeColor.Broadcast();
}

void APZ_05_BoxCollision::ChangeLocation()
{
	const FVector NewLocation = (GetActorLocation() + FVector(100.0f,0.0f,0.0f));
	SetActorLocation(NewLocation);
}