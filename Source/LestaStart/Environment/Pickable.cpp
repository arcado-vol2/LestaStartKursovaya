// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickable.h"

#include "Components/BoxComponent.h"
#include "LestaStart/Core/LestaCharacter.h"


// Sets default values
APickable::APickable()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	SetRootComponent(TriggerVolume);
	VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	VisualMesh->SetupAttachment(TriggerVolume);
	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &APickable::OnPeakUp);
}

// Called when the game starts or when spawned
void APickable::BeginPlay()
{
	Super::BeginPlay();
	
}

void APickable::OnPeakUp(UPrimitiveComponent* OverlappedComp, AActor* PickerActor, UPrimitiveComponent* PickerComp, int32 PickerBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HasAuthority())
	{
		return;
	}
	if (ALestaCharacter* PickedPlayer =Cast<ALestaCharacter>(PickerActor))
	{
		Server_OnPeakUp(PickedPlayer);
	}
}

void APickable::Server_OnPeakUp_Implementation(ALestaCharacter* PickerPlayer)
{
	Destroy();
}

