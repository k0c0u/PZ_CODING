// Fill out your copyright notice in the Description page of Project Settings.


#include "World/PZ_07_SpyCameraActor.h"
#include "Character/PZ_07_BasePawn.h"
#include "TimerManager.h"

APZ_07_SpyCameraActor::APZ_07_SpyCameraActor()
{
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComp->SetBoxExtent(FVector(500.0f));
	BoxComp->SetCollisionObjectType(ECC_WorldStatic);
	BoxComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxComp->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);
	BoxComp->SetupAttachment(GetRootComponent());
}

void APZ_07_SpyCameraActor::BeginPlay()
{
	Super::BeginPlay();

	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &APZ_07_SpyCameraActor::BoxBeginOverlap);
	BoxComp->OnComponentEndOverlap.AddDynamic(this, &APZ_07_SpyCameraActor::BoxEndOverlap);
}

void APZ_07_SpyCameraActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	BoxComp->OnComponentBeginOverlap.RemoveDynamic(this, &APZ_07_SpyCameraActor::BoxBeginOverlap);
	BoxComp->OnComponentEndOverlap.RemoveDynamic(this, &APZ_07_SpyCameraActor::BoxEndOverlap);
}

void APZ_07_SpyCameraActor::BoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	FViewTargetTransitionParams TransitionParams;
	TransitionParams.BlendTime = 2.0f;
	TransitionParams.bLockOutgoing = true;
	PC->SetViewTarget(this, TransitionParams);

	if((BasePawn = Cast<APZ_07_BasePawn>(OtherActor)) != nullptr)
	{
		GetWorldTimerManager().SetTimer(DetectionTimer, this, &APZ_07_SpyCameraActor::DetectionPlayer, 1.0f, false);
	}
}

void APZ_07_SpyCameraActor::BoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	FViewTargetTransitionParams TransitionParams;
	TransitionParams.BlendTime = 2.0f;
	TransitionParams.bLockOutgoing = true;
	PC->SetViewTarget(PC->GetPawn(), TransitionParams);

	if(Cast<APZ_07_BasePawn>(OtherActor))
	{
		BasePawn = nullptr;
		if(GetWorldTimerManager().IsTimerActive(DetectionTimer))
		{
			GetWorldTimerManager().ClearTimer(DetectionTimer);
		}
	}
}

void APZ_07_SpyCameraActor::DetectionPlayer()
{
	if (!BasePawn) {return;}

	UStaticMeshComponent* Mesh = BasePawn->FindComponentByClass<UStaticMeshComponent>();
	if (!Mesh) {return;}

	UMaterialInterface* Material = Mesh->GetMaterial(0);
	if (!Material) {return;}

	UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(Material,nullptr);
	if (!DynamicMaterial) {return;}

	Mesh->SetMaterial(0, DynamicMaterial);
	
	DynamicMaterial->SetScalarParameterValue("ChangeColor", 1.0f);
}
