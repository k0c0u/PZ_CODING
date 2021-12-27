
#include "World/PZ_08_Cube.h"
#include "TimerManager.h"
#include "World/PZ_04_DestroyActor.h"

APZ_08_Cube::APZ_08_Cube()
{
	PrimaryActorTick.bCanEverTick = false;
}

void APZ_08_Cube::BeginPlay()
{
	Super::BeginPlay();
	
	for (int32 Index = 0; Index !=Count; ++Index)
	{
		auto Actor = GetWorld()->SpawnActor<AActor>(ActorToSpawn, (GetActorLocation() + FVector(100.0f * Index, 0.0f, 0.0f)),FRotator::ZeroRotator);
		if (!Actor) continue;
		
		ActorArray.Add(Actor);
	}

	GetWorldTimerManager().SetTimer(DestroyActorTimerHandle, this, &ThisClass::DestroyActor, 5.0f, false);
	GetWorldTimerManager().SetTimer(ChangeStatusTimerHandle, this, &ThisClass::ChangeStatus, 1.0f, true);
}

void APZ_08_Cube::DestroyActor()
{
	for(auto Actor : ActorArray)
	{
		Actor->Destroy();
	}
	ActorArray.Empty();
	GetWorldTimerManager().ClearTimer(DestroyActorTimerHandle);
}

void APZ_08_Cube::ChangeStatus()
{
	for(auto Actor : ActorArray)
	{
		if(APZ_04_DestroyActor* DestroyActor = Cast<APZ_04_DestroyActor>(Actor))
		{
			DestroyActor->CurrentStatus = DestroyActor->CurrentStatus == Status::Attacking ? Status::Stopped : Status::Attacking;
		}
	}
	
	GetWorldTimerManager().ClearTimer(ChangeStatusTimerHandle);
}

